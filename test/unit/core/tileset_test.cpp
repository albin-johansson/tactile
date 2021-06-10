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
    m_tileset = std::make_unique<core::tileset>(1_t,
                                                TACTILE_QSTRING(u"terrain.png"),
                                                32_tw,
                                                32_th);
  }

  [[maybe_unused]] static void TearDownTestSuite()
  {
    m_tileset.reset();
  }

  inline static unique<core::tileset> m_tileset;
};

TEST_F(TilesetTest, ImageConstructor)
{
  const QImage goodImage{TACTILE_QSTRING(u"terrain.png")};
  EXPECT_NO_THROW((core::tileset{1_t, goodImage, 32_tw, 32_th}));

  EXPECT_THROW((core::tileset{1_t, goodImage, 0_tw, 32_th}), tactile_error);
  EXPECT_THROW((core::tileset{1_t, goodImage, 32_tw, 0_th}), tactile_error);

  const QImage badImage{TACTILE_QSTRING(u"dummy.png")};
  EXPECT_THROW((core::tileset{1_t, badImage, 32_tw, 32_th}), tactile_error);
}

TEST_F(TilesetTest, PathConstructor)
{
  const auto goodPath = TACTILE_QSTRING(u"terrain.png");
  EXPECT_NO_THROW((core::tileset{1_t, goodPath, 32_tw, 32_th}));

  EXPECT_THROW((core::tileset{1_t, goodPath, 0_tw, 32_th}), tactile_error);
  EXPECT_THROW((core::tileset{1_t, goodPath, 32_tw, 0_th}), tactile_error);

  const auto badPath = TACTILE_QSTRING(u"dummy.png");
  EXPECT_THROW((core::tileset{1_t, badPath, 32_tw, 32_th}), tactile_error);
}

TEST_F(TilesetTest, IterateSelection)
{
  {  // Empty selection
    m_tileset->clear_selection();

    auto count = 0;
    m_tileset->iterate_selection([&](const core::Position) {
      ++count;
    });

    EXPECT_EQ(0, count);
  }

  {  // Non-empty selection
    constexpr core::Position topLeft{1_row, 2_col};
    constexpr core::Position bottomRight{4_row, 5_col};
    m_tileset->set_selection({topLeft, bottomRight});

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
    m_tileset->iterate_selection([&](const core::Position) {
      ++count;
    });

    EXPECT_EQ(16, count);
  }

  m_tileset->clear_selection();
}

TEST_F(TilesetTest, SetSelection)
{
  const core::tileset_selection selection{{1_row, 2_col}, {7_row, 8_col}};

  m_tileset->set_selection(selection);
  ASSERT_TRUE(m_tileset->get_selection());

  EXPECT_EQ(selection.topLeft, m_tileset->get_selection()->topLeft);
  EXPECT_EQ(selection.bottomRight, m_tileset->get_selection()->bottomRight);

  m_tileset->clear_selection();
}

TEST_F(TilesetTest, ClearSelection)
{
  m_tileset->clear_selection();
  EXPECT_FALSE(m_tileset->get_selection());

  const core::tileset_selection selection{{1_row, 2_col}, {7_row, 8_col}};
  m_tileset->set_selection(selection);
  EXPECT_TRUE(m_tileset->get_selection());

  m_tileset->clear_selection();
  EXPECT_FALSE(m_tileset->get_selection());

  EXPECT_NO_THROW(m_tileset->clear_selection());
  EXPECT_FALSE(m_tileset->get_selection());
}

TEST_F(TilesetTest, SetName)
{
  const auto name = TACTILE_QSTRING(u"Gandalf");

  m_tileset->set_name(name);
  EXPECT_EQ(name, m_tileset->name());
}

TEST_F(TilesetTest, SetPath)
{
  const QFileInfo file{TACTILE_QSTRING(u"interior.png")};

  m_tileset->set_path(file);
  EXPECT_EQ(file, m_tileset->file());
}

TEST_F(TilesetTest, Contains)
{
  const auto first = m_tileset->first_id();
  const auto count = m_tileset->tile_count();

  EXPECT_FALSE(m_tileset->contains(-1_t));
  EXPECT_FALSE(m_tileset->contains(0_t));

  EXPECT_TRUE(m_tileset->contains(1_t));
  EXPECT_TRUE(m_tileset->contains(first + tile_id{count}));

  EXPECT_FALSE(m_tileset->contains(first + tile_id{count + 1}));
}

TEST_F(TilesetTest, IsSingleTileSelected)
{
  EXPECT_FALSE(m_tileset->is_single_tile_selected());

  m_tileset->set_selection({{1_row, 1_col}, {1_row, 1_col}});
  EXPECT_TRUE(m_tileset->is_single_tile_selected());

  m_tileset->set_selection({{1_row, 1_col}, {2_row, 1_col}});
  EXPECT_FALSE(m_tileset->is_single_tile_selected());

  m_tileset->clear_selection();
}

TEST_F(TilesetTest, TileAt)
{
  EXPECT_NE(empty, m_tileset->tile_at({0_row, 0_col}));
  EXPECT_NE(empty, m_tileset->tile_at({0_row, m_tileset->col_count() - 1_col}));
  EXPECT_NE(empty, m_tileset->tile_at({m_tileset->row_count() - 1_row, 0_col}));

  EXPECT_EQ(empty, m_tileset->tile_at({0_row, m_tileset->col_count()}));
  EXPECT_EQ(empty, m_tileset->tile_at({m_tileset->row_count(), 0_col}));
}

TEST_F(TilesetTest, Width)
{
  EXPECT_EQ(m_tileset->col_count().get() * m_tileset->get_tile_width().get(),
            m_tileset->width());
}

TEST_F(TilesetTest, Height)
{
  EXPECT_EQ(m_tileset->row_count().get() * m_tileset->get_tile_height().get(),
            m_tileset->height());
}

TEST_F(TilesetTest, ImageSource)
{
  EXPECT_FALSE(m_tileset->image_source(0_t));

  const auto source = m_tileset->image_source(2_t);
  ASSERT_TRUE(source);

  EXPECT_EQ(0, source->y());
  EXPECT_EQ(m_tileset->get_tile_width().get(), source->x());
}

TEST_F(TilesetTest, GetTileWidth)
{
  EXPECT_EQ(32_tw, m_tileset->get_tile_width());
}

TEST_F(TilesetTest, GetTileHeight)
{
  EXPECT_EQ(32_th, m_tileset->get_tile_height());
}

TEST_F(TilesetTest, FirstID)
{
  EXPECT_EQ(1_t, m_tileset->first_id());
}

TEST_F(TilesetTest, LastID)
{
  EXPECT_EQ(m_tileset->first_id() + tile_id{m_tileset->tile_count()},
            m_tileset->last_id());
}

TEST_F(TilesetTest, TileCount)
{
  EXPECT_EQ(1024, m_tileset->tile_count());
  EXPECT_EQ(m_tileset->last_id().get() - m_tileset->first_id().get(),
            m_tileset->tile_count());
}

TEST_F(TilesetTest, FilePath)
{
  EXPECT_EQ(m_tileset->file().filePath(), m_tileset->file_path());
}
