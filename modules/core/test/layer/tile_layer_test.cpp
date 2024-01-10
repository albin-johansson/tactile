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

/// \tests tactile::TileLayer::flood
TEST(TileLayer, Flood)
{
  const MatrixExtent extent {5, 5};
  TileLayer layer {extent};

  for (auto row = 0_z; row < extent.row_count; ++row) {
    for (auto col = 0_z; col < extent.col_count; ++col) {
      EXPECT_EQ(layer.get_tile(TilePos {row, col}), kEmptyTile);
    }
  }

  //     0   1   2   3   4            0   1   2   3   4
  //   ┌───┬───┬───┬───┬───┐        ┌───┬───┬───┬───┬───┐
  // 0 │ 0 │ 0 │ 0 │ 0 │ 0 │      0 │ 1 │ 1 │ 1 │ 1 │ 1 │
  //   ├───┼───┼───┼───┼───┤        ├───┼───┼───┼───┼───┤
  // 1 │ 0 │ 0 │ 0 │ 0 │ 0 │      1 │ 1 │ 1 │ 1 │ 1 │ 1 │
  //   ├───┼───┼───┼───┼───┤        ├───┼───┼───┼───┼───┤
  // 2 │ 0 │ 0 │ 0 │ 0 │ 0 │  ->  2 │ 1 │ 1 │ 1 │ 1 │ 1 │
  //   ├───┼───┼───┼───┼───┤        ├───┼───┼───┼───┼───┤
  // 3 │ 0 │ 0 │ 0 │ 0 │ 0 │      3 │ 1 │ 1 │ 1 │ 1 │ 1 │
  //   ├───┼───┼───┼───┼───┤        ├───┼───┼───┼───┼───┤
  // 4 │ 0 │ 0 │ 0 │ 0 │ 0 │      4 │ 1 │ 1 │ 1 │ 1 │ 1 │
  //   └───┴───┴───┴───┴───┘        └───┴───┴───┴───┴───┘
  layer.flood(TilePos {0, 0}, TileID {1});

  for (auto row = 0_z; row < extent.row_count; ++row) {
    for (auto col = 0_z; col < extent.col_count; ++col) {
      EXPECT_EQ(layer.get_tile(TilePos {row, col}), TileID {1});
    }
  }

  //     0   1   2   3   4            0   1   2   3   4
  //   ┌───┬───┬───┬───┬───┐        ┌───┬───┬───┬───┬───┐
  // 0 │ 1 │ 1 │ 1 │ 1 │ 1 │      0 │ 2 │ 2 │ 1 │ 3 │ 3 │
  //   ├───┼───┼───┼───┼───┤        ├───┼───┼───┼───┼───┤
  // 1 │ 1 │ 1 │ 1 │ 1 │ 1 │      1 │ 2 │ 2 │ 1 │ 3 │ 3 │
  //   ├───┼───┼───┼───┼───┤        ├───┼───┼───┼───┼───┤
  // 2 │ 1 │ 1 │ 1 │ 1 │ 1 │  ->  2 │ 1 │ 1 │ 1 │ 1 │ 1 │
  //   ├───┼───┼───┼───┼───┤        ├───┼───┼───┼───┼───┤
  // 3 │ 1 │ 1 │ 1 │ 1 │ 1 │      3 │ 4 │ 4 │ 1 │ 5 │ 5 │
  //   ├───┼───┼───┼───┼───┤        ├───┼───┼───┼───┼───┤
  // 4 │ 1 │ 1 │ 1 │ 1 │ 1 │      4 │ 4 │ 4 │ 1 │ 5 │ 5 │
  //   └───┴───┴───┴───┴───┘        └───┴───┴───┴───┴───┘

  layer.set_tile(TilePos {0, 0}, TileID {2});
  layer.set_tile(TilePos {0, 1}, TileID {2});
  layer.set_tile(TilePos {1, 0}, TileID {2});
  layer.set_tile(TilePos {1, 1}, TileID {2});

  layer.set_tile(TilePos {0, 3}, TileID {3});
  layer.set_tile(TilePos {0, 4}, TileID {3});
  layer.set_tile(TilePos {1, 3}, TileID {3});
  layer.set_tile(TilePos {1, 4}, TileID {3});

  layer.set_tile(TilePos {3, 0}, TileID {4});
  layer.set_tile(TilePos {3, 1}, TileID {4});
  layer.set_tile(TilePos {4, 0}, TileID {4});
  layer.set_tile(TilePos {4, 1}, TileID {4});

  layer.set_tile(TilePos {3, 3}, TileID {5});
  layer.set_tile(TilePos {3, 4}, TileID {5});
  layer.set_tile(TilePos {4, 3}, TileID {5});
  layer.set_tile(TilePos {4, 4}, TileID {5});

  //     0   1   2   3   4            0   1   2   3   4
  //   ┌───┬───┬───┬───┬───┐        ┌───┬───┬───┬───┬───┐
  // 0 │ 2 │ 2 │ 1 │ 3 │ 3 │      0 │ 8 │ 8 │ 1 │ 3 │ 3 │
  //   ├───┼───┼───┼───┼───┤        ├───┼───┼───┼───┼───┤
  // 1 │ 2 │ 2 │ 1 │ 3 │ 3 │      1 │ 8 │ 8 │ 1 │ 3 │ 3 │
  //   ├───┼───┼───┼───┼───┤        ├───┼───┼───┼───┼───┤
  // 2 │ 1 │ 1 │ 1 │ 1 │ 1 │  ->  2 │ 1 │ 1 │ 1 │ 1 │ 1 │
  //   ├───┼───┼───┼───┼───┤        ├───┼───┼───┼───┼───┤
  // 3 │ 4 │ 4 │ 1 │ 5 │ 5 │      3 │ 4 │ 4 │ 1 │ 5 │ 5 │
  //   ├───┼───┼───┼───┼───┤        ├───┼───┼───┼───┼───┤
  // 4 │ 4 │ 4 │ 1 │ 5 │ 5 │      4 │ 4 │ 4 │ 1 │ 5 │ 5 │
  //   └───┴───┴───┴───┴───┘        └───┴───┴───┴───┴───┘
  layer.flood(TilePos {0, 0}, TileID {8});

  EXPECT_EQ(layer.get_tile(TilePos {0, 2}), TileID {1});
  EXPECT_EQ(layer.get_tile(TilePos {1, 2}), TileID {1});
  EXPECT_EQ(layer.get_tile(TilePos {2, 0}), TileID {1});
  EXPECT_EQ(layer.get_tile(TilePos {2, 1}), TileID {1});
  EXPECT_EQ(layer.get_tile(TilePos {2, 2}), TileID {1});
  EXPECT_EQ(layer.get_tile(TilePos {2, 3}), TileID {1});
  EXPECT_EQ(layer.get_tile(TilePos {2, 4}), TileID {1});
  EXPECT_EQ(layer.get_tile(TilePos {3, 2}), TileID {1});
  EXPECT_EQ(layer.get_tile(TilePos {4, 2}), TileID {1});

  EXPECT_EQ(layer.get_tile(TilePos {0, 0}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {0, 1}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {1, 0}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {1, 1}), TileID {8});

  EXPECT_EQ(layer.get_tile(TilePos {0, 3}), TileID {3});
  EXPECT_EQ(layer.get_tile(TilePos {0, 4}), TileID {3});
  EXPECT_EQ(layer.get_tile(TilePos {1, 3}), TileID {3});
  EXPECT_EQ(layer.get_tile(TilePos {1, 4}), TileID {3});

  EXPECT_EQ(layer.get_tile(TilePos {3, 0}), TileID {4});
  EXPECT_EQ(layer.get_tile(TilePos {3, 1}), TileID {4});
  EXPECT_EQ(layer.get_tile(TilePos {4, 0}), TileID {4});
  EXPECT_EQ(layer.get_tile(TilePos {4, 1}), TileID {4});

  EXPECT_EQ(layer.get_tile(TilePos {3, 3}), TileID {5});
  EXPECT_EQ(layer.get_tile(TilePos {3, 4}), TileID {5});
  EXPECT_EQ(layer.get_tile(TilePos {4, 3}), TileID {5});
  EXPECT_EQ(layer.get_tile(TilePos {4, 4}), TileID {5});

  //     0   1   2   3   4            0   1   2   3   4
  //   ┌───┬───┬───┬───┬───┐        ┌───┬───┬───┬───┬───┐
  // 0 │ 8 │ 8 │ 1 │ 3 │ 3 │      0 │ 8 │ 8 │ 7 │ 3 │ 3 │
  //   ├───┼───┼───┼───┼───┤        ├───┼───┼───┼───┼───┤
  // 1 │ 8 │ 8 │ 1 │ 3 │ 3 │      1 │ 8 │ 8 │ 7 │ 3 │ 3 │
  //   ├───┼───┼───┼───┼───┤        ├───┼───┼───┼───┼───┤
  // 2 │ 1 │ 1 │ 1 │ 1 │ 1 │  ->  2 │ 7 │ 7 │ 7 │ 7 │ 7 │
  //   ├───┼───┼───┼───┼───┤        ├───┼───┼───┼───┼───┤
  // 3 │ 4 │ 4 │ 1 │ 5 │ 5 │      3 │ 4 │ 4 │ 7 │ 5 │ 5 │
  //   ├───┼───┼───┼───┼───┤        ├───┼───┼───┼───┼───┤
  // 4 │ 4 │ 4 │ 1 │ 5 │ 5 │      4 │ 4 │ 4 │ 7 │ 5 │ 5 │
  //   └───┴───┴───┴───┴───┘        └───┴───┴───┴───┴───┘
  layer.flood(TilePos {2, 2}, TileID {7});

  EXPECT_EQ(layer.get_tile(TilePos {0, 2}), TileID {7});
  EXPECT_EQ(layer.get_tile(TilePos {1, 2}), TileID {7});
  EXPECT_EQ(layer.get_tile(TilePos {2, 0}), TileID {7});
  EXPECT_EQ(layer.get_tile(TilePos {2, 1}), TileID {7});
  EXPECT_EQ(layer.get_tile(TilePos {2, 2}), TileID {7});
  EXPECT_EQ(layer.get_tile(TilePos {2, 3}), TileID {7});
  EXPECT_EQ(layer.get_tile(TilePos {2, 4}), TileID {7});
  EXPECT_EQ(layer.get_tile(TilePos {3, 2}), TileID {7});
  EXPECT_EQ(layer.get_tile(TilePos {4, 2}), TileID {7});

  EXPECT_EQ(layer.get_tile(TilePos {0, 0}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {0, 1}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {1, 0}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {1, 1}), TileID {8});

  EXPECT_EQ(layer.get_tile(TilePos {0, 3}), TileID {3});
  EXPECT_EQ(layer.get_tile(TilePos {0, 4}), TileID {3});
  EXPECT_EQ(layer.get_tile(TilePos {1, 3}), TileID {3});
  EXPECT_EQ(layer.get_tile(TilePos {1, 4}), TileID {3});

  EXPECT_EQ(layer.get_tile(TilePos {3, 0}), TileID {4});
  EXPECT_EQ(layer.get_tile(TilePos {3, 1}), TileID {4});
  EXPECT_EQ(layer.get_tile(TilePos {4, 0}), TileID {4});
  EXPECT_EQ(layer.get_tile(TilePos {4, 1}), TileID {4});

  EXPECT_EQ(layer.get_tile(TilePos {3, 3}), TileID {5});
  EXPECT_EQ(layer.get_tile(TilePos {3, 4}), TileID {5});
  EXPECT_EQ(layer.get_tile(TilePos {4, 3}), TileID {5});
  EXPECT_EQ(layer.get_tile(TilePos {4, 4}), TileID {5});

  //     0   1   2   3   4            0   1   2   3   4
  //   ┌───┬───┬───┬───┬───┐        ┌───┬───┬───┬───┬───┐
  // 0 │ 8 │ 8 │ 7 │ 3 │ 3 │      0 │ 8 │ 8 │ 8 │ 3 │ 3 │
  //   ├───┼───┼───┼───┼───┤        ├───┼───┼───┼───┼───┤
  // 1 │ 8 │ 8 │ 7 │ 3 │ 3 │      1 │ 8 │ 8 │ 8 │ 3 │ 3 │
  //   ├───┼───┼───┼───┼───┤        ├───┼───┼───┼───┼───┤
  // 2 │ 7 │ 7 │ 7 │ 7 │ 7 │  ->  2 │ 8 │ 8 │ 8 │ 8 │ 8 │
  //   ├───┼───┼───┼───┼───┤        ├───┼───┼───┼───┼───┤
  // 3 │ 4 │ 4 │ 7 │ 5 │ 5 │      3 │ 4 │ 4 │ 8 │ 5 │ 5 │
  //   ├───┼───┼───┼───┼───┤        ├───┼───┼───┼───┼───┤
  // 4 │ 4 │ 4 │ 7 │ 5 │ 5 │      4 │ 4 │ 4 │ 8 │ 5 │ 5 │
  //   └───┴───┴───┴───┴───┘        └───┴───┴───┴───┴───┘
  layer.flood(TilePos {0, 2}, TileID {8});

  EXPECT_EQ(layer.get_tile(TilePos {0, 2}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {1, 2}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {2, 0}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {2, 1}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {2, 2}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {2, 3}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {2, 4}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {3, 2}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {4, 2}), TileID {8});

  EXPECT_EQ(layer.get_tile(TilePos {0, 0}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {0, 1}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {1, 0}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {1, 1}), TileID {8});

  EXPECT_EQ(layer.get_tile(TilePos {0, 3}), TileID {3});
  EXPECT_EQ(layer.get_tile(TilePos {0, 4}), TileID {3});
  EXPECT_EQ(layer.get_tile(TilePos {1, 3}), TileID {3});
  EXPECT_EQ(layer.get_tile(TilePos {1, 4}), TileID {3});

  EXPECT_EQ(layer.get_tile(TilePos {3, 0}), TileID {4});
  EXPECT_EQ(layer.get_tile(TilePos {3, 1}), TileID {4});
  EXPECT_EQ(layer.get_tile(TilePos {4, 0}), TileID {4});
  EXPECT_EQ(layer.get_tile(TilePos {4, 1}), TileID {4});

  EXPECT_EQ(layer.get_tile(TilePos {3, 3}), TileID {5});
  EXPECT_EQ(layer.get_tile(TilePos {3, 4}), TileID {5});
  EXPECT_EQ(layer.get_tile(TilePos {4, 3}), TileID {5});
  EXPECT_EQ(layer.get_tile(TilePos {4, 4}), TileID {5});

  // This shouldn't change anything from the previous state.
  layer.flood(TilePos {2, 2}, TileID {8});

  EXPECT_EQ(layer.get_tile(TilePos {0, 2}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {1, 2}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {2, 0}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {2, 1}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {2, 2}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {2, 3}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {2, 4}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {3, 2}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {4, 2}), TileID {8});

  EXPECT_EQ(layer.get_tile(TilePos {0, 0}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {0, 1}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {1, 0}), TileID {8});
  EXPECT_EQ(layer.get_tile(TilePos {1, 1}), TileID {8});

  EXPECT_EQ(layer.get_tile(TilePos {0, 3}), TileID {3});
  EXPECT_EQ(layer.get_tile(TilePos {0, 4}), TileID {3});
  EXPECT_EQ(layer.get_tile(TilePos {1, 3}), TileID {3});
  EXPECT_EQ(layer.get_tile(TilePos {1, 4}), TileID {3});

  EXPECT_EQ(layer.get_tile(TilePos {3, 0}), TileID {4});
  EXPECT_EQ(layer.get_tile(TilePos {3, 1}), TileID {4});
  EXPECT_EQ(layer.get_tile(TilePos {4, 0}), TileID {4});
  EXPECT_EQ(layer.get_tile(TilePos {4, 1}), TileID {4});

  EXPECT_EQ(layer.get_tile(TilePos {3, 3}), TileID {5});
  EXPECT_EQ(layer.get_tile(TilePos {3, 4}), TileID {5});
  EXPECT_EQ(layer.get_tile(TilePos {4, 3}), TileID {5});
  EXPECT_EQ(layer.get_tile(TilePos {4, 4}), TileID {5});
}

