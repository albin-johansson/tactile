#include "map.hpp"

#include <catch.hpp>

#include "algorithm.hpp"

using namespace tactile;
using namespace core;

TEST_CASE("map(int, int)", "[map]")
{
  SECTION("Bad args")
  {
    const core::map map{0_row, 0_col};
    CHECK(map.rows() == 1_row);
    CHECK(map.cols() == 1_col);
  }

  SECTION("Good args")
  {
    const auto rows = 7_row;
    const auto cols = 5_col;
    const core::map map{rows, cols};
    CHECK(map.rows() == rows);
    CHECK(map.cols() == cols);
  }
}

TEST_CASE("map::add_layer", "[map]")
{
  core::map map{5_row, 5_col};

  const auto nLayers = map.num_layers();

  map.add_layer();

  CHECK(map.num_layers() == nLayers + 1);
}

TEST_CASE("map::add_row", "[map]")
{
  const auto start = 4_row;
  core::map map{start, 4_col};

  map.add_row();

  CHECK(map.rows() == start + 1_row);
}

TEST_CASE("map::add_col", "[map]")
{
  const auto start = 7_col;
  core::map map{1_row, start};

  map.add_col();

  CHECK(map.cols() == start + 1_col);
}

TEST_CASE("map::remove_row", "[map]")
{
  const auto initialRows = 4_row;
  core::map map{initialRows, 10_col};

  map.remove_row();
  CHECK(map.rows() == (initialRows - 1_row));

  do_n(20, [&map] { map.remove_row(); });

  CHECK(map.rows() == 1_row);
}

TEST_CASE("map::remove_col", "[map]")
{
  const auto initialCols = 9_col;
  core::map map{7_row, initialCols};

  map.remove_col();
  CHECK(map.cols() == (initialCols - 1_col));

  do_n(20, [&map] { map.remove_col(); });

  CHECK(map.cols() == 1_col);
}

TEST_CASE("map::set_rows", "[map]")
{
  core::map map{3_row, 3_col};

  const auto nRows = 12_row;
  map.set_rows(nRows);

  CHECK(map.cols() == 3_col);
  CHECK(map.rows() == nRows);

  map.set_rows(0_row);
  CHECK(map.rows() == 1_row);

  map.set_rows(-1_row);
  CHECK(map.rows() == 1_row);
}

TEST_CASE("map::set_cols", "[map]")
{
  core::map map{3_row, 3_col};

  const auto nCols = 9_col;
  map.set_cols(nCols);

  CHECK(map.rows() == 3_row);
  CHECK(map.cols() == nCols);

  map.set_cols(0_col);
  CHECK(map.cols() == 1_col);

  map.set_cols(-1_col);
  CHECK(map.cols() == 1_col);
}

TEST_CASE("map::set_visibility", "[map]")
{
  core::map map{5_row, 5_col};

  CHECK(!map.is_visible(layer_id{-1}));  // invalid index
  CHECK(map.is_visible(layer_id{0}));    // valid index, should be true
  CHECK(!map.is_visible(layer_id{1}));   // invalid index

  map.set_visibility(layer_id{0}, false);
  CHECK(!map.is_visible(layer_id{0}));

  map.set_visibility(layer_id{0}, true);
  CHECK(map.is_visible(layer_id{0}));
}

TEST_CASE("map::get_layer_amount", "[map]")
{
  core::map map{3_row, 3_col};
  CHECK(map.num_layers() == 1);
}

TEST_CASE("map::is_visible", "[map]")
{
  core::map map{5_row, 5_col};

  CHECK(!map.is_visible(layer_id{-1}));
  CHECK(!map.is_visible(layer_id{1}));
  CHECK(map.is_visible(layer_id{0}));
}