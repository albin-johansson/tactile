#include "tile_layer.hpp"

#include <catch.hpp>

#include "algorithm.hpp"
#include "position.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

using namespace tactile;

TEST_CASE("tile_layer(row_t, col_t)", "[tile_layer]")
{
  SECTION("Invalid dimensions")
  {
    CHECK_THROWS_AS(core::tile_layer(0_row, 0_col), tactile_error);
    CHECK_THROWS_AS(core::tile_layer(1_row, 0_col), tactile_error);
    CHECK_THROWS_AS(core::tile_layer(0_row, 1_col), tactile_error);
    CHECK_THROWS_AS(core::tile_layer(-1_row, -1_col), tactile_error);
  }

  SECTION("Valid dimensions")
  {
    CHECK_NOTHROW(core::tile_layer(1_row, 1_col));

    const auto rows = 18_row;
    const auto cols = 52_col;
    const core::tile_layer layer{rows, cols};
    CHECK(layer.row_count() == rows);
    CHECK(layer.col_count() == cols);
  }
}

TEST_CASE("tile_layer::for_each", "[tile_layer]")
{
  core::tile_layer layer;

  int count{0};
  layer.for_each([&](tile_id id) {
    ++count;
  });

  CHECK(count == layer.tile_count());
}

TEST_CASE("tile_layer::remove_all", "[tile_layer]")
{
  core::tile_layer layer;
  std::vector<core::position> positions;

  layer.flood({}, 1_t, positions);
  layer.for_each([](tile_id id) {
    CHECK(id == 1_t);
  });

  layer.remove_all(1_t);
  layer.for_each([](tile_id id) {
    CHECK(id == empty);
  });
}

TEST_CASE("tile_layer::add_row", "[tile_layer]")
{
  const auto nRows = 5_row;
  const tile_id tileID{84};

  core::tile_layer layer{nRows, 3_col};
  layer.add_row(tileID);

  CHECK(layer.row_count() == (nRows + 1_row));

  for (auto c = 0_col; c < layer.col_count(); ++c) {
    CHECK(*layer.tile_at({nRows - 1_row, c}) == empty);
    CHECK(*layer.tile_at({nRows, c}) == tileID);
  }
}

TEST_CASE("tile_layer::add_col", "[tile_layer]")
{
  const auto nCols = 7_col;
  const tile_id tileID{33};

  core::tile_layer layer{5_row, nCols};
  layer.add_col(tileID);

  CHECK(layer.col_count() == (nCols + 1_col));

  for (auto r = 0_row; r < layer.row_count(); ++r) {
    CHECK(*layer.tile_at({r, nCols - 1_col}) == empty);
    CHECK(*layer.tile_at({r, nCols}) == tileID);
  }
}

TEST_CASE("tile_layer::remove_row", "[tile_layer]")
{
  core::tile_layer layer{5_row, 5_col};
  for (int i = 0; i < 10; ++i) {
    layer.remove_row();  // shouldn't throw
  }
  CHECK(layer.row_count() == 1_row);
}

TEST_CASE("tile_layer::remove_col", "[tile_layer]")
{
  core::tile_layer layer{5_row, 5_col};
  for (int i = 0; i < 10; ++i) {
    layer.remove_col();  // shouldn't throw
  }
  CHECK(layer.col_count() == 1_col);
}

TEST_CASE("tile_layer::set_rows", "[tile_layer]")
{
  core::tile_layer layer{5_row, 5_col};

  CHECK(layer.row_count() == 5_row);
  CHECK(layer.col_count() == 5_col);

  layer.set_rows(32_row);
  CHECK(layer.row_count() == 32_row);
  CHECK(layer.col_count() == 5_col);

  layer.set_rows(15_row);
  CHECK(layer.row_count() == 15_row);
  CHECK(layer.col_count() == 5_col);
}

TEST_CASE("tile_layer::set_cols", "[tile_layer]")
{
  core::tile_layer layer{5_row, 5_col};

  CHECK(layer.row_count() == 5_row);
  CHECK(layer.col_count() == 5_col);

  layer.set_cols(41_col);
  CHECK(layer.row_count() == 5_row);
  CHECK(layer.col_count() == 41_col);

  layer.set_cols(8_col);
  CHECK(layer.row_count() == 5_row);
  CHECK(layer.col_count() == 8_col);
}

TEST_CASE("tile_layer::set_tile", "[tile_layer]")
{
  core::tile_layer layer;

  const core::position pos{2_row, 2_col};
  CHECK(*layer.tile_at(pos) == empty);

  const tile_id tileID{24};
  layer.set_tile(pos, tileID);

  CHECK(*layer.tile_at(pos) == tileID);

  CHECK_NOTHROW(layer.set_tile({-1_row, -1_col}, tile_id{5}));
  CHECK_NOTHROW(
      layer.set_tile({row_t{layer.row_count()}, col_t{layer.col_count()}},
                     tile_id{7}));
  CHECK(layer.tile_at({row_t{layer.row_count()}, col_t{layer.col_count()}}) ==
        std::nullopt);
}

TEST_CASE("tile_layer::set_opacity", "[tile_layer]")
{
  core::tile_layer layer;
  CHECK(layer.opacity() == 1.0);

  SECTION("Valid opacity")
  {
    const auto alpha = 0.75;
    layer.set_opacity(alpha);

    CHECK(layer.opacity() == alpha);
  }

  SECTION("Underflow opacity")
  {
    layer.set_opacity(-1);
    CHECK(layer.opacity() == 0);
  }

  SECTION("Overflow opacity")
  {
    layer.set_opacity(1.1);
    CHECK(layer.opacity() == 1);
  }
}

TEST_CASE("tile_layer::set_visible", "[tile_layer]")
{
  core::tile_layer layer;

  CHECK(layer.visible());

  layer.set_visible(false);
  CHECK(!layer.visible());

  layer.set_visible(true);
  CHECK(layer.visible());
}

TEST_CASE("tile_layer::row_count", "[tile_layer]")
{
  const core::tile_layer layer{5_row, 7_col};
  CHECK(layer.row_count() == 5_row);
}

TEST_CASE("tile_layer::col_count", "[tile_layer]")
{
  const core::tile_layer layer{5_row, 7_col};
  CHECK(layer.col_count() == 7_col);
}

TEST_CASE("tile_layer::tile_count", "[tile_layer]")
{
  const core::tile_layer layer{4_row, 4_col};
  CHECK(layer.tile_count() == 16);
}

TEST_CASE("tile_layer::tile_at", "[tile_layer]")
{
  const core::tile_layer layer{5_row, 5_col};
  CHECK(layer.tile_at({0_row, 0_col}) != std::nullopt);
  CHECK(layer.tile_at({4_row, 4_col}) != std::nullopt);
  CHECK(layer.tile_at({5_row, 5_col}) == std::nullopt);
}

TEST_CASE("tile_layer::in_bounds", "[tile_layer]")
{
  const core::tile_layer layer{5_row, 5_col};
  CHECK(layer.in_bounds({0_row, 0_col}));
  CHECK(layer.in_bounds({4_row, 4_col}));
  CHECK(!layer.in_bounds({5_row, 5_col}));
}

TEST_CASE("tile_layer::visible", "[tile_layer]")
{
  const core::tile_layer layer;
  CHECK(layer.visible());
}

TEST_CASE("tile_layer::name", "[tile_layer]")
{
  const core::tile_layer layer;
  CHECK(layer.name() == TACTILE_QSTRING(u"Tile layer"));
}