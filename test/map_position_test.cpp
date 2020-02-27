#include "map_position.h"

#include <catch.hpp>

using namespace tactile;

TEST_CASE("MapPosition()", "[MapPosition]")
{
  const MapPosition position;
  CHECK(position.row() == 0);
  CHECK(position.col() == 0);
}

TEST_CASE("MapPosition(int, int)", "[MapPosition]")
{
  SECTION("Valid arguments")
  {
    const auto row = 123;
    const auto col = 557;
    const MapPosition position{row, col};
    CHECK(position.row() == row);
    CHECK(position.col() == col);
  }

  SECTION("Edge-case arguments")
  {
    const MapPosition valid1{0, 0};
    CHECK(valid1.row() == 0);
    CHECK(valid1.col() == 0);

    const MapPosition invalid1{-1, 0};
    CHECK(invalid1.row() == 0);

    const MapPosition invalid2{0, -1};
    CHECK(invalid2.col() == 0);
  }
}

TEST_CASE("MapPosition::set_row", "[MapPosition]")
{
  MapPosition position;

  const auto row = 892;
  position.set_row(row);

  CHECK(position.row() == row);
}

TEST_CASE("MapPosition::set_col", "[MapPosition]")
{
  MapPosition position;

  const auto col = 438291;
  position.set_col(col);

  CHECK(position.col() == col);
}

TEST_CASE("MapPosition::row", "[MapPosition]")
{
  const auto row = 8324;
  const MapPosition position{row, 0};

  CHECK(position.row() == row);
}

TEST_CASE("MapPosition::col", "[MapPosition]")
{
  const auto col = 493;
  const MapPosition position{0, col};

  CHECK(position.col() == col);
}

TEST_CASE("MapPosition equality operator", "[MapPosition]")
{
  const MapPosition pos{45, 23};
  CHECK(pos == pos);

  const MapPosition copy{pos};

  CHECK(pos == copy);
  CHECK(copy == pos);
}

TEST_CASE("MapPosition inequality operator", "[MapPosition]")
{
  const MapPosition pos{45, 23};
  CHECK(!(pos != pos));

  const MapPosition other{839, 54};
  CHECK(pos != other);
  CHECK(other != pos);
}