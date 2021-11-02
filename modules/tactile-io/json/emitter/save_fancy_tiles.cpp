#include "save_fancy_tiles.hpp"

#include <utility>  // move
#include <vector>   // vector

#include "common_saving.hpp"
#include "save_object.hpp"
#include "save_properties.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto SaveFrame(const AnimationFrame& frame) -> JSON
{
  auto json = JSON::object();

  json["tileid"] = GetTile(frame);
  json["duration"] = GetDuration(frame);

  return json;
}

[[nodiscard]] auto SaveAnimation(const Tile& tile) -> JSON
{
  auto array = JSON::array();

  EachAnimationFrame(tile,
                     [&](const AnimationFrame& frame) { array += SaveFrame(frame); });

  return array;
}

[[nodiscard]] auto SaveFancyTile(const Tile& tile, const std::filesystem::path& dir)
    -> JSON
{
  auto json = JSON::object();
  json["id"] = GetId(tile);

  if (GetAnimationFrameCount(tile) != 0) {
    json["animation"] = SaveAnimation(tile);
  }

  if (GetObjectCount(tile) != 0) {
    auto dummy = JSON::object();
    dummy["draworder"] = "index";
    dummy["name"] = "";
    dummy["opacity"] = 1;
    dummy["type"] = "objectgroup";
    dummy["visible"] = true;
    dummy["x"] = 0;
    dummy["y"] = 0;

    auto objects = JSON::array();
    EachObject(tile, [&](const Object& object) { objects += SaveObject(object, dir); });

    dummy["objects"] = std::move(objects);
    json["objectgroup"] = std::move(dummy);
  }

  if (GetPropertyCount(tile) != 0) {
    json["properties"] = SaveProperties(tile, dir);
  }

  return json;
}

}  // namespace

auto SaveFancyTiles(const Tileset& tileset, const std::filesystem::path& dir) -> JSON
{
  auto array = JSON::array();

  EachTileInfo(tileset, [&](const Tile& tile) {
    if (IsWorthSaving(tile)) {
      array += SaveFancyTile(tile, dir);
    }
  });

  return array;
}

}  // namespace Tactile::IO