/// \tests tactile::TileLayer::flood
TEST(TileLayer, FloodWithAffectedPositions)
{
  const MatrixExtent extent {4, 4};
  TileLayer layer {extent};

  //     0   1   2   3            0   1   2   3
  //   ┌───┬───┬───┬───┐        ┌───┬───┬───┬───┐
  // 0 │ 0 │ 0 │ 0 │ 0 │      0 │ 5 │ 5 │ 5 │ 5 │
  //   ├───┼───┼───┼───┤        ├───┼───┼───┼───┤
  // 1 │ 0 │ 0 │ 0 │ 0 │      1 │ 5 │ 5 │ 5 │ 5 │
  //   ├───┼───┼───┼───┤        ├───┼───┼───┼───┤
  // 2 │ 0 │ 0 │ 0 │ 0 │  ->  2 │ 5 │ 5 │ 5 │ 5 │
  //   ├───┼───┼───┼───┤        ├───┼───┼───┼───┤
  // 3 │ 0 │ 0 │ 0 │ 0 │      3 │ 5 │ 5 │ 5 │ 5 │
  //   └───┴───┴───┴───┘        └───┴───┴───┴───┘
  Vector<TilePos> affected_positions {};
  layer.flood(TilePos {3, 3}, TileID {5}, &affected_positions);

  EXPECT_EQ(std::ssize(affected_positions), extent.row_count * extent.col_count);

  // clang-format off
  EXPECT_EQ(std::count(affected_positions.begin(), affected_positions.end(), TilePos {0, 0}), 1_uz);
  EXPECT_EQ(std::count(affected_positions.begin(), affected_positions.end(), TilePos {0, 1}), 1_uz);
  EXPECT_EQ(std::count(affected_positions.begin(), affected_positions.end(), TilePos {0, 2}), 1_uz);
  EXPECT_EQ(std::count(affected_positions.begin(), affected_positions.end(), TilePos {0, 3}), 1_uz);

  EXPECT_EQ(std::count(affected_positions.begin(), affected_positions.end(), TilePos {1, 0}), 1_uz);
  EXPECT_EQ(std::count(affected_positions.begin(), affected_positions.end(), TilePos {1, 1}), 1_uz);
  EXPECT_EQ(std::count(affected_positions.begin(), affected_positions.end(), TilePos {1, 2}), 1_uz);
  EXPECT_EQ(std::count(affected_positions.begin(), affected_positions.end(), TilePos {1, 3}), 1_uz);

  EXPECT_EQ(std::count(affected_positions.begin(), affected_positions.end(), TilePos {2, 0}), 1_uz);
  EXPECT_EQ(std::count(affected_positions.begin(), affected_positions.end(), TilePos {2, 1}), 1_uz);
  EXPECT_EQ(std::count(affected_positions.begin(), affected_positions.end(), TilePos {2, 2}), 1_uz);
  EXPECT_EQ(std::count(affected_positions.begin(), affected_positions.end(), TilePos {2, 3}), 1_uz);

  EXPECT_EQ(std::count(affected_positions.begin(), affected_positions.end(), TilePos {3, 0}), 1_uz);
  EXPECT_EQ(std::count(affected_positions.begin(), affected_positions.end(), TilePos {3, 1}), 1_uz);
  EXPECT_EQ(std::count(affected_positions.begin(), affected_positions.end(), TilePos {3, 2}), 1_uz);
  EXPECT_EQ(std::count(affected_positions.begin(), affected_positions.end(), TilePos {3, 3}), 1_uz);
  // clang-format on
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
