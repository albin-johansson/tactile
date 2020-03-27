#include "tile_sheet_manager.h"

#include <QImage>
#include <catch.hpp>

#include "tile_sheet.h"

using namespace tactile;

namespace {

UniquePtr<TileSheet> create_tile_sheet()
{
  auto image = std::make_shared<QImage>("terrain.png");
  return std::make_unique<TileSheet>(image, 32);
}

}  // namespace

TEST_CASE("TileSheetManager::add", "[TileSheetManager]")
{
  TileSheetManager manager;

  CHECK_NOTHROW(manager.add(0, nullptr));
  CHECK(manager.sheets() == 0);
  CHECK(!manager.has_active_tile_sheet());

  manager.add(7, create_tile_sheet());
  CHECK_NOTHROW(manager.add(7, create_tile_sheet()));

  CHECK(manager.sheets() == 1);
}

TEST_CASE("TileSheetManager::remove", "[TileSheetManager]")
{
  TileSheetManager manager;
  CHECK_NOTHROW(manager.remove(0));

  manager.add(5, create_tile_sheet());
  CHECK(manager.sheets() == 1);

  manager.remove(4);
  CHECK(manager.sheets() == 1);

  manager.remove(5);
  CHECK(manager.sheets() == 0);
}

TEST_CASE("TileSheetManager::remove_all", "[TileSheetManager]")
{
  TileSheetManager manager;
  CHECK_NOTHROW(manager.remove_all());

  manager.add(3, create_tile_sheet());
  manager.add(6, create_tile_sheet());

  CHECK(manager.sheets() == 2);

  manager.remove_all();
  CHECK(manager.sheets() == 0);
}

TEST_CASE("TileSheetManager::select", "[TileSheetManager]")
{
  TileSheetManager manager;

  CHECK_NOTHROW(manager.select(9));
  CHECK(!manager.has_active_tile_sheet());

  manager.add(3, create_tile_sheet());
  manager.add(8, create_tile_sheet());

  manager.select(3);
  CHECK(manager.has_active_tile_sheet());

  manager.select(nothing);
  CHECK(!manager.has_active_tile_sheet());
}