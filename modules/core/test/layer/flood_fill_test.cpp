// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/flood_fill.hpp"

#include <algorithm>  // count

#include <gtest/gtest.h>

#include "tactile/core/layer/dense_tile_layer.hpp"
#include "tactile/core/layer/sparse_tile_layer.hpp"

namespace {

using namespace tactile;

using int_literals::operator""_z;
using int_literals::operator""_uz;

using TileLayerTypes = testing::Types<DenseTileLayer, SparseTileLayer>;

template <typename T>
class FloodFillTest : public testing::Test {};

TYPED_TEST_SUITE(FloodFillTest, TileLayerTypes);

// NOLINTBEGIN(*-function-size, *-function-cognitive-complexity)

/// \tests tactile::flood_fill
TYPED_TEST(FloodFillTest, FloodFill)
{
  const MatrixExtent extent {5, 5};
  TypeParam layer {extent};

  for (auto row = 0_z; row < extent.row_count; ++row) {
    for (auto col = 0_z; col < extent.col_count; ++col) {
      EXPECT_EQ(layer.tile_at(TilePos {row, col}), kEmptyTile);
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
  flood_fill(layer, TilePos {0, 0}, TileID {1});

  for (auto row = 0_z; row < extent.row_count; ++row) {
    for (auto col = 0_z; col < extent.col_count; ++col) {
      EXPECT_EQ(layer.tile_at(TilePos {row, col}), TileID {1});
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

  EXPECT_TRUE(layer.set_tile(TilePos {0, 0}, TileID {2}));
  EXPECT_TRUE(layer.set_tile(TilePos {0, 1}, TileID {2}));
  EXPECT_TRUE(layer.set_tile(TilePos {1, 0}, TileID {2}));
  EXPECT_TRUE(layer.set_tile(TilePos {1, 1}, TileID {2}));

  EXPECT_TRUE(layer.set_tile(TilePos {0, 3}, TileID {3}));
  EXPECT_TRUE(layer.set_tile(TilePos {0, 4}, TileID {3}));
  EXPECT_TRUE(layer.set_tile(TilePos {1, 3}, TileID {3}));
  EXPECT_TRUE(layer.set_tile(TilePos {1, 4}, TileID {3}));

  EXPECT_TRUE(layer.set_tile(TilePos {3, 0}, TileID {4}));
  EXPECT_TRUE(layer.set_tile(TilePos {3, 1}, TileID {4}));
  EXPECT_TRUE(layer.set_tile(TilePos {4, 0}, TileID {4}));
  EXPECT_TRUE(layer.set_tile(TilePos {4, 1}, TileID {4}));

  EXPECT_TRUE(layer.set_tile(TilePos {3, 3}, TileID {5}));
  EXPECT_TRUE(layer.set_tile(TilePos {3, 4}, TileID {5}));
  EXPECT_TRUE(layer.set_tile(TilePos {4, 3}, TileID {5}));
  EXPECT_TRUE(layer.set_tile(TilePos {4, 4}, TileID {5}));

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
  flood_fill(layer, TilePos {0, 0}, TileID {8});

  EXPECT_EQ(layer.tile_at(TilePos {0, 2}), TileID {1});
  EXPECT_EQ(layer.tile_at(TilePos {1, 2}), TileID {1});
  EXPECT_EQ(layer.tile_at(TilePos {2, 0}), TileID {1});
  EXPECT_EQ(layer.tile_at(TilePos {2, 1}), TileID {1});
  EXPECT_EQ(layer.tile_at(TilePos {2, 2}), TileID {1});
  EXPECT_EQ(layer.tile_at(TilePos {2, 3}), TileID {1});
  EXPECT_EQ(layer.tile_at(TilePos {2, 4}), TileID {1});
  EXPECT_EQ(layer.tile_at(TilePos {3, 2}), TileID {1});
  EXPECT_EQ(layer.tile_at(TilePos {4, 2}), TileID {1});

  EXPECT_EQ(layer.tile_at(TilePos {0, 0}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {0, 1}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {1, 0}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {1, 1}), TileID {8});

  EXPECT_EQ(layer.tile_at(TilePos {0, 3}), TileID {3});
  EXPECT_EQ(layer.tile_at(TilePos {0, 4}), TileID {3});
  EXPECT_EQ(layer.tile_at(TilePos {1, 3}), TileID {3});
  EXPECT_EQ(layer.tile_at(TilePos {1, 4}), TileID {3});

  EXPECT_EQ(layer.tile_at(TilePos {3, 0}), TileID {4});
  EXPECT_EQ(layer.tile_at(TilePos {3, 1}), TileID {4});
  EXPECT_EQ(layer.tile_at(TilePos {4, 0}), TileID {4});
  EXPECT_EQ(layer.tile_at(TilePos {4, 1}), TileID {4});

  EXPECT_EQ(layer.tile_at(TilePos {3, 3}), TileID {5});
  EXPECT_EQ(layer.tile_at(TilePos {3, 4}), TileID {5});
  EXPECT_EQ(layer.tile_at(TilePos {4, 3}), TileID {5});
  EXPECT_EQ(layer.tile_at(TilePos {4, 4}), TileID {5});

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
  flood_fill(layer, TilePos {2, 2}, TileID {7});

  EXPECT_EQ(layer.tile_at(TilePos {0, 2}), TileID {7});
  EXPECT_EQ(layer.tile_at(TilePos {1, 2}), TileID {7});
  EXPECT_EQ(layer.tile_at(TilePos {2, 0}), TileID {7});
  EXPECT_EQ(layer.tile_at(TilePos {2, 1}), TileID {7});
  EXPECT_EQ(layer.tile_at(TilePos {2, 2}), TileID {7});
  EXPECT_EQ(layer.tile_at(TilePos {2, 3}), TileID {7});
  EXPECT_EQ(layer.tile_at(TilePos {2, 4}), TileID {7});
  EXPECT_EQ(layer.tile_at(TilePos {3, 2}), TileID {7});
  EXPECT_EQ(layer.tile_at(TilePos {4, 2}), TileID {7});

  EXPECT_EQ(layer.tile_at(TilePos {0, 0}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {0, 1}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {1, 0}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {1, 1}), TileID {8});

  EXPECT_EQ(layer.tile_at(TilePos {0, 3}), TileID {3});
  EXPECT_EQ(layer.tile_at(TilePos {0, 4}), TileID {3});
  EXPECT_EQ(layer.tile_at(TilePos {1, 3}), TileID {3});
  EXPECT_EQ(layer.tile_at(TilePos {1, 4}), TileID {3});

  EXPECT_EQ(layer.tile_at(TilePos {3, 0}), TileID {4});
  EXPECT_EQ(layer.tile_at(TilePos {3, 1}), TileID {4});
  EXPECT_EQ(layer.tile_at(TilePos {4, 0}), TileID {4});
  EXPECT_EQ(layer.tile_at(TilePos {4, 1}), TileID {4});

  EXPECT_EQ(layer.tile_at(TilePos {3, 3}), TileID {5});
  EXPECT_EQ(layer.tile_at(TilePos {3, 4}), TileID {5});
  EXPECT_EQ(layer.tile_at(TilePos {4, 3}), TileID {5});
  EXPECT_EQ(layer.tile_at(TilePos {4, 4}), TileID {5});

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
  flood_fill(layer, TilePos {0, 2}, TileID {8});

  EXPECT_EQ(layer.tile_at(TilePos {0, 2}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {1, 2}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {2, 0}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {2, 1}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {2, 2}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {2, 3}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {2, 4}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {3, 2}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {4, 2}), TileID {8});

  EXPECT_EQ(layer.tile_at(TilePos {0, 0}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {0, 1}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {1, 0}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {1, 1}), TileID {8});

  EXPECT_EQ(layer.tile_at(TilePos {0, 3}), TileID {3});
  EXPECT_EQ(layer.tile_at(TilePos {0, 4}), TileID {3});
  EXPECT_EQ(layer.tile_at(TilePos {1, 3}), TileID {3});
  EXPECT_EQ(layer.tile_at(TilePos {1, 4}), TileID {3});

  EXPECT_EQ(layer.tile_at(TilePos {3, 0}), TileID {4});
  EXPECT_EQ(layer.tile_at(TilePos {3, 1}), TileID {4});
  EXPECT_EQ(layer.tile_at(TilePos {4, 0}), TileID {4});
  EXPECT_EQ(layer.tile_at(TilePos {4, 1}), TileID {4});

  EXPECT_EQ(layer.tile_at(TilePos {3, 3}), TileID {5});
  EXPECT_EQ(layer.tile_at(TilePos {3, 4}), TileID {5});
  EXPECT_EQ(layer.tile_at(TilePos {4, 3}), TileID {5});
  EXPECT_EQ(layer.tile_at(TilePos {4, 4}), TileID {5});

  // This shouldn't change anything from the previous state.
  flood_fill(layer, TilePos {2, 2}, TileID {8});

  EXPECT_EQ(layer.tile_at(TilePos {0, 2}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {1, 2}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {2, 0}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {2, 1}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {2, 2}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {2, 3}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {2, 4}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {3, 2}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {4, 2}), TileID {8});

  EXPECT_EQ(layer.tile_at(TilePos {0, 0}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {0, 1}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {1, 0}), TileID {8});
  EXPECT_EQ(layer.tile_at(TilePos {1, 1}), TileID {8});

  EXPECT_EQ(layer.tile_at(TilePos {0, 3}), TileID {3});
  EXPECT_EQ(layer.tile_at(TilePos {0, 4}), TileID {3});
  EXPECT_EQ(layer.tile_at(TilePos {1, 3}), TileID {3});
  EXPECT_EQ(layer.tile_at(TilePos {1, 4}), TileID {3});

  EXPECT_EQ(layer.tile_at(TilePos {3, 0}), TileID {4});
  EXPECT_EQ(layer.tile_at(TilePos {3, 1}), TileID {4});
  EXPECT_EQ(layer.tile_at(TilePos {4, 0}), TileID {4});
  EXPECT_EQ(layer.tile_at(TilePos {4, 1}), TileID {4});

  EXPECT_EQ(layer.tile_at(TilePos {3, 3}), TileID {5});
  EXPECT_EQ(layer.tile_at(TilePos {3, 4}), TileID {5});
  EXPECT_EQ(layer.tile_at(TilePos {4, 3}), TileID {5});
  EXPECT_EQ(layer.tile_at(TilePos {4, 4}), TileID {5});
}

/// \tests tactile::flood_fill
TYPED_TEST(FloodFillTest, FloodFillWithAffectedPositions)
{
  const MatrixExtent extent {4, 4};
  TypeParam layer {extent};

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
  flood_fill(layer, TilePos {3, 3}, TileID {5}, &affected_positions);

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

// NOLINTEND(*-function-size, *-function-cognitive-complexity)

}  // namespace
