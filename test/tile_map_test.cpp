#include "tile_map.h"

#include <catch.hpp>

using namespace tactile;

TEST_CASE("TileMap(int, int)", "[TileMap]")
{
  SECTION("Bad args")
  {
    const TileMap map{0, 0};
    CHECK(map.rows() == 1);
    CHECK(map.cols() == 1);
  }

  SECTION("Good args")
  {
    const auto rows = 7;
    const auto cols = 5;
    const TileMap map{rows, cols};
    CHECK(map.rows() == rows);
    CHECK(map.cols() == cols);
  }
}

TEST_CASE("TileMap::add_layer", "[TileMap]")
{
  TileMap map{5, 5};

  const auto nLayers = map.get_layer_amount();

  map.add_layer();

  CHECK(map.get_layer_amount() == nLayers + 1);
}

TEST_CASE("TileMap::add_row", "[TileMap]")
{
  const auto start = 4;
  TileMap map{start, 4};

  map.add_row();

  CHECK(map.rows() == start + 1);
}

TEST_CASE("TileMap::add_col", "[TileMap]")
{
  const auto start = 7;
  TileMap map{1, start};

  map.add_col();

  CHECK(map.cols() == start + 1);
}

TEST_CASE("TileMap::set_rows", "[TileMap]")
{
  TileMap map{3, 3};

  const auto nRows = 12;
  map.set_rows(nRows);

  CHECK(map.cols() == 3);
  CHECK(map.rows() == nRows);

  map.set_rows(0);
  CHECK(map.rows() == 1);

  map.set_rows(-1);
  CHECK(map.rows() == 1);
}

TEST_CASE("TileMap::set_cols", "[TileMap]")
{
  TileMap map{3, 3};

  const auto nCols = 9;
  map.set_cols(nCols);

  CHECK(map.rows() == 3);
  CHECK(map.cols() == nCols);

  map.set_cols(0);
  CHECK(map.cols() == 1);

  map.set_cols(-1);
  CHECK(map.cols() == 1);
}

TEST_CASE("TileMap::set_visibility", "[TileMap]")
{
  TileMap map{5, 5};

  CHECK(!map.is_visible(-1));  // invalid index
  CHECK(map.is_visible(0));    // valid index, should be true
  CHECK(!map.is_visible(1));   // invalid index

  map.set_visibility(0, false);
  CHECK(!map.is_visible(0));

  map.set_visibility(0, true);
  CHECK(map.is_visible(0));
}

TEST_CASE("TileMap::get_layer_amount", "[TileMap]")
{
  TileMap map{3, 3};
  CHECK(map.get_layer_amount() == 1);
}

TEST_CASE("TileMap::is_visible", "[TileMap]")
{
  TileMap map{5, 5};

  CHECK(!map.is_visible(-1));
  CHECK(!map.is_visible(1));
  CHECK(map.is_visible(0));
}