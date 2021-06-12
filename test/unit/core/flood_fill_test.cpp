#include "flood_fill.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(FloodFill, Correctness)
{
  core::TileLayer layer;
  for (int i = 0; i < 5; ++i)
  {
    layer.SetTile({row_t{i}, col_t{i}}, 1_t);
  }

  /*
   1 0 0 0 0
   0 1 0 0 0
   0 0 1 0 0
   0 0 0 1 0
   0 0 0 0 1
   */

  const auto replacement = 2_t;
  std::vector<core::Position> positions;
  layer.Flood({1_row, 0_col}, replacement, positions);

  /*
   Expected:
   1 0 0 0 0
   2 1 0 0 0
   2 2 1 0 0
   2 2 2 1 0
   2 2 2 2 1
   */

  {  // Affected tiles
    EXPECT_EQ(replacement, layer.TileAt({1_row, 0_col}));
    EXPECT_EQ(replacement, layer.TileAt({2_row, 0_col}));
    EXPECT_EQ(replacement, layer.TileAt({3_row, 0_col}));
    EXPECT_EQ(replacement, layer.TileAt({4_row, 0_col}));

    EXPECT_EQ(replacement, layer.TileAt({2_row, 1_col}));
    EXPECT_EQ(replacement, layer.TileAt({3_row, 1_col}));
    EXPECT_EQ(replacement, layer.TileAt({4_row, 1_col}));

    EXPECT_EQ(replacement, layer.TileAt({3_row, 2_col}));
    EXPECT_EQ(replacement, layer.TileAt({4_row, 2_col}));

    EXPECT_EQ(replacement, layer.TileAt({4_row, 3_col}));
  }

  {  // Unaffected diagonal blocking the flood
    EXPECT_EQ(1_t, layer.TileAt({0_row, 0_col}));
    EXPECT_EQ(1_t, layer.TileAt({1_row, 1_col}));
    EXPECT_EQ(1_t, layer.TileAt({2_row, 2_col}));
    EXPECT_EQ(1_t, layer.TileAt({3_row, 3_col}));
    EXPECT_EQ(1_t, layer.TileAt({4_row, 4_col}));
  }

  {  // Unaffected right hand side
    EXPECT_EQ(empty, layer.TileAt({0_row, 1_col}));
    EXPECT_EQ(empty, layer.TileAt({0_row, 2_col}));
    EXPECT_EQ(empty, layer.TileAt({0_row, 3_col}));
    EXPECT_EQ(empty, layer.TileAt({0_row, 4_col}));

    EXPECT_EQ(empty, layer.TileAt({1_row, 2_col}));
    EXPECT_EQ(empty, layer.TileAt({1_row, 3_col}));
    EXPECT_EQ(empty, layer.TileAt({1_row, 4_col}));

    EXPECT_EQ(empty, layer.TileAt({2_row, 3_col}));
    EXPECT_EQ(empty, layer.TileAt({2_row, 4_col}));

    EXPECT_EQ(empty, layer.TileAt({3_row, 4_col}));
  }
}

TEST(FloodFill, OutOfBounds)
{
  core::TileLayer layer;

  std::vector<core::Position> positions;
  EXPECT_NO_THROW(layer.Flood({5_row, 5_col}, empty, positions));

  positions.clear();
  EXPECT_NO_THROW(layer.Flood({6_row, 6_col}, empty, positions));

  positions.clear();
  EXPECT_NO_THROW(core::FloodFill(layer, {6_row, 6_col}, empty, positions));
}
