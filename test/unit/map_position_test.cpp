#include <catch.hpp>

#include "position.hpp"

using namespace tactile::model;

TEST_CASE("map_position()", "[map_position]")
{
  const position position;
  CHECK(position.row() == 0);
  CHECK(position.col() == 0);
}

TEST_CASE("map_position(int, int)", "[map_position]")
{
  SECTION("Valid arguments")
  {
    const auto row = 123;
    const auto col = 557;
    const position position{row, col};
    CHECK(position.row() == row);
    CHECK(position.col() == col);
  }

  SECTION("Edge-case arguments")
  {
    const position valid1{0, 0};
    CHECK(valid1.row() == 0);
    CHECK(valid1.col() == 0);

    const position invalid1{-1, 0};
    CHECK(invalid1.row() == 0);

    const position invalid2{0, -1};
    CHECK(invalid2.col() == 0);
  }
}

TEST_CASE("map_position::set_row", "[map_position]")
{
  position position;

  const auto row = 892;
  position.set_row(row);

  CHECK(position.row() == row);
}

TEST_CASE("map_position::set_col", "[map_position]")
{
  position position;

  const auto col = 438291;
  position.set_col(col);

  CHECK(position.col() == col);
}

TEST_CASE("map_position::row", "[map_position]")
{
  const auto row = 8324;
  const position position{row, 0};

  CHECK(position.row() == row);
}

TEST_CASE("map_position::col", "[map_position]")
{
  const auto col = 493;
  const position position{0, col};

  CHECK(position.col() == col);
}

TEST_CASE("map_position::urow", "[map_position]")
{
  const auto row = 6532;
  const position position{row, 0};

  CHECK(position.urow() == static_cast<std::size_t>(row));
}

TEST_CASE("map_position::ucol", "[map_position]")
{
  const auto col = 18343;
  const position position{0, col};

  CHECK(position.ucol() == static_cast<std::size_t>(col));
}

TEST_CASE("map_position::north", "[map_position]")
{
  const position fst{7, 15};
  const position snd = fst.north();

  CHECK(snd.row() == fst.row() - 1);
  CHECK(snd.col() == fst.col());
}

TEST_CASE("map_position::east", "[map_position]")
{
  const position fst{52, 77};
  const position snd = fst.east();

  CHECK(snd.row() == fst.row());
  CHECK(snd.col() == fst.col() + 1);
}

TEST_CASE("map_position::south", "[map_position]")
{
  const position fst{33, 6};
  const position snd = fst.south();

  CHECK(snd.row() == fst.row() + 1);
  CHECK(snd.col() == fst.col());
}

TEST_CASE("map_position::west", "[map_position]")
{
  const position fst{62, 39};
  const position snd = fst.west();

  CHECK(snd.row() == fst.row());
  CHECK(snd.col() == fst.col() - 1);
}

TEST_CASE("map_position operator==", "[map_position]")
{
  const position pos{45, 23};
  CHECK(pos == pos);

  const position copy{pos};

  CHECK(pos == copy);
  CHECK(copy == pos);
}

TEST_CASE("map_position operator!=", "[map_position]")
{
  const position pos{45, 23};
  CHECK(!(pos != pos));

  const position other{839, 54};
  CHECK(pos != other);
  CHECK(other != pos);
}