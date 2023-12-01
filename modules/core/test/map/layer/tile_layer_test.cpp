// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/map/layer/tile_layer.hpp"

#include <gtest/gtest.h>

#include "tactile/core/debug/error.hpp"
#include "tactile/core/misc/conversion.hpp"

using namespace tactile;
using namespace tactile::int_literals;

TEST(TileLayer, Constructor)
{
  const TileLayer layer {3_uz, 4_uz};
  EXPECT_EQ(layer.row_count(), 3_uz);
  EXPECT_EQ(layer.column_count(), 4_uz);

  for (auto row = 0_z; row < 3_z; ++row) {
    for (auto col = 0_z; col < 4_z; ++col) {
      EXPECT_EQ(layer.get_tile(TilePos {row, col}), kEmptyTile);
    }
  }
}

TEST(TileLayer, Resize)
{
  TileLayer layer {5_uz, 7_uz};

  // Remove row.
  layer.resize(4_uz, 7_uz);
  EXPECT_EQ(layer.row_count(), 4_uz);
  EXPECT_EQ(layer.column_count(), 7_uz);

  // Remove column.
  layer.resize(4_uz, 6_uz);
  EXPECT_EQ(layer.row_count(), 4_uz);
  EXPECT_EQ(layer.column_count(), 6_uz);

  // Add rows.
  layer.resize(10_uz, 6_uz);
  EXPECT_EQ(layer.row_count(), 10_uz);
  EXPECT_EQ(layer.column_count(), 6_uz);

  // Add columns.
  layer.resize(10_uz, 8_uz);
  EXPECT_EQ(layer.row_count(), 10_uz);
  EXPECT_EQ(layer.column_count(), 8_uz);

  // Minimum size.
  layer.resize(1_uz, 1_uz);
  EXPECT_EQ(layer.row_count(), 1_uz);
  EXPECT_EQ(layer.column_count(), 1_uz);

  EXPECT_THROW(layer.resize(0_uz, 1_uz), Error);
  EXPECT_THROW(layer.resize(1_uz, 0_uz), Error);
}

TEST(TileLayer, Flood)
{
  // TODO
}

TEST(TileLayer, SetTile)
{
  TileLayer layer {5_uz, 5_uz};

  const TilePos tile_pos {2_z, 2_z};
  const TileID tile_id {42};

  layer.set_tile(tile_pos, tile_id);
  EXPECT_EQ(layer.get_tile(tile_pos), tile_id);
}

TEST(TileLayer, GetTile)
{
  const TileLayer layer {10_uz, 8_uz};

  const auto s_row_count = as_signed(layer.row_count());
  const auto s_col_count = as_signed(layer.column_count());

  EXPECT_EQ(layer.get_tile(TilePos {0_z, 0_z}), kEmptyTile);
  EXPECT_FALSE(layer.get_tile(TilePos {s_row_count, s_col_count}).has_value());
}

TEST(TileLayer, IsValidPos)
{
  const TileLayer layer {10_uz, 8_uz};

  const auto s_row_count = as_signed(layer.row_count());
  const auto s_col_count = as_signed(layer.column_count());

  EXPECT_TRUE(layer.is_valid_pos(TilePos {0_z, 0_z}));
  EXPECT_TRUE(layer.is_valid_pos(TilePos {s_row_count - 1_z, s_col_count - 1_z}));

  EXPECT_FALSE(layer.is_valid_pos(TilePos {s_row_count, 0_z}));
  EXPECT_FALSE(layer.is_valid_pos(TilePos {0_z, s_col_count}));
  EXPECT_FALSE(layer.is_valid_pos(TilePos {s_row_count, s_col_count}));

  EXPECT_FALSE(layer.is_valid_pos(TilePos {-1_z, -1_z}));
  EXPECT_FALSE(layer.is_valid_pos(TilePos {3_z, s_col_count + 832_z}));
}
