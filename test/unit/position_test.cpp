#include "position.hpp"

#include <catch.hpp>

using namespace tactile;
using core::operator""_row;
using core::operator""_col;

TEST_CASE("position()", "[position]")
{
  const core::position position;
  CHECK(position.row() == 0_row);
  CHECK(position.col() == 0_col);
}

TEST_CASE("position(row_t, col_t)", "[position]")
{
  SECTION("Valid arguments")
  {
    const auto row = 123_row;
    const auto col = 557_col;
    const core::position position{row, col};
    CHECK(position.row() == row);
    CHECK(position.col() == col);
  }

  SECTION("Edge-case arguments")
  {
    const core::position valid1{0_row, 0_col};
    CHECK(valid1.row() == 0_row);
    CHECK(valid1.col() == 0_col);

    const core::position invalid1{-1_row, 0_col};
    CHECK(invalid1.row() == 0_row);

    const core::position invalid2{0_row, -1_col};
    CHECK(invalid2.col() == 0_col);
  }
}

TEST_CASE("position::set_row", "[position]")
{
  core::position position;

  const auto row = 892_row;
  position.set_row(row);

  CHECK(position.row() == row);
}

TEST_CASE("position::set_col", "[position]")
{
  core::position position;

  const auto col = 438291_col;
  position.set_col(col);

  CHECK(position.col() == col);
}

TEST_CASE("position::offset_by(row_t, col_t)", "[position]")
{
  const core::position position;
  const auto result = position.offset_by(4_row, 6_col);
  CHECK(result.row() == 4_row);
  CHECK(result.col() == 6_col);
}

TEST_CASE("position::offset_by(position)", "[position]")
{
  const core::position fst{2_row, 3_col};
  const core::position snd{6_row, 4_col};

  const auto result = fst.offset_by(snd);
  CHECK(result.row() == fst.row() + snd.row());
  CHECK(result.col() == fst.col() + snd.col());
}

TEST_CASE("position::row", "[position]")
{
  const auto row = 8324_row;
  const core::position position{row, 0_col};

  CHECK(position.row() == row);
}

TEST_CASE("position::col", "[position]")
{
  const auto col = 493_col;
  const core::position position{0_row, col};

  CHECK(position.col() == col);
}

TEST_CASE("position::row_index", "[position]")
{
  const auto row = 6532_row;
  const core::position position{row, 0_col};

  CHECK(position.row_index() == static_cast<std::size_t>(row.get()));
}

TEST_CASE("position::col_index", "[position]")
{
  const auto col = 18343_col;
  const core::position position{0_row, col};

  CHECK(position.col_index() == static_cast<std::size_t>(col.get()));
}

TEST_CASE("position::row_to_x", "[position]")
{
  const auto tileSize = 134;
  const core::position position{12_row, 34_col};
  CHECK(position.row_to_y(tileSize) == position.row().get() * tileSize);
}

TEST_CASE("position::col_to_x", "[position]")
{
  const auto tileSize = 68;
  const core::position position{12_row, 34_col};
  CHECK(position.col_to_x(tileSize) == position.col().get() * tileSize);
}

TEST_CASE("position::unpack", "[position]")
{
  const core::position position{32_row, 65_col};
  const auto [row, col] = position.unpack();
  CHECK(position.row() == row);
  CHECK(position.col() == col);
}

TEST_CASE("position::north", "[position]")
{
  const core::position fst{7_row, 15_col};
  const core::position snd = fst.north();

  CHECK(snd.row() == fst.row() - 1_row);
  CHECK(snd.col() == fst.col());
}

TEST_CASE("position::east", "[position]")
{
  const core::position fst{52_row, 77_col};
  const core::position snd = fst.east();

  CHECK(snd.row() == fst.row());
  CHECK(snd.col() == fst.col() + 1_col);
}

TEST_CASE("position::south", "[position]")
{
  const core::position fst{33_row, 6_col};
  const core::position snd = fst.south();

  CHECK(snd.row() == fst.row() + 1_row);
  CHECK(snd.col() == fst.col());
}

TEST_CASE("position::west", "[position]")
{
  const core::position fst{62_row, 39_col};
  const core::position snd = fst.west();

  CHECK(snd.row() == fst.row());
  CHECK(snd.col() == fst.col() - 1_col);
}

TEST_CASE("position operator==", "[position]")
{
  const core::position pos{45_row, 23_col};
  CHECK(pos == pos);

  const core::position copy{pos};

  CHECK(pos == copy);
  CHECK(copy == pos);
}

TEST_CASE("position operator!=", "[position]")
{
  const core::position pos{45_row, 23_col};
  CHECK(!(pos != pos));

  const core::position other{839_row, 54_col};
  CHECK(pos != other);
  CHECK(other != pos);
}