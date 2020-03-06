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
    SharedPtr<QImage> image = nullptr;
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
  QImage i{""};

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