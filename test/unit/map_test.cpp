#include "map.hpp"

#include <catch.hpp>

#include "algorithm.hpp"
#include "tactile_error.hpp"

using namespace tactile;
using core::operator""_row;
using core::operator""_col;

TEST_CASE("map(row_t, col_t)", "[map]")
{
  SECTION("Invalid dimensions")
  {
    CHECK_THROWS_AS(core::map(0_row, 1_col), tactile_error);
    CHECK_THROWS_AS(core::map(1_row, 0_col), tactile_error);
    CHECK_THROWS_AS(core::map(0_row, 0_col), tactile_error);
    CHECK_THROWS_AS(core::map(-1_row, -1_col), tactile_error);
  }

  SECTION("Valid dimensions")
  {
    CHECK_NOTHROW(core::map{1_row, 1_col});

    const auto rows = 7_row;
    const auto cols = 5_col;
    const core::map map{rows, cols};
    CHECK(map.row_count() == rows);
    CHECK(map.col_count() == cols);
  }
}

TEST_CASE("map::set_tile", "[map]")
{
  core::map map{4_row, 4_col};

  const core::position pos{1_row, 2_col};
  map.set_tile(pos, 7_t);

  CHECK(map.tile_at(pos) == 7_t);
}

TEST_CASE("map::remove_all", "[map]")
{
  core::map map{4_row, 4_col};

  map.add_layer();
  map.add_layer();

  std::vector<core::position> positions;
  map.flood({}, empty, positions);
  map.remove_all(1_t);

  for (const auto& [key, layer] : map) {
    layer.for_each([](tile_id id) { CHECK(id == empty); });
  }
}

TEST_CASE("map::select_layer", "[map]")
{
  core::map map{4_row, 4_col};
  CHECK(map.active_layer_id() == 1_layer);

  CHECK_NOTHROW(map.select_layer(-1_layer));
  CHECK(map.active_layer_id() == 1_layer);

  map.add_layer();
  map.select_layer(1_layer);
  CHECK(map.active_layer_id() == 1_layer);

  CHECK_NOTHROW(map.select_layer(2_layer));
  CHECK(map.active_layer_id() == 2_layer);
}

TEST_CASE("map::add_layer", "[map]")
{
  core::map map{5_row, 5_col};

  const auto count = map.layer_count();
  map.add_layer();

  CHECK(map.layer_count() == count + 1);
}

TEST_CASE("map::add_row", "[map]")
{
  const auto start = 4_row;
  core::map map{start, 4_col};

  map.add_row();

  CHECK(map.row_count() == start + 1_row);
}

TEST_CASE("map::add_col", "[map]")
{
  const auto start = 7_col;
  core::map map{1_row, start};

  map.add_col();

  CHECK(map.col_count() == start + 1_col);
}

TEST_CASE("map::remove_row", "[map]")
{
  const auto initialRows = 4_row;
  core::map map{initialRows, 10_col};

  map.remove_row();
  CHECK(map.row_count() == (initialRows - 1_row));

  invoke_n(20, [&map] { map.remove_row(); });

  CHECK(map.row_count() == 1_row);
}

TEST_CASE("map::remove_col", "[map]")
{
  const auto initialCols = 9_col;
  core::map map{7_row, initialCols};

  map.remove_col();
  CHECK(map.col_count() == (initialCols - 1_col));

  invoke_n(20, [&map] { map.remove_col(); });

  CHECK(map.col_count() == 1_col);
}

TEST_CASE("map::set_rows", "[map]")
{
  core::map map{3_row, 3_col};

  const auto nRows = 12_row;
  map.set_rows(nRows);

  CHECK(map.col_count() == 3_col);
  CHECK(map.row_count() == nRows);

  map.set_rows(0_row);
  CHECK(map.row_count() == 1_row);

  map.set_rows(-1_row);
  CHECK(map.row_count() == 1_row);
}

TEST_CASE("map::set_cols", "[map]")
{
  core::map map{3_row, 3_col};

  const auto nCols = 9_col;
  map.set_cols(nCols);

  CHECK(map.row_count() == 3_row);
  CHECK(map.col_count() == nCols);

  map.set_cols(0_col);
  CHECK(map.col_count() == 1_col);

  map.set_cols(-1_col);
  CHECK(map.col_count() == 1_col);
}

TEST_CASE("map::set_visibility", "[map]")
{
  core::map map{5_row, 5_col};

  CHECK(!map.is_visible(-1_layer));  // invalid index
  CHECK(!map.is_visible(0_layer));   // invalid index
  CHECK(map.is_visible(1_layer));    // valid index, should be true

  map.set_visibility(1_layer, false);
  CHECK(!map.is_visible(1_layer));

  map.set_visibility(1_layer, true);
  CHECK(map.is_visible(1_layer));
}

TEST_CASE("map::tile_at", "[map]")
{
  const core::map map{4_row, 4_col};
  CHECK(map.tile_at({}).value() == empty);
  CHECK(map.tile_at({3_row, 3_col}).value() == empty);
  CHECK(!map.tile_at({4_row, 4_col}));
}

TEST_CASE("map::is_visible", "[map]")
{
  const core::map map{5_row, 5_col};

  CHECK(!map.is_visible(-1_layer));
  CHECK(!map.is_visible(0_layer));
  CHECK(map.is_visible(1_layer));
}

TEST_CASE("map::layer_count", "[map]")
{
  core::map map{3_row, 3_col};
  CHECK(map.layer_count() == 1);

  map.add_layer();
  CHECK(map.layer_count() == 2);
}

TEST_CASE("map::has_layer", "[map]")
{
  core::map map{4_row, 4_col};

  CHECK(map.has_layer(1_layer));
  CHECK(!map.has_layer(-1_layer));
  CHECK(!map.has_layer(2_layer));

  map.add_layer();
  CHECK(map.has_layer(2_layer));
  CHECK(!map.has_layer(3_layer));
}

TEST_CASE("map::in_bounds", "[map]")
{
  const core::map map{4_row, 4_col};

  CHECK(map.in_bounds({0_row, 0_col}));
  CHECK(map.in_bounds({3_row, 3_col}));
  CHECK(!map.in_bounds({4_row, 3_col}));
  CHECK(!map.in_bounds({3_row, 4_col}));
  CHECK(!map.in_bounds({4_row, 4_col}));
}

TEST_CASE("map::row_count", "[map]")
{
  const core::map map{7_row, 3_col};
  CHECK(map.row_count() == 7_row);
}

TEST_CASE("map::col_count", "[map]")
{
  const core::map map{2_row, 12_col};
  CHECK(map.col_count() == 12_col);
}

TEST_CASE("map::tile_count", "[map]")
{
  const core::map map{8_row, 12_col};
  CHECK(map.tile_count() == 96);
}

TEST_CASE("map::width", "[map]")
{
  const core::map map{4_row, 5_col};
  CHECK(map.width() == map.col_count().get() * map.current_tile_size());
}

TEST_CASE("map::height", "[map]")
{
  const core::map map{4_row, 5_col};
  CHECK(map.height() == map.row_count().get() * map.current_tile_size());
}

TEST_CASE("map::active_layer_id", "[map]")
{
  core::map map{4_row, 5_col};
  CHECK(map.active_layer_id() == 1_layer);

  map.add_layer();
  map.select_layer(2_layer);
  CHECK(map.active_layer_id() == 2_layer);
}

TEST_CASE("map::current_tile_size", "[map]")
{
  const core::map map{4_row, 4_col};
  CHECK(map.current_tile_size() == core::tile_size::default_size());
}