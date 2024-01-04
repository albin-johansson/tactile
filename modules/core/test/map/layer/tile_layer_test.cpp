// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/map/layer/tile_layer.hpp"

#include <algorithm>  // count

#include <gtest/gtest.h>

#include "tactile/foundation/debug/exception.hpp"
#include "tactile/foundation/misc/integer_conversion.hpp"

using namespace tactile;

using int_literals::operator""_uz;
using int_literals::operator""_z;

/// \tests tactile::TileLayer::TileLayer
TEST(TileLayer, Constructor)
{
  const TileLayer layer {3_z, 4_z};
  EXPECT_EQ(layer.row_count(), 3_z);
  EXPECT_EQ(layer.column_count(), 4_z);

  for (auto row = 0_z; row < 3_z; ++row) {
    for (auto col = 0_z; col < 4_z; ++col) {
      EXPECT_EQ(layer.get_tile(TilePos {row, col}), kEmptyTile);
    }
  }
}

/// \tests tactile::TileLayer::resize
TEST(TileLayer, Resize)
{
  TileLayer layer {5_z, 7_z};

  // Remove row.
  layer.resize(4_z, 7_z);
  EXPECT_EQ(layer.row_count(), 4_z);
  EXPECT_EQ(layer.column_count(), 7_z);

  // Remove column.
  layer.resize(4_z, 6_z);
  EXPECT_EQ(layer.row_count(), 4_z);
  EXPECT_EQ(layer.column_count(), 6_z);

  // Add rows.
  layer.resize(10_z, 6_z);
  EXPECT_EQ(layer.row_count(), 10_z);
  EXPECT_EQ(layer.column_count(), 6_z);

  // Add columns.
  layer.resize(10_z, 8_z);
  EXPECT_EQ(layer.row_count(), 10_z);
  EXPECT_EQ(layer.column_count(), 8_z);

  // Minimum size.
  layer.resize(1_z, 1_z);
  EXPECT_EQ(layer.row_count(), 1_z);
  EXPECT_EQ(layer.column_count(), 1_z);

  EXPECT_THROW(layer.resize(0_z, 1_z), Exception);
  EXPECT_THROW(layer.resize(1_z, 0_z), Exception);
  EXPECT_THROW(layer.resize(-1_z, 1_z), Exception);
  EXPECT_THROW(layer.resize(1_z, -1_z), Exception);
}

/// \tests tactile::TileLayer::flood
TEST(TileLayer, Flood)
{
  TileLayer layer {5_z, 5_z};

  for (auto row = 0_z; row < layer.row_count(); ++row) {
    for (auto col = 0_z; col < layer.column_count(); ++col) {
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

  for (auto row = 0_z; row < layer.row_count(); ++row) {
    for (auto col = 0_z; col < layer.column_count(); ++col) {
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
  TileLayer layer {4_z, 4_z};

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

  EXPECT_EQ(std::ssize(affected_positions), layer.row_count() * layer.column_count());

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
  TileLayer layer {5_z, 5_z};

  const TilePos tile_pos {2_z, 2_z};
  const TileID tile_id {42};

  layer.set_tile(tile_pos, tile_id);
  EXPECT_EQ(layer.get_tile(tile_pos), tile_id);
}

/// \tests tactile::TileLayer::get_tile
TEST(TileLayer, GetTile)
{
  const TileLayer layer {10_z, 8_z};

  EXPECT_EQ(layer.get_tile(TilePos {0_z, 0_z}), kEmptyTile);
  EXPECT_FALSE(
      layer.get_tile(TilePos {layer.row_count(), layer.column_count()}).has_value());
}

/// \tests tactile::TileLayer::is_valid_pos
TEST(TileLayer, IsValidPos)
{
  const TileLayer layer {10_z, 8_z};

  const auto row_count = layer.row_count();
  const auto col_count = layer.column_count();

  EXPECT_TRUE(layer.is_valid_pos(TilePos {0_z, 0_z}));
  EXPECT_TRUE(layer.is_valid_pos(TilePos {row_count - 1_z, col_count - 1_z}));

  EXPECT_FALSE(layer.is_valid_pos(TilePos {row_count, 0_z}));
  EXPECT_FALSE(layer.is_valid_pos(TilePos {0_z, col_count}));
  EXPECT_FALSE(layer.is_valid_pos(TilePos {row_count, col_count}));

  EXPECT_FALSE(layer.is_valid_pos(TilePos {-1_z, -1_z}));
  EXPECT_FALSE(layer.is_valid_pos(TilePos {3_z, col_count + 832_z}));
}
