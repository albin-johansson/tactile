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