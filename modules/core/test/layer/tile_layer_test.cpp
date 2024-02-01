// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/tile_layer.hpp"

#include <algorithm>  // count

#include <gtest/gtest.h>

#include "tactile/core/layer/dense_tile_layer.hpp"
#include "tactile/core/layer/sparse_tile_layer.hpp"
#include "tactile/foundation/debug/exception.hpp"
#include "tactile/foundation/misc/integer_conversion.hpp"

namespace {

using namespace tactile;
using namespace tactile::core;

using int_literals::operator""_z;

using TileLayerTypes = testing::Types<DenseTileLayer, SparseTileLayer>;

template <typename T>
class TileLayerTest : public testing::Test {};

TYPED_TEST_SUITE(TileLayerTest, TileLayerTypes);

// NOLINTBEGIN(*-use-trailing-return-type)
// NOLINTBEGIN(*-function-cognitive-complexity)

/// \tests tactile::TileLayer::TileLayer
TYPED_TEST(TileLayerTest, Constructor)
{
  const MatrixExtent extent {3, 4};
  const TypeParam layer {extent};

  EXPECT_EQ(layer.extent(), extent);

  for (auto row = 0_z; row < extent.row_count; ++row) {
    for (auto col = 0_z; col < extent.col_count; ++col) {
      EXPECT_EQ(layer.tile_at(TilePos {row, col}), kEmptyTile);
    }
  }
}

/// \tests tactile::TileLayer::set_extent
TYPED_TEST(TileLayerTest, SetExtent)
{
  TypeParam layer {MatrixExtent {5, 7}};

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
TYPED_TEST(TileLayerTest, SetTile)
{
  const MatrixExtent extent {5, 5};
  TypeParam layer {extent};

  const TilePos tile_pos {2, 2};
  const TileID tile_id {42};

  layer.set_tile(tile_pos, tile_id);
  EXPECT_EQ(layer.tile_at(tile_pos), tile_id);
}

/// \tests tactile::TileLayer::tile_at
TYPED_TEST(TileLayerTest, GetTile)
{
  const MatrixExtent extent {10, 8};
  const TypeParam layer {extent};

  EXPECT_EQ(layer.tile_at(TilePos {0, 0}), kEmptyTile);
  EXPECT_FALSE(layer.tile_at(TilePos {extent.row_count, extent.col_count}).has_value());
}

/// \tests tactile::TileLayer::is_valid_pos
TYPED_TEST(TileLayerTest, IsValidPos)
{
  const MatrixExtent extent {10, 8};
  const TypeParam layer {extent};

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

}  // namespace
