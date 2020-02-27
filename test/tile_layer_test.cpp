#include "tile_layer.h"

#include <catch.hpp>

using namespace tactile;

TEST_CASE("TileLayer(int, int)", "[TileLayer]")
{
  CHECK_NOTHROW(TileLayer{-1, -1});
  CHECK_NOTHROW(TileLayer{0, 0});
}