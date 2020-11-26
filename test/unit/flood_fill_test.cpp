#include "flood_fill.hpp"

#include <catch.hpp>
#include <iostream>

using namespace tactile;

TEST_CASE("Correctness of flood fill algorithm", "[flood_fill]")
{
  core::layer layer{5_row, 5_col};
  for (int i = 0; i < 5; ++i) {
    layer.set_tile({row_t{i}, col_t{i}}, 1_t);
  }

  /*
   1 0 0 0 0
   0 1 0 0 0
   0 0 1 0 0
   0 0 0 1 0
   0 0 0 0 1
   */

  const tile_id replacement{2};
  std::vector<core::position> positions;
  layer.flood({1_row, 0_col}, replacement, positions);

  /*
   Expected:
   1 0 0 0 0
   2 1 0 0 0
   2 2 1 0 0
   2 2 2 1 0
   2 2 2 2 1
   */

  SECTION("Affected tiles")
  {
    CHECK(*layer.tile_at({1_row, 0_col}) == replacement);
    CHECK(*layer.tile_at({2_row, 0_col}) == replacement);
    CHECK(*layer.tile_at({3_row, 0_col}) == replacement);
    CHECK(*layer.tile_at({4_row, 0_col}) == replacement);

    CHECK(*layer.tile_at({2_row, 1_col}) == replacement);
    CHECK(*layer.tile_at({3_row, 1_col}) == replacement);
    CHECK(*layer.tile_at({4_row, 1_col}) == replacement);

    CHECK(*layer.tile_at({3_row, 2_col}) == replacement);
    CHECK(*layer.tile_at({4_row, 2_col}) == replacement);

    CHECK(*layer.tile_at({4_row, 3_col}) == replacement);
  }

  SECTION("Unaffected diagonal blocking the flood")
  {
    const tile_id id{1};
    CHECK(*layer.tile_at({0_row, 0_col}) == id);
    CHECK(*layer.tile_at({1_row, 1_col}) == id);
    CHECK(*layer.tile_at({2_row, 2_col}) == id);
    CHECK(*layer.tile_at({3_row, 3_col}) == id);
    CHECK(*layer.tile_at({4_row, 4_col}) == id);
  }

  SECTION("Unaffected right hand side")
  {
    CHECK(*layer.tile_at({0_row, 1_col}) == empty);
    CHECK(*layer.tile_at({0_row, 2_col}) == empty);
    CHECK(*layer.tile_at({0_row, 3_col}) == empty);
    CHECK(*layer.tile_at({0_row, 4_col}) == empty);

    CHECK(*layer.tile_at({1_row, 2_col}) == empty);
    CHECK(*layer.tile_at({1_row, 3_col}) == empty);
    CHECK(*layer.tile_at({1_row, 4_col}) == empty);

    CHECK(*layer.tile_at({2_row, 3_col}) == empty);
    CHECK(*layer.tile_at({2_row, 4_col}) == empty);

    CHECK(*layer.tile_at({3_row, 4_col}) == empty);
  }
}

TEST_CASE("Out-of-bounds position", "[flood_fill]")
{
  core::layer layer{5_row, 5_col};

  std::vector<core::position> positions;
  CHECK_NOTHROW(layer.flood({5_row, 5_col}, empty, positions));

  positions.clear();
  CHECK_NOTHROW(layer.flood({6_row, 6_col}, empty, positions));

  positions.clear();
  CHECK_NOTHROW(core::flood_fill(layer, {6_row, 6_col}, empty, positions));
}