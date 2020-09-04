#include <catch.hpp>

#include "position.hpp"

using namespace tactile::model;

TEST_CASE("map_position()", "[map_position]")
{
  const position position;
  CHECK(position.get_row() == 0_row);
  CHECK(position.get_col() == 0_col);
}

TEST_CASE("map_position(int, int)", "[map_position]")
{
  SECTION("Valid arguments")
  {
    const auto row = 123_row;
    const auto col = 557_col;
    const position position{row, col};
    CHECK(position.get_row() == row);
    CHECK(position.get_col() == col);
  }

  SECTION("Edge-case arguments")
  {
    const position valid1{0_row, 0_col};
    CHECK(valid1.get_row() == 0_row);
    CHECK(valid1.get_col() == 0_col);

    const position invalid1{-1_row, 0_col};
    CHECK(invalid1.get_row() == 0_row);

    const position invalid2{0_row, -1_col};
    CHECK(invalid2.get_col() == 0_col);
  }
}

TEST_CASE("map_position::set_row", "[map_position]")
{
  position position;

  const auto row = 892_row;
  position.set_row(row);

  CHECK(position.get_row() == row);
}

TEST_CASE("map_position::set_col", "[map_position]")
{
  position position;

  const auto col = 438291_col;
  position.set_col(col);

  CHECK(position.get_col() == col);
}

TEST_CASE("map_position::row", "[map_position]")
{
  const auto row = 8324_row;
  const position position{row, 0_col};

  CHECK(position.get_row() == row);
}

TEST_CASE("map_position::col", "[map_position]")
{
  const auto col = 493_col;
  const position position{0_row, col};

  CHECK(position.get_col() == col);
}

TEST_CASE("map_position::urow", "[map_position]")
{
  const auto row = 6532_row;
  const position position{row, 0_col};

  CHECK(position.urow() == static_cast<std::size_t>(row.get()));
}

TEST_CASE("map_position::ucol", "[map_position]")
{
  const auto col = 18343_col;
  const position position{0_row, col};

  CHECK(position.ucol() == static_cast<std::size_t>(col.get()));
}

TEST_CASE("map_position::north", "[map_position]")
{
  const position fst{7_row, 15_col};
  const position snd = fst.north();

  CHECK(snd.get_row() == fst.get_row() - 1_row);
  CHECK(snd.get_col() == fst.get_col());
}

TEST_CASE("map_position::east", "[map_position]")
{
  const position fst{52_row, 77_col};
  const position snd = fst.east();

  CHECK(snd.get_row() == fst.get_row());
  CHECK(snd.get_col() == fst.get_col() + 1_col);
}

TEST_CASE("map_position::south", "[map_position]")
{
  const position fst{33_row, 6_col};
  const position snd = fst.south();

  CHECK(snd.get_row() == fst.get_row() + 1_row);
  CHECK(snd.get_col() == fst.get_col());
}

TEST_CASE("map_position::west", "[map_position]")
{
  const position fst{62_row, 39_col};
  const position snd = fst.west();

  CHECK(snd.get_row() == fst.get_row());
  CHECK(snd.get_col() == fst.get_col() - 1_col);
}

TEST_CASE("map_position operator==", "[map_position]")
{
  const position pos{45_row, 23_col};
  CHECK(pos == pos);

  const position copy{pos};

  CHECK(pos == copy);
  CHECK(copy == pos);
}

TEST_CASE("map_position operator!=", "[map_position]")
{
  const position pos{45_row, 23_col};
  CHECK(!(pos != pos));

  const position other{839_row, 54_col};
  CHECK(pos != other);
  CHECK(other != pos);
}