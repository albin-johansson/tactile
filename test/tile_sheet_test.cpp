#include "tile_sheet.h"

#include <QImage>
#include <catch.hpp>
#include <memory>
#include <stdexcept>

using namespace tactile;

TEST_CASE("TileSheet(const SharedPtr<QImage>&, int)", "[TileSheet]")
{
  SECTION("Null image")
  {
    Shared<QImage> image = nullptr;
    CHECK_THROWS_AS(TileSheet(nullptr, 10), std::invalid_argument);
  }

  SECTION("Zero size")
  {
    auto image = std::make_shared<QImage>("terrain.png");
    CHECK_NOTHROW(TileSheet{image, 0});
  }

  SECTION("Good args")
  {
    auto image = std::make_shared<QImage>("terrain.png");
    CHECK_NOTHROW(TileSheet{image, 32});
  }
}

TEST_CASE("TileSheet::select", "[TileSheet]")
{
  auto image = std::make_shared<QImage>("terrain.png");
  const auto size = 32;
  TileSheet sheet{image, size};

  sheet.select(0, 0);
  CHECK(sheet.selection().size() == 1);

  sheet.select(0, 0);
  CHECK(sheet.selection().size() == 1);

  sheet.select(3 * size, 2 * size);
  CHECK(sheet.selection().size() == 2);

  sheet.select(-1, -1);
  CHECK(sheet.selection().size() == 2);

  sheet.clear_selection();
  CHECK(sheet.selection().empty());
}

TEST_CASE("TileSheet::set_first_id", "[TileSheet]")
{
  auto image = std::make_shared<QImage>("terrain.png");
  const auto size = 32;

  TileSheet sheet{image, size};

  CHECK(sheet.first_id() == 1);

  const auto id = 84;
  sheet.set_first_id(id);

  CHECK(sheet.first_id() == id);

  sheet.set_first_id(0);  // invalid
  CHECK(sheet.first_id() == id);

  sheet.set_first_id(-1);  // invalid
  CHECK(sheet.first_id() == id);
}

TEST_CASE("TileSheet::width", "[TileSheet]")
{
  auto image = std::make_shared<QImage>("terrain.png");
  TileSheet sheet{image, 32};

  CHECK(sheet.width() == image->width());
}

TEST_CASE("TileSheet::height", "[TileSheet]")
{
  auto image = std::make_shared<QImage>("outside.png");
  TileSheet sheet{image, 32};

  CHECK(sheet.height() == image->height());
}

TEST_CASE("TileSheet::tiles", "[TileSheet]")
{
  auto image = std::make_shared<QImage>("outside.png");
  TileSheet sheet{image, 32};

  CHECK(sheet.tiles() == 1024);
}

TEST_CASE("TileSheet::last_id", "[TileSheet]")
{
  auto image = std::make_shared<QImage>("outside.png");
  TileSheet sheet{image, 32};

  CHECK(sheet.last_id() == 1025);
  CHECK(sheet.last_id() - sheet.first_id() == sheet.tiles());

  sheet.set_first_id(43);
  CHECK(sheet.last_id() - sheet.first_id() == sheet.tiles());
}

TEST_CASE("TileSheet::contains", "[TileSheet]")
{
  auto image = std::make_shared<QImage>("terrain.png");
  TileSheet sheet{image, 32};

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

TEST_CASE("TileSheet::tile_at", "[TileSheet]")
{
  SECTION("Outside of the tile sheet area")
  {
    auto image = std::make_shared<QImage>("terrain.png");
    TileSheet sheet{image, 32};

    CHECK(sheet.tile_at(-1, -1) == empty);
    CHECK(sheet.tile_at(sheet.width() + 1, 0) == empty);
    CHECK(sheet.tile_at(0, sheet.height() + 1) == empty);
    CHECK(sheet.tile_at(sheet.width() + 1, sheet.height() + 1) == empty);
  }

  SECTION("Without changed first ID")
  {
    auto image = std::make_shared<QImage>("terrain.png");
    TileSheet sheet{image, 32};

    const auto row = 7;
    const auto col = 5;
    const auto x = col * sheet.tile_size() + 13;
    const auto y = row * sheet.tile_size() + 24;

    const auto index = row * sheet.cols() + col;
    CHECK(sheet.tile_at(x, y) == sheet.first_id() + index);
  }

  SECTION("With changed first ID")
  {
    auto image = std::make_shared<QImage>("terrain.png");
    TileSheet sheet{image, 32};

    const auto first = 38;
    sheet.set_first_id(first);

    const auto row = 9;
    const auto col = 4;
    const auto x = col * sheet.tile_size();
    const auto y = row * sheet.tile_size();

    const auto index = row * sheet.cols() + col;
    CHECK(sheet.tile_at(x, y) == sheet.first_id() + index);
  }
}

TEST_CASE("TileSheet::tile_size", "[TileSheet]")
{
  SECTION("Good size")
  {
    auto image = std::make_shared<QImage>("terrain.png");
    TileSheet sheet{image, 32};
    CHECK(sheet.tile_size() == 32);
  }

  SECTION("Clamping bad size")
  {
    auto image = std::make_shared<QImage>("terrain.png");
    TileSheet sheet{image, 0};
    CHECK(sheet.tile_size() == 1);
  }
}

TEST_CASE("TileSheet::selection", "[TileSheet]")
{
  // TODO
}