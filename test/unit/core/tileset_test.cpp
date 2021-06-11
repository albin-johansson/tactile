#include "tileset.hpp"

#include <gtest/gtest.h>

#include "smart_pointers.hpp"
#include "tactile_error.hpp"

using namespace tactile;

class TilesetTest : public testing::Test
{
 public:
  [[maybe_unused]] static void SetUpTestSuite()
  {
    m_tileset = std::make_unique<core::Tileset>(1_t,
                                                TACTILE_QSTRING(u"terrain.png"),
                                                32_tw,
                                                32_th);
  }

  [[maybe_unused]] static void TearDownTestSuite()
  {
    m_tileset.reset();
  }

  inline static unique<core::Tileset> m_tileset;
};

TEST_F(TilesetTest, ImageConstructor)
{
  const QImage goodImage{TACTILE_QSTRING(u"terrain.png")};
  EXPECT_NO_THROW((core::Tileset{1_t, goodImage, 32_tw, 32_th}));

  EXPECT_THROW((core::Tileset{1_t, goodImage, 0_tw, 32_th}), TactileError);
  EXPECT_THROW((core::Tileset{1_t, goodImage, 32_tw, 0_th}), TactileError);

  const QImage badImage{TACTILE_QSTRING(u"dummy.png")};
  EXPECT_THROW((core::Tileset{1_t, badImage, 32_tw, 32_th}), TactileError);
}

TEST_F(TilesetTest, PathConstructor)
{
  const auto goodPath = TACTILE_QSTRING(u"terrain.png");
  EXPECT_NO_THROW((core::Tileset{1_t, goodPath, 32_tw, 32_th}));

  EXPECT_THROW((core::Tileset{1_t, goodPath, 0_tw, 32_th}), TactileError);
  EXPECT_THROW((core::Tileset{1_t, goodPath, 32_tw, 0_th}), TactileError);

  const auto badPath = TACTILE_QSTRING(u"dummy.png");
  EXPECT_THROW((core::Tileset{1_t, badPath, 32_tw, 32_th}), TactileError);
}

TEST_F(TilesetTest, VisitSelection)
{
  {  // Empty selection
    m_tileset->ClearSelection();

    auto count = 0;
    m_tileset->VisitSelection([&](const core::Position) { ++count; });

    EXPECT_EQ(0, count);
  }

  {  // Non-empty selection
    constexpr core::Position topLeft{1_row, 2_col};
    constexpr core::Position bottomRight{4_row, 5_col};
    m_tileset->SetSelection({topLeft, bottomRight});

    /*  0 1 2 3 4 5 6 7

     0  X X X X X X X X ...
     1  X X T o o o X X ...
     2  X X o o o o X X ...
     3  X X o o o o X X ...
     4  X X o o o B X X ...
     5  X X X X X X X X ...
     6  X X X X X X X X ...
     */

    auto count = 0;
    m_tileset->VisitSelection([&](const core::Position) { ++count; });

    EXPECT_EQ(16, count);
  }

  m_tileset->ClearSelection();
}

TEST_F(TilesetTest, SetSelection)
{
  const core::TilesetSelection selection{{1_row, 2_col}, {7_row, 8_col}};

  m_tileset->SetSelection(selection);
  ASSERT_TRUE(m_tileset->GetSelection());

  EXPECT_EQ(selection.topLeft, m_tileset->GetSelection()->topLeft);
  EXPECT_EQ(selection.bottomRight, m_tileset->GetSelection()->bottomRight);

  m_tileset->ClearSelection();
}

TEST_F(TilesetTest, ClearSelection)
{
  m_tileset->ClearSelection();
  EXPECT_FALSE(m_tileset->GetSelection());

  const core::TilesetSelection selection{{1_row, 2_col}, {7_row, 8_col}};
  m_tileset->SetSelection(selection);
  EXPECT_TRUE(m_tileset->GetSelection());

  m_tileset->ClearSelection();
  EXPECT_FALSE(m_tileset->GetSelection());

  EXPECT_NO_THROW(m_tileset->ClearSelection());
  EXPECT_FALSE(m_tileset->GetSelection());
}

TEST_F(TilesetTest, SetName)
{
  const auto name = TACTILE_QSTRING(u"Gandalf");

  m_tileset->SetName(name);
  EXPECT_EQ(name, m_tileset->Name());
}

TEST_F(TilesetTest, SetPath)
{
  const QFileInfo file{TACTILE_QSTRING(u"interior.png")};

  m_tileset->SetPath(file);
  EXPECT_EQ(file, m_tileset->File());
}

TEST_F(TilesetTest, Contains)
{
  const auto first = m_tileset->FirstId();
  const auto count = m_tileset->TileCount();

  EXPECT_FALSE(m_tileset->Contains(-1_t));
  EXPECT_FALSE(m_tileset->Contains(0_t));

  EXPECT_TRUE(m_tileset->Contains(1_t));
  EXPECT_TRUE(m_tileset->Contains(first + tile_id{count}));

  EXPECT_FALSE(m_tileset->Contains(first + tile_id{count + 1}));
}

TEST_F(TilesetTest, IsSingleTileSelected)
{
  EXPECT_FALSE(m_tileset->IsSingleTileSelected());

  m_tileset->SetSelection({{1_row, 1_col}, {1_row, 1_col}});
  EXPECT_TRUE(m_tileset->IsSingleTileSelected());

  m_tileset->SetSelection({{1_row, 1_col}, {2_row, 1_col}});
  EXPECT_FALSE(m_tileset->IsSingleTileSelected());

  m_tileset->ClearSelection();
}

TEST_F(TilesetTest, TileAt)
{
  EXPECT_NE(empty, m_tileset->TileAt({0_row, 0_col}));
  EXPECT_NE(empty, m_tileset->TileAt({0_row, m_tileset->ColumnCount() - 1_col}));
  EXPECT_NE(empty, m_tileset->TileAt({m_tileset->RowCount() - 1_row, 0_col}));

  EXPECT_EQ(empty, m_tileset->TileAt({0_row, m_tileset->ColumnCount()}));
  EXPECT_EQ(empty, m_tileset->TileAt({m_tileset->RowCount(), 0_col}));
}

TEST_F(TilesetTest, Width)
{
  EXPECT_EQ(m_tileset->ColumnCount().get() * m_tileset->GetTileWidth().get(),
            m_tileset->Width());
}

TEST_F(TilesetTest, Height)
{
  EXPECT_EQ(m_tileset->RowCount().get() * m_tileset->GetTileHeight().get(),
            m_tileset->Height());
}

TEST_F(TilesetTest, ImageSource)
{
  EXPECT_FALSE(m_tileset->ImageSource(0_t));

  const auto source = m_tileset->ImageSource(2_t);
  ASSERT_TRUE(source);

  EXPECT_EQ(0, source->y());
  EXPECT_EQ(m_tileset->GetTileWidth().get(), source->x());
}

TEST_F(TilesetTest, GetTileWidth)
{
  EXPECT_EQ(32_tw, m_tileset->GetTileWidth());
}

TEST_F(TilesetTest, GetTileHeight)
{
  EXPECT_EQ(32_th, m_tileset->GetTileHeight());
}

TEST_F(TilesetTest, FirstID)
{
  EXPECT_EQ(1_t, m_tileset->FirstId());
}

TEST_F(TilesetTest, LastID)
{
  EXPECT_EQ(m_tileset->FirstId() + tile_id{m_tileset->TileCount()},
            m_tileset->LastId());
}

TEST_F(TilesetTest, TileCount)
{
  EXPECT_EQ(1024, m_tileset->TileCount());
  EXPECT_EQ(m_tileset->LastId().get() - m_tileset->FirstId().get(),
            m_tileset->TileCount());
}

TEST_F(TilesetTest, FilePath)
{
  EXPECT_EQ(m_tileset->File().filePath(), m_tileset->FilePath());
}
