#include "core/tileset/tileset.hpp"

#include <gtest/gtest.h>

#include "core/tactile_error.hpp"
#include "utils/load_texture.hpp"

using namespace Tactile;

class TilesetTest : public testing::Test
{
 public:
  [[maybe_unused]] static void SetUpTestSuite()
  {
    const auto info = LoadTexture("resources/terrain.png");
    ASSERT_TRUE(info.has_value());

    mTexture = info->texture;
    mTileset = std::make_unique<Tileset>(1_tile, *info, 32, 32);
  }

  [[maybe_unused]] static void TearDownTestSuite()
  {
    glDeleteTextures(1, &mTexture.value());
    mTileset.reset();
  }

  inline static Unique<Tileset> mTileset;
  inline static Maybe<uint> mTexture;
};

TEST_F(TilesetTest, VisitSelection)
{
  {  // Empty selection
    mTileset->ClearSelection();

    int count = 0;
    mTileset->VisitSelection([&](const MapPosition pos) { ++count; });

    ASSERT_EQ(0, count);
  }

  {  // Non-empty selection
    const MapPosition topLeft{1_row, 2_col};
    const MapPosition bottomRight{4_row, 5_col};
    mTileset->SetSelection({topLeft, bottomRight});

    /*  0 1 2 3 4 5 6 7
     0  X X X X X X X X ...
     1  X X T o o o X X ...
     2  X X o o o o X X ...
     3  X X o o o o X X ...
     4  X X o o o B X X ...
     5  X X X X X X X X ...
     6  X X X X X X X X ...
     */

    int count = 0;
    mTileset->VisitSelection([&](const MapPosition pos) { ++count; });

    ASSERT_EQ(16, count);
  }

  mTileset->ClearSelection();
}

TEST_F(TilesetTest, SetSelection)
{
  const TilesetSelection selection{{1_row, 2_col}, {7_row, 8_col}};

  mTileset->SetSelection(selection);
  ASSERT_TRUE(mTileset->GetSelection());

  ASSERT_EQ(selection.top_left, mTileset->GetSelection()->top_left);
  ASSERT_EQ(selection.bottom_right, mTileset->GetSelection()->bottom_right);

  mTileset->ClearSelection();
}

TEST_F(TilesetTest, ClearSelection)
{
  mTileset->ClearSelection();
  ASSERT_FALSE(mTileset->GetSelection());

  const TilesetSelection selection{{1_row, 2_col}, {7_row, 8_col}};
  mTileset->SetSelection(selection);
  ASSERT_TRUE(mTileset->GetSelection());

  mTileset->ClearSelection();
  ASSERT_FALSE(mTileset->GetSelection());

  ASSERT_NO_THROW(mTileset->ClearSelection());
  ASSERT_FALSE(mTileset->GetSelection());
}

TEST_F(TilesetTest, SetName)
{
  const auto name = "Gandalf";

  mTileset->SetName(name);
  ASSERT_EQ(name, mTileset->GetName());
}

TEST_F(TilesetTest, SetPath)
{
  const std::filesystem::path file{"resources/interior.png"};

  mTileset->SetPath(file);
  ASSERT_EQ(file, mTileset->GetFilePath());
}

TEST_F(TilesetTest, Contains)
{
  const auto first = mTileset->GetFirstId();
  const auto count = mTileset->GetTileCount();

  ASSERT_FALSE(mTileset->Contains(-1_tile));
  ASSERT_FALSE(mTileset->Contains(0_tile));

  ASSERT_TRUE(mTileset->Contains(1_tile));
  ASSERT_TRUE(mTileset->Contains(first + tile_id{count}));

  ASSERT_FALSE(mTileset->Contains(first + tile_id{count + 1}));
}

TEST_F(TilesetTest, IsSingleTileSelected)
{
  ASSERT_FALSE(mTileset->IsSingleTileSelected());

  mTileset->SetSelection({{1_row, 1_col}, {1_row, 1_col}});
  ASSERT_TRUE(mTileset->IsSingleTileSelected());

  mTileset->SetSelection({{1_row, 1_col}, {2_row, 1_col}});
  ASSERT_FALSE(mTileset->IsSingleTileSelected());

  mTileset->ClearSelection();
}

TEST_F(TilesetTest, GetTile)
{
  ASSERT_NE(empty_tile, mTileset->GetTile({0_row, 0_col}));
  ASSERT_NE(empty_tile,
            mTileset->GetTile({0_row, mTileset->GetColumnCount() - 1_col}));
  ASSERT_NE(empty_tile,
            mTileset->GetTile({mTileset->GetRowCount() - 1_row, 0_col}));

  ASSERT_EQ(empty_tile, mTileset->GetTile({0_row, mTileset->GetColumnCount()}));
  ASSERT_EQ(empty_tile, mTileset->GetTile({mTileset->GetRowCount(), 0_col}));
}

TEST_F(TilesetTest, GetWidth)
{
  ASSERT_EQ(mTileset->GetColumnCount().get() * mTileset->GetTileWidth(),
            mTileset->GetWidth());
}

TEST_F(TilesetTest, GetHeight)
{
  ASSERT_EQ(mTileset->GetRowCount().get() * mTileset->GetTileHeight(),
            mTileset->GetHeight());
}

TEST_F(TilesetTest, GetImageSource)
{
  ASSERT_FALSE(mTileset->GetImageSource(0_tile));

  const auto source = mTileset->GetImageSource(2_tile);
  ASSERT_TRUE(source);

  ASSERT_EQ(0, source->y());
  ASSERT_EQ(mTileset->GetTileWidth(), source->x());
}

TEST_F(TilesetTest, GetTileWidth)
{
  ASSERT_EQ(32, mTileset->GetTileWidth());
}

TEST_F(TilesetTest, GetTileHeight)
{
  ASSERT_EQ(32, mTileset->GetTileHeight());
}

TEST_F(TilesetTest, GetFirstID)
{
  ASSERT_EQ(1_tile, mTileset->GetFirstId());
}

TEST_F(TilesetTest, GetLastID)
{
  ASSERT_EQ(mTileset->GetFirstId() + tile_id{mTileset->GetTileCount()},
            mTileset->GetLastId());
}

TEST_F(TilesetTest, GetTileCount)
{
  ASSERT_EQ(1024, mTileset->GetTileCount());
  ASSERT_EQ(mTileset->GetLastId().get() - mTileset->GetFirstId().get(),
            mTileset->GetTileCount());
}
