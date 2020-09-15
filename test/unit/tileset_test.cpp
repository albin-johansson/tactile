#include "tileset.hpp"

#include <catch.hpp>
#include <memory>

#include "tactile_error.hpp"
#include "types.hpp"

using namespace tactile;
using namespace tactile::core;

TEST_CASE("tileset(tile_id, QImage, tile_width, tile_height)", "[tileset]")
{
  SECTION("Null image")
  {
    QImage image;
    CHECK_THROWS_AS(tileset(1_t, image, 10_tw, 10_th), tactile_error);
  }

  SECTION("Zero size")
  {
    const tileset tileset{1_t, "terrain.png", 0_tw, 0_th};
    CHECK(tileset.get_tile_width() == 1_tw);
    CHECK(tileset.get_tile_height() == 1_th);
  }

  SECTION("Good args")
  {
    CHECK_NOTHROW(tileset{1_t, "terrain.png", 32_tw, 32_th});
  }
}

TEST_CASE("tileset::width", "[tileset]")
{
  const QImage image{"terrain.png"};
  tileset sheet{1_t, image, 32_tw, 32_th};

  CHECK(sheet.width() == image.width());
}

TEST_CASE("tileset::height", "[tileset]")
{
  const QImage image{"terrain.png"};
  tileset sheet{1_t, image, 32_tw, 32_th};

  CHECK(sheet.height() == image.height());
}

TEST_CASE("tileset::tiles", "[tileset]")
{
  const QImage image{"terrain.png"};
  tileset sheet{1_t, image, 32_tw, 32_th};

  CHECK(sheet.tile_count() == 1024);
}

TEST_CASE("tileset::last_id", "[tileset]")
{
  tileset sheet{1_t, "terrain.png", 32_tw, 32_th};

  CHECK(sheet.last_id() == 1025_t);
  CHECK(sheet.last_id() - sheet.first_id() == tile_id{sheet.tile_count()});
}

TEST_CASE("tileset::contains", "[tileset]")
{
  tileset sheet{1_t, "terrain.png", 32_tw, 32_th};

  CHECK(sheet.contains(sheet.first_id()));
  CHECK(sheet.contains(sheet.last_id()));
  CHECK(!sheet.contains(sheet.first_id() - tile_id{1}));
  CHECK(!sheet.contains(sheet.last_id() + tile_id{1}));
}

TEST_CASE("tileset::tile_at", "[tileset]")
{
  SECTION("Outside of the tile sheet area")
  {
    tileset sheet{1_t, "terrain.png", 32_tw, 32_th};

    CHECK(sheet.tile_at({row_t{sheet.width() + 1}, 0_col}) == empty);
    CHECK(sheet.tile_at({0_row, col_t{sheet.height() + 1}}) == empty);
    CHECK(sheet.tile_at(
              {row_t{sheet.width() + 1}, col_t{sheet.height() + 1}}) == empty);
  }

  SECTION("Valid ID")
  {
    tileset sheet{1_t, "terrain.png", 32_tw, 32_th};

    const auto row = 7_row;
    const auto col = 5_col;
    const tile_id index{row.get() * sheet.cols().get() + col.get()};
    CHECK(sheet.tile_at({row, col}) == sheet.first_id() + index);
  }
}

TEST_CASE("tileset::tile_width", "[tileset]")
{
  SECTION("Good size")
  {
    tileset sheet{1_t, "terrain.png", 32_tw, 15_th};
    CHECK(sheet.get_tile_width() == 32_tw);
  }

  SECTION("Clamping bad size")
  {
    tileset sheet{1_t, "terrain.png", 0_tw, 15_th};
    CHECK(sheet.get_tile_width() == 1_tw);
  }
}

TEST_CASE("tileset::tile_height", "[tileset]")
{
  SECTION("Good size")
  {
    tileset sheet{1_t, "terrain.png", 15_tw, 32_th};
    CHECK(sheet.get_tile_height() == 32_th);
  }

  SECTION("Clamping bad size")
  {
    tileset sheet{1_t, "terrain.png", 32_tw, 0_th};
    CHECK(sheet.get_tile_height() == 1_th);
  }
}
