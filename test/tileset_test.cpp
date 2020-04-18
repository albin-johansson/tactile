#include <QImage>
#include <catch.hpp>
#include <memory>

#include "core/tileset/tileset.h"
#include "tactile_types.h"

using namespace tactile;

TEST_CASE("Tileset(const SharedPtr<QImage>&, int, int)", "[Tileset]")
{
  SECTION("Null image")
  {
    Shared<QImage> image = nullptr;
    CHECK_THROWS_AS(Tileset(nullptr, 10, 10), BadArg);
  }

  SECTION("Zero size")
  {
    auto image = std::make_shared<QImage>("terrain.png");
    CHECK_NOTHROW(Tileset{image, 0, 0});
  }

  SECTION("Good args")
  {
    auto image = std::make_shared<QImage>("terrain.png");
    CHECK_NOTHROW(Tileset{image, 32, 32});
  }
}

TEST_CASE("Tileset::unique", "[Tileset]")
{
  CHECK_THROWS_AS(Tileset::unique(nullptr, 32, 32), BadArg);

  auto image = std::make_shared<QImage>("terrain.png");
  CHECK(Tileset::unique(image, 32, 32));
}

TEST_CASE("Tileset::shared", "[Tileset]")
{
  CHECK_THROWS_AS(Tileset::shared(nullptr, 32, 32), BadArg);

  auto image = std::make_shared<QImage>("terrain.png");
  CHECK(Tileset::shared(image, 32, 32));
}

TEST_CASE("Tileset::select", "[Tileset]")
{
  auto image = std::make_shared<QImage>("terrain.png");
  const auto tileWidth = 32;
  const auto tileHeight = 32;
  Tileset sheet{image, tileWidth, tileHeight};

  sheet.select(0, 0);
  CHECK(sheet.selection().size() == 1);

  sheet.select(0, 0);
  CHECK(sheet.selection().size() == 1);

  sheet.select(3 * tileWidth, 2 * tileHeight);
  CHECK(sheet.selection().size() == 2);

  sheet.select(-1, -1);
  CHECK(sheet.selection().size() == 2);

  sheet.clear_selection();
  CHECK(sheet.selection().empty());
}

TEST_CASE("Tileset::set_first_id", "[Tileset]")
{
  auto image = std::make_shared<QImage>("terrain.png");
  Tileset sheet{image, 32, 32};

  CHECK(sheet.first_id() == 1);

  const auto id = 84;
  sheet.set_first_id(id);

  CHECK(sheet.first_id() == id);

  sheet.set_first_id(0);  // invalid
  CHECK(sheet.first_id() == id);

  sheet.set_first_id(-1);  // invalid
  CHECK(sheet.first_id() == id);
}

TEST_CASE("Tileset::width", "[Tileset]")
{
  auto image = std::make_shared<QImage>("terrain.png");
  Tileset sheet{image, 32, 32};

  CHECK(sheet.width() == image->width());
}

TEST_CASE("Tileset::height", "[Tileset]")
{
  auto image = std::make_shared<QImage>("outside.png");
  Tileset sheet{image, 32, 32};

  CHECK(sheet.height() == image->height());
}

TEST_CASE("Tileset::tiles", "[Tileset]")
{
  auto image = std::make_shared<QImage>("outside.png");
  Tileset sheet{image, 32, 32};

  CHECK(sheet.tiles() == 1024);
}

TEST_CASE("Tileset::last_id", "[Tileset]")
{
  auto image = std::make_shared<QImage>("outside.png");
  Tileset sheet{image, 32, 32};

  CHECK(sheet.last_id() == 1025);
  CHECK(sheet.last_id() - sheet.first_id() == sheet.tiles());

  sheet.set_first_id(43);
  CHECK(sheet.last_id() - sheet.first_id() == sheet.tiles());
}

TEST_CASE("Tileset::contains", "[Tileset]")
{
  auto image = std::make_shared<QImage>("terrain.png");
  Tileset sheet{image, 32, 32};

  CHECK(sheet.contains(sheet.first_id()));
  CHECK(sheet.contains(sheet.last_id()));
  CHECK(!sheet.contains(sheet.first_id() - 1));
  CHECK(!sheet.contains(sheet.last_id() + 1));

  const auto id = 82;
  sheet.set_first_id(id);
  CHECK(sheet.contains(id));
  CHECK(sheet.contains(id + 5));
  CHECK(sheet.contains(sheet.last_id()));
}

TEST_CASE("Tileset::tile_at", "[Tileset]")
{
  SECTION("Outside of the tile sheet area")
  {
    auto image = std::make_shared<QImage>("terrain.png");
    Tileset sheet{image, 32, 32};

    CHECK(sheet.tile_at(-1, -1) == empty);
    CHECK(sheet.tile_at(sheet.width() + 1, 0) == empty);
    CHECK(sheet.tile_at(0, sheet.height() + 1) == empty);
    CHECK(sheet.tile_at(sheet.width() + 1, sheet.height() + 1) == empty);
  }

  SECTION("Without changed first ID")
  {
    auto image = std::make_shared<QImage>("terrain.png");
    Tileset sheet{image, 32, 32};

    const auto row = 7;
    const auto col = 5;
    const auto x = col * sheet.tile_width() + 13;
    const auto y = row * sheet.tile_height() + 24;

    const auto index = row * sheet.cols() + col;
    CHECK(sheet.tile_at(x, y) == sheet.first_id() + index);
  }

  SECTION("With changed first ID")
  {
    auto image = std::make_shared<QImage>("terrain.png");
    Tileset sheet{image, 32, 32};

    const auto first = 38;
    sheet.set_first_id(first);

    const auto row = 9;
    const auto col = 4;
    const auto x = col * sheet.tile_width();
    const auto y = row * sheet.tile_height();

    const auto index = row * sheet.cols() + col;
    CHECK(sheet.tile_at(x, y) == sheet.first_id() + index);
  }
}

TEST_CASE("Tileset::tile_width", "[Tileset]")
{
  SECTION("Good size")
  {
    auto image = std::make_shared<QImage>("terrain.png");
    Tileset sheet{image, 32, 15};
    CHECK(sheet.tile_width() == 32);
  }

  SECTION("Clamping bad size")
  {
    auto image = std::make_shared<QImage>("terrain.png");
    Tileset sheet{image, 0, 15};
    CHECK(sheet.tile_width() == 1);
  }
}

TEST_CASE("Tileset::tile_height", "[Tileset]")
{
  SECTION("Good size")
  {
    auto image = std::make_shared<QImage>("terrain.png");
    Tileset sheet{image, 15, 32};
    CHECK(sheet.tile_height() == 32);
  }

  SECTION("Clamping bad size")
  {
    auto image = std::make_shared<QImage>("terrain.png");
    Tileset sheet{image, 32, 0};
    CHECK(sheet.tile_height() == 1);
  }
}

TEST_CASE("Tileset::selection", "[Tileset]")
{
  // TODO
}