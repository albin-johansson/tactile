#include "layer.hpp"

#include <catch.hpp>

#include "algorithm.hpp"
#include "position.hpp"

using namespace tactile;
using core::operator""_row;
using core::operator""_col;

TEST_CASE("layer(row_t, col_t)", "[layer]")
{
  SECTION("Negative dimensions")
  {
    const core::layer layer{-1_row, -1_col};
    CHECK(layer.row_count() == 1_row);
    CHECK(layer.col_count() == 1_col);
  }

  SECTION("Zeroed dimensions")
  {
    const core::layer layer{0_row, 0_col};
    CHECK(layer.row_count() == 1_row);
    CHECK(layer.col_count() == 1_col);
  }

  SECTION("Valid dimensions")
  {
    const auto rows = 18_row;
    const auto cols = 52_col;
    const core::layer layer{rows, cols};
    CHECK(layer.row_count() == rows);
    CHECK(layer.col_count() == cols);
  }
}

TEST_CASE("layer::for_each", "[layer]")
{
  core::layer layer{10_row, 10_col};

  int count{0};
  layer.for_each([&](tile_id id) { ++count; });

  CHECK(count == layer.tile_count());
}

TEST_CASE("layer::remove_all", "[layer]")
{
  core::layer layer{5_row, 5_col};
  std::vector<core::position> positions;

  layer.flood({}, 1_t, positions);
  layer.for_each([](tile_id id) { CHECK(id == 1_t); });

  layer.remove_all(1_t);
  layer.for_each([](tile_id id) { CHECK(id == empty); });
}

TEST_CASE("layer::add_row", "[layer]")
{
  const auto nRows = 5_row;
  const tile_id tileID{84};

  core::layer layer{nRows, 3_col};
  layer.add_row(tileID);

  CHECK(layer.row_count() == (nRows + 1_row));

  for (auto c = 0_col; c < layer.col_count(); ++c) {
    CHECK(*layer.tile_at({nRows - 1_row, c}) == empty);
    CHECK(*layer.tile_at({nRows, c}) == tileID);
  }
}

TEST_CASE("layer::add_col", "[layer]")
{
  const auto nCols = 7_col;
  const tile_id tileID{33};

  core::layer layer{5_row, nCols};
  layer.add_col(tileID);

  CHECK(layer.col_count() == (nCols + 1_col));

  for (auto r = 0_row; r < layer.row_count(); ++r) {
    CHECK(*layer.tile_at({r, nCols - 1_col}) == empty);
    CHECK(*layer.tile_at({r, nCols}) == tileID);
  }
}

TEST_CASE("layer::remove_row", "[layer]")
{
  core::layer layer{5_row, 5_col};
  for (int i = 0; i < 10; ++i) {
    layer.remove_row();  // shouldn't throw
  }
  CHECK(layer.row_count() == 1_row);
}

TEST_CASE("layer::remove_col", "[layer]")
{
  core::layer layer{5_row, 5_col};
  for (int i = 0; i < 10; ++i) {
    layer.remove_col();  // shouldn't throw
  }
  CHECK(layer.col_count() == 1_col);
}

TEST_CASE("layer::set_rows", "[layer]")
{
  core::layer layer{5_row, 5_col};

  CHECK(layer.row_count() == 5_row);
  CHECK(layer.col_count() == 5_col);

  layer.set_rows(32_row);
  CHECK(layer.row_count() == 32_row);
  CHECK(layer.col_count() == 5_col);

  layer.set_rows(15_row);
  CHECK(layer.row_count() == 15_row);
  CHECK(layer.col_count() == 5_col);
}

TEST_CASE("layer::set_cols", "[layer]")
{
  core::layer layer{5_row, 5_col};

  CHECK(layer.row_count() == 5_row);
  CHECK(layer.col_count() == 5_col);

  layer.set_cols(41_col);
  CHECK(layer.row_count() == 5_row);
  CHECK(layer.col_count() == 41_col);

  layer.set_cols(8_col);
  CHECK(layer.row_count() == 5_row);
  CHECK(layer.col_count() == 8_col);
}

TEST_CASE("layer::set_tile", "[layer]")
{
  core::layer layer{5_row, 5_col};

  const core::position pos{2_row, 2_col};
  CHECK(*layer.tile_at(pos) == empty);

  const tile_id tileID{24};
  layer.set_tile(pos, tileID);

  CHECK(*layer.tile_at(pos) == tileID);

  CHECK_NOTHROW(layer.set_tile({-1_row, -1_col}, tile_id{5}));
  CHECK_NOTHROW(layer.set_tile(
      {core::row_t{layer.row_count()}, core::col_t{layer.col_count()}},
      tile_id{7}));
  CHECK(layer.tile_at({core::row_t{layer.row_count()},
                       core::col_t{layer.col_count()}}) == std::nullopt);
}

TEST_CASE("layer::set_visible", "[layer]")
{
  core::layer layer{5_row, 5_col};

  CHECK(layer.visible());

  layer.set_visible(false);
  CHECK(!layer.visible());

  layer.set_visible(true);
  CHECK(layer.visible());
}

TEST_CASE("layer::row_count", "[layer]")
{
  const core::layer layer{5_row, 7_col};
  CHECK(layer.row_count() == 5_row);
}

TEST_CASE("layer::col_count", "[layer]")
{
  const core::layer layer{5_row, 7_col};
  CHECK(layer.col_count() == 7_col);
}

TEST_CASE("layer::tile_count", "[layer]")
{
  const core::layer layer{4_row, 4_col};
  CHECK(layer.tile_count() == 16);
}

TEST_CASE("layer::tile_at", "[layer]")
{
  const core::layer layer{5_row, 5_col};
  CHECK(layer.tile_at({0_row, 0_col}) != std::nullopt);
  CHECK(layer.tile_at({4_row, 4_col}) != std::nullopt);
  CHECK(layer.tile_at({5_row, 5_col}) == std::nullopt);
}

TEST_CASE("layer::in_bounds", "[layer]")
{
  const core::layer layer{5_row, 5_col};
  CHECK(layer.in_bounds({0_row, 0_col}));
  CHECK(layer.in_bounds({4_row, 4_col}));
  CHECK(!layer.in_bounds({5_row, 5_col}));
}

TEST_CASE("layer::visible", "[layer]")
{
  const core::layer layer{4_row, 4_col};
  CHECK(layer.visible());
}