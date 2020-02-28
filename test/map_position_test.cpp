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

TEST_CASE("MapPosition::north", "[MapPosition]") {
  const MapPosition fst{7, 15};
  const MapPosition snd = fst.north();

  CHECK(snd.row() == fst.row() - 1);
  CHECK(snd.col() == fst.col());
}

TEST_CASE("MapPosition::east", "[MapPosition]") {
  const MapPosition fst{52, 77};
  const MapPosition snd = fst.east();

  CHECK(snd.row() == fst.row());
  CHECK(snd.col() == fst.col() + 1);
}

TEST_CASE("MapPosition::south", "[MapPosition]") {
  const MapPosition fst{33, 6};
  const MapPosition snd = fst.south();

  CHECK(snd.row() == fst.row() + 1);
  CHECK(snd.col() == fst.col());
}

TEST_CASE("MapPosition::west", "[MapPosition]") {
  const MapPosition fst{62, 39};
  const MapPosition snd = fst.west();

  CHECK(snd.row() == fst.row());
  CHECK(snd.col() == fst.col() - 1);
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