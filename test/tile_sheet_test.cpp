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
    //    SharedPtr<QImage> image = nullptr;
    //    CHECK_THROWS_AS(TileSheet(nullptr, 10), std::invalid_argument);
  }

  SECTION("Zero size")
  {
    //    auto image = std::make_shared<QImage>("terrain.png");
    //    CHECK_NOTHROW(TileSheet{image, 0});
  }

  SECTION("Good args")
  {
    //    auto image = std::make_shared<QImage>("terrain.png");
    //    CHECK_NOTHROW(TileSheet{image, 32});
  }
}

TEST_CASE("TileSheet::set_first_id", "[TileSheet]")
{
  //  auto image = std::make_shared<QImage>("terrain.png");
//  auto image = std::make_shared<MockImage>();
//  const auto size = 32;
//
//  TileSheet sheet{image, size};
  //
  //  CHECK(sheet.first_id() == 1);

  //  const auto id = 84;
  //  sheet.set_first_id(id);
  //
  //  CHECK(sheet.first_id() == id);
  //
  //  sheet.set_first_id(0);  // invalid
  //  CHECK(sheet.first_id() == id);
  //
  //  sheet.set_first_id(-1);  // invalid
  //  CHECK(sheet.first_id() == id);
}
