#include "map.hpp"

#include <catch.hpp>

#include "algorithm.hpp"

using namespace tactile;

TEST_CASE("map(int, int)", "[map]")
{
  SECTION("Bad args")
  {
    const model::map map{0, 0};
    CHECK(map.rows() == 1);
    CHECK(map.cols() == 1);
  }

  SECTION("Good args")
  {
    const auto rows = 7;
    const auto cols = 5;
    const model::map map{rows, cols};
    CHECK(map.rows() == rows);
    CHECK(map.cols() == cols);
  }
}

TEST_CASE("map::add_layer", "[map]")
{
  model::map map{5, 5};

  const auto nLayers = map.num_layers();

  map.add_layer();

  CHECK(map.num_layers() == nLayers + 1);
}

TEST_CASE("map::add_row", "[map]")
{
  const auto start = 4;
  model::map map{start, 4};

  map.add_row();

  CHECK(map.rows() == start + 1);
}

TEST_CASE("map::add_col", "[map]")
{
  const auto start = 7;
  model::map map{1, start};

  map.add_col();

  CHECK(map.cols() == start + 1);
}

TEST_CASE("map::remove_row", "[map]")
{
  const auto initialRows = 4;
  model::map map{initialRows, 10};

  map.remove_row();
  CHECK(map.rows() == (initialRows - 1));

  do_n(20, [&map] { map.remove_row(); });

  CHECK(map.rows() == 1);
}

TEST_CASE("map::remove_col", "[map]")
{
  const auto initialCols = 9;
  model::map map{7, initialCols};

  map.remove_col();
  CHECK(map.cols() == (initialCols - 1));

  do_n(20, [&map] { map.remove_col(); });

  CHECK(map.cols() == 1);
}

TEST_CASE("map::set_rows", "[map]")
{
  model::map map{3, 3};

  const auto nRows = 12;
  map.set_rows(nRows);

  CHECK(map.cols() == 3);
  CHECK(map.rows() == nRows);

  map.set_rows(0);
  CHECK(map.rows() == 1);

  map.set_rows(-1);
  CHECK(map.rows() == 1);
}

TEST_CASE("map::set_cols", "[map]")
{
  model::map map{3, 3};

  const auto nCols = 9;
  map.set_cols(nCols);

  CHECK(map.rows() == 3);
  CHECK(map.cols() == nCols);

  map.set_cols(0);
  CHECK(map.cols() == 1);

  map.set_cols(-1);
  CHECK(map.cols() == 1);
}

TEST_CASE("map::set_visibility", "[map]")
{
  model::map map{5, 5};

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
  model::map map{3, 3};
  CHECK(map.num_layers() == 1);
}

TEST_CASE("map::is_visible", "[map]")
{
  model::map map{5, 5};

  CHECK(!map.is_visible(layer_id{-1}));
  CHECK(!map.is_visible(layer_id{1}));
  CHECK(map.is_visible(layer_id{0}));
}