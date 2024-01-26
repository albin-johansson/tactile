// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/tile_layer.hpp"

#include <algorithm>  // count

#include <gtest/gtest.h>

#include "tactile/foundation/debug/exception.hpp"
#include "tactile/foundation/misc/integer_conversion.hpp"

using namespace tactile;

using int_literals::operator""_uz;
using int_literals::operator""_z;

// NOLINTBEGIN(*-use-trailing-return-type)
// NOLINTBEGIN(*-function-cognitive-complexity)

/// \tests tactile::TileLayer::TileLayer
TEST(TileLayer, Constructor)
{
  const MatrixExtent extent {3, 4};
  const TileLayer layer {extent};

  EXPECT_EQ(layer.extent(), extent);

  for (auto row = 0_z; row < extent.row_count; ++row) {
    for (auto col = 0_z; col < extent.col_count; ++col) {
      EXPECT_EQ(layer.get_tile(TilePos {row, col}), kEmptyTile);
    }
  }
}

/// \tests tactile::TileLayer::set_extent
TEST(TileLayer, SetExtent)
{
  TileLayer layer {MatrixExtent {5, 7}};

  // Remove a row.
  EXPECT_EQ(layer.set_extent(MatrixExtent {4, 7}), kOK);
  EXPECT_EQ(layer.extent().row_count, 4);
  EXPECT_EQ(layer.extent().col_count, 7);

  // Remove a column.
  EXPECT_EQ(layer.set_extent(MatrixExtent {4, 6}), kOK);
  EXPECT_EQ(layer.extent().row_count, 4);
  EXPECT_EQ(layer.extent().col_count, 6);

  // Add several rows.
  EXPECT_EQ(layer.set_extent(MatrixExtent {10, 6}), kOK);
  EXPECT_EQ(layer.extent().row_count, 10);
  EXPECT_EQ(layer.extent().col_count, 6);

  // Add several columns.
  EXPECT_EQ(layer.set_extent(MatrixExtent {10, 8}), kOK);
  EXPECT_EQ(layer.extent().row_count, 10);
  EXPECT_EQ(layer.extent().col_count, 8);

  // Use minimum extent.
  EXPECT_EQ(layer.set_extent(MatrixExtent {1, 1}), kOK);
  EXPECT_EQ(layer.extent().row_count, 1);
  EXPECT_EQ(layer.extent().col_count, 1);

  // Invalid extents.
  EXPECT_NE(layer.set_extent(MatrixExtent {0, 1}), kOK);
  EXPECT_NE(layer.set_extent(MatrixExtent {1, 0}), kOK);
  EXPECT_NE(layer.set_extent(MatrixExtent {-1, 1}), kOK);
  EXPECT_NE(layer.set_extent(MatrixExtent {1, -1}), kOK);
}

/// \tests tactile::TileLayer::set_tile
TEST(TileLayer, SetTile)
{
  const MatrixExtent extent {5, 5};
  TileLayer layer {extent};

  const TilePos tile_pos {2, 2};
  const TileID tile_id {42};

  layer.set_tile(tile_pos, tile_id);
  EXPECT_EQ(layer.get_tile(tile_pos), tile_id);
}

/// \tests tactile::TileLayer::get_tile
TEST(TileLayer, GetTile)
{
  const MatrixExtent extent {10, 8};
  const TileLayer layer {extent};

  EXPECT_EQ(layer.get_tile(TilePos {0, 0}), kEmptyTile);
  EXPECT_FALSE(layer.get_tile(TilePos {extent.row_count, extent.col_count}).has_value());
}

/// \tests tactile::TileLayer::is_valid_pos
TEST(TileLayer, IsValidPos)
{
  const MatrixExtent extent {10, 8};
  const TileLayer layer {extent};

  EXPECT_TRUE(layer.is_valid_pos(TilePos {0, 0}));
  EXPECT_TRUE(layer.is_valid_pos(TilePos {extent.row_count - 1, extent.col_count - 1}));

  EXPECT_FALSE(layer.is_valid_pos(TilePos {extent.row_count, 0}));
  EXPECT_FALSE(layer.is_valid_pos(TilePos {0, extent.col_count}));
  EXPECT_FALSE(layer.is_valid_pos(TilePos {extent.row_count, extent.col_count}));

  EXPECT_FALSE(layer.is_valid_pos(TilePos {-1, -1}));
  EXPECT_FALSE(layer.is_valid_pos(TilePos {3, extent.col_count + 832}));
}

// NOLINTEND(*-function-cognitive-complexity)
// NOLINTEND(*-use-trailing-return-type)
