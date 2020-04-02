#include "tile_size.h"

#include <catch.hpp>

using namespace tactile;

TEST_CASE("TileSize::reset", "[TileSize]")
{
  TileSize size;

  size.set_size(74);
  size.reset();

  CHECK(size.size() == TileSize::defaultSize);
}

TEST_CASE("TileSize::increase", "[TileSize]")
{
  TileSize size;

  size.set_size(100);
  size.increase();

  CHECK(size.size() == 110);  // should increase by 10 percent
}

TEST_CASE("TileSize::decrease", "[TileSize]")
{
  TileSize size;

  size.set_size(100);
  size.decrease();

  CHECK(size.size() == 90);  // should decrease by 10 percent

  for (int i = 0; i < 100; ++i) {
    size.decrease();
  }

  CHECK(size.size() == TileSize::minimumSize);
}

TEST_CASE("TileSize::set_size", "[TileSize]")
{
  TileSize tileSize;

  SECTION("Valid size")
  {
    const auto newSize = 83;
    tileSize.set_size(newSize);

    CHECK(tileSize.size() == newSize);
  }

  SECTION("Check that minimum size is enforced")
  {
    const auto badSize = TileSize::minimumSize - 1;
    tileSize.set_size(badSize);

    CHECK(tileSize.size() == TileSize::minimumSize);
  }
}