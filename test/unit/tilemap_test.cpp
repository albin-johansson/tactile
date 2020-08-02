#include "tilemap.hpp"

#include <catch.hpp>

#include "algorithm_utils.hpp"

using namespace tactile;

TEST_CASE("Tilemap(int, int)", "[Tilemap]")
{
  SECTION("Bad args")
  {
    const Tilemap map{0, 0};
    CHECK(map.rows() == 1);
    CHECK(map.cols() == 1);
  }

  SECTION("Good args")
  {
    const auto rows = 7;
    const auto cols = 5;
    const Tilemap map{rows, cols};
    CHECK(map.rows() == rows);
    CHECK(map.cols() == cols);
  }
}

TEST_CASE("Tilemap::add_layer", "[Tilemap]")
{
  Tilemap map{5, 5};

  const auto nLayers = map.get_layer_amount();

  map.add_layer();

  CHECK(map.get_layer_amount() == nLayers + 1);
}

TEST_CASE("Tilemap::add_row", "[Tilemap]")
{
  const auto start = 4;
  Tilemap map{start, 4};

  map.add_row();

  CHECK(map.rows() == start + 1);
}

TEST_CASE("Tilemap::add_col", "[Tilemap]")
{
  const auto start = 7;
  Tilemap map{1, start};

  map.add_col();

  CHECK(map.cols() == start + 1);
}

TEST_CASE("Tilemap::remove_row", "[Tilemap]")
{
  const auto initialRows = 4;
  Tilemap map{initialRows, 10};

  map.remove_row();
  CHECK(map.rows() == (initialRows - 1));

  repeat(20, [&map] { map.remove_row(); });

  CHECK(map.rows() == 1);
}

TEST_CASE("Tilemap::remove_col", "[Tilemap]")
{
  const auto initialCols = 9;
  Tilemap map{7, initialCols};

  map.remove_col();
  CHECK(map.cols() == (initialCols - 1));

  repeat(20, [&map] { map.remove_col(); });

  CHECK(map.cols() == 1);
}

TEST_CASE("Tilemap::set_rows", "[Tilemap]")
{
  Tilemap map{3, 3};

  const auto nRows = 12;
  map.set_rows(nRows);

  CHECK(map.cols() == 3);
  CHECK(map.rows() == nRows);

  map.set_rows(0);
  CHECK(map.rows() == 1);

  map.set_rows(-1);
  CHECK(map.rows() == 1);
}

TEST_CASE("Tilemap::set_cols", "[Tilemap]")
{
  Tilemap map{3, 3};

  const auto nCols = 9;
  map.set_cols(nCols);

  CHECK(map.rows() == 3);
  CHECK(map.cols() == nCols);

  map.set_cols(0);
  CHECK(map.cols() == 1);

  map.set_cols(-1);
  CHECK(map.cols() == 1);
}

TEST_CASE("Tilemap::set_visibility", "[Tilemap]")
{
  Tilemap map{5, 5};

  CHECK(!map.is_visible(-1));  // invalid index
  CHECK(map.is_visible(0));    // valid index, should be true
  CHECK(!map.is_visible(1));   // invalid index

  map.set_visibility(0, false);
  CHECK(!map.is_visible(0));

  map.set_visibility(0, true);
  CHECK(map.is_visible(0));
}

TEST_CASE("Tilemap::get_layer_amount", "[Tilemap]")
{
  Tilemap map{3, 3};
  CHECK(map.get_layer_amount() == 1);
}

TEST_CASE("Tilemap::is_visible", "[Tilemap]")
{
  Tilemap map{5, 5};

  CHECK(!map.is_visible(-1));
  CHECK(!map.is_visible(1));
  CHECK(map.is_visible(0));
}