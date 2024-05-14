// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include <gtest/gtest.h>

#include "tactile/core/debug/exception.hpp"
#include "tactile/core/layer/dense_tile_matrix.hpp"
#include "tactile/core/layer/sparse_tile_matrix.hpp"

namespace tactile {
namespace {

template <typename MatrixType>
void _validate_tiles(const MatrixType& tile_matrix,
                     const MatrixExtent& expected_extent)
{
  EXPECT_EQ(tile_matrix.get_extent(), expected_extent);

  for (MatrixExtent::value_type row = 0; row < expected_extent.rows; ++row) {
    for (MatrixExtent::value_type col = 0; col < expected_extent.cols; ++col) {
      const MatrixIndex index {row, col};
      EXPECT_TRUE(tile_matrix.is_valid(index)) << "index is " << index;
    }
  }
}

}  // namespace

template <typename MatrixType>
class TileMatrixTest : public testing::Test
{};

using TileMatrixTypes = testing::Types<DenseTileMatrix, SparseTileMatrix>;

TYPED_TEST_SUITE(TileMatrixTest, TileMatrixTypes);

/// \trace tactile::DenseTileMatrix::DenseTileMatrix
/// \trace tactile::SparseTileMatrix::SparseTileMatrix
TYPED_TEST(TileMatrixTest, DefaultConstructor)
{
  const TypeParam tile_matrix {};
  _validate_tiles(tile_matrix, MatrixExtent {0, 0});
}

/// \trace tactile::DenseTileMatrix::DenseTileMatrix [MatrixExtent]
/// \trace tactile::SparseTileMatrix::SparseTileMatrix [MatrixExtent]
TYPED_TEST(TileMatrixTest, ExtentConstructor)
{
  const TypeParam tile_matrix {MatrixExtent {12, 24}};
  _validate_tiles(tile_matrix, MatrixExtent {12, 24});
}

/// \trace tactile::DenseTileMatrix::resize
/// \trace tactile::SparseTileMatrix::resize
TYPED_TEST(TileMatrixTest, Resize)
{
  TypeParam tile_matrix {};

  const auto resize_and_validate = [&](const MatrixExtent& extent) {
    tile_matrix.resize(extent);
    _validate_tiles(tile_matrix, extent);
  };

  // Increase row count.
  resize_and_validate(MatrixExtent {20, 0});

  // Increase column count.
  resize_and_validate(MatrixExtent {20, 12});

  // Decrease row count.
  resize_and_validate(MatrixExtent {15, 12});

  // Decrease column count.
  resize_and_validate(MatrixExtent {15, 10});

  // Increase both row and column count.
  resize_and_validate(MatrixExtent {27, 19});

  // Decrease both row and column count.
  resize_and_validate(MatrixExtent {25, 15});

  // Resize to zero.
  resize_and_validate(MatrixExtent {0, 0});

  // Increase both row and column count.
  resize_and_validate(MatrixExtent {20, 20});

  // Increment row count.
  resize_and_validate(MatrixExtent {21, 20});

  // Increment column count.
  resize_and_validate(MatrixExtent {21, 21});

  // Increase row count and decrease column count.
  resize_and_validate(MatrixExtent {30, 10});

  // Decrease row count and increase column count.
  resize_and_validate(MatrixExtent {18, 31});
}

/// \trace tactile::DenseTileMatrix::at
/// \trace tactile::SparseTileMatrix::at
TYPED_TEST(TileMatrixTest, At)
{
  TypeParam tile_matrix {MatrixExtent {2, 2}};
  tile_matrix[{0, 0}] = TileID {42};
  tile_matrix[{0, 1}] = TileID {99};
  tile_matrix[{1, 1}] = TileID {123};

  const auto& const_tile_matrix = tile_matrix;

  EXPECT_EQ(tile_matrix.at({0, 0}), TileID {42});
  EXPECT_EQ(tile_matrix.at({0, 1}), TileID {99});
  EXPECT_EQ(tile_matrix.at({1, 0}), kEmptyTile);
  EXPECT_EQ(tile_matrix.at({1, 1}), TileID {123});

  EXPECT_EQ(const_tile_matrix.at({0, 0}), TileID {42});
  EXPECT_EQ(const_tile_matrix.at({0, 1}), TileID {99});
  EXPECT_EQ(const_tile_matrix.at({1, 0}), kEmptyTile);
  EXPECT_EQ(const_tile_matrix.at({1, 1}), TileID {123});

  EXPECT_THROW((void) tile_matrix.at({2, 1}), Exception);
  EXPECT_THROW((void) const_tile_matrix.at({1, 2}), Exception);
}

/// \trace tactile::DenseTileMatrix::operator[]
/// \trace tactile::SparseTileMatrix::operator[]
TYPED_TEST(TileMatrixTest, SubscriptOperator)
{
  TypeParam tile_matrix {MatrixExtent {3, 4}};

  const auto assign_and_check = [&](const MatrixIndex& index,
                                    const TileID tile_id) {
    tile_matrix[index] = tile_id;
    EXPECT_EQ(tile_matrix[index], tile_id) << "index is " << index;
  };

  assign_and_check({0, 0}, TileID {0});
  assign_and_check({0, 1}, TileID {1});
  assign_and_check({0, 2}, TileID {2});
  assign_and_check({0, 3}, TileID {3});

  assign_and_check({1, 0}, TileID {4});
  assign_and_check({1, 1}, TileID {5});
  assign_and_check({1, 2}, TileID {6});
  assign_and_check({1, 3}, TileID {7});

  assign_and_check({2, 0}, TileID {8});
  assign_and_check({2, 1}, TileID {9});
  assign_and_check({2, 2}, TileID {10});
  assign_and_check({2, 3}, TileID {11});
}

/// \trace tactile::DenseTileMatrix::is_valid
/// \trace tactile::SparseTileMatrix::is_valid
TYPED_TEST(TileMatrixTest, IsValid)
{
  const MatrixExtent extent {6, 5};
  const TypeParam tile_matrix {extent};

  _validate_tiles(tile_matrix, extent);

  EXPECT_TRUE(tile_matrix.is_valid({5, 4}));
  EXPECT_FALSE(tile_matrix.is_valid({6, 4}));
  EXPECT_FALSE(tile_matrix.is_valid({5, 5}));
  EXPECT_FALSE(tile_matrix.is_valid({6, 5}));
}

}  // namespace tactile
