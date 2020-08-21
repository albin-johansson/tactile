#include <catch.hpp>
#include <memory>

#include "tactile_types.hpp"
#include "tileset.hpp"
#include "tileset_model.hpp"

using tactile::tileset_id;
using namespace tactile::model;

TEST_CASE("tileset_manager::emplace", "[tileset_manager]")
{
  tileset_model manager;

  CHECK(manager.sheets() == 0);
  CHECK(!manager.has_active_tileset());

  const auto first = manager.emplace("terrain.png", 32, 32);
  const auto second = manager.emplace("terrain.png", 32, 32);

  CHECK(second > first);

  CHECK(manager.sheets() == 2);
}

TEST_CASE("tileset_manager::remove", "[tileset_manager]")
{
  tileset_model manager;
  CHECK_NOTHROW(manager.remove(tileset_id{0}));

  const auto id = manager.emplace("terrain.png", 32, 32);
  CHECK(manager.sheets() == 1);

  manager.remove(id + tileset_id{5});
  CHECK(manager.sheets() == 1);

  manager.remove(id);
  CHECK(manager.sheets() == 0);
}

TEST_CASE("tileset_manager::remove_all", "[tileset_manager]")
{
  tileset_model manager;
  CHECK_NOTHROW(manager.remove_all());

  manager.emplace("terrain.png", 32, 32);
  manager.emplace("terrain.png", 32, 32);

  CHECK(manager.sheets() == 2);

  manager.remove_all();
  CHECK(manager.sheets() == 0);
}

TEST_CASE("tileset_manager::select", "[tileset_manager]")
{
  tileset_model manager;

  CHECK_NOTHROW(manager.select(tileset_id{9}));
  CHECK(!manager.has_active_tileset());

  const auto id = manager.emplace("terrain.png", 32, 32);
  manager.emplace("terrain.png", 32, 32);

  manager.select(id);
  CHECK(manager.has_active_tileset());

  manager.select(std::nullopt);
  CHECK(!manager.has_active_tileset());
}