#include "save_fancy_tiles.hpp"

#include <utility>  // move
#include <vector>   // vector

#include "common_saving.hpp"
#include "save_object.hpp"
#include "save_properties.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto SaveFrame(const FrameData& frame) -> JSON
{
  auto json = JSON::object();

  json["tileid"] = frame.tile;
  json["duration"] = frame.duration;

  return json;
}

[[nodiscard]] auto SaveAnimation(const std::vector<FrameData>& animation) -> JSON
{
  auto array = JSON::array();

  for (const auto& frame : animation) {
    array += SaveFrame(frame);
  }

  return array;
}

[[nodiscard]] auto SaveFancyTile(const TileData& tile, const std::filesystem::path& dir)
    -> JSON
{
  auto json = JSON::object();
  json["id"] = tile.id;

  if (!tile.animation.empty()) {
    json["animation"] = SaveAnimation(tile.animation);
  }

  if (!tile.objects.empty()) {
    auto dummy = JSON::object();
    dummy["draworder"] = "index";
    dummy["name"] = "";
    dummy["opacity"] = 1;
    dummy["type"] = "objectgroup";
    dummy["visible"] = true;
    dummy["x"] = 0;
    dummy["y"] = 0;

    auto objects = JSON::array();
    for (const auto& object : tile.objects) {
      objects += SaveObject(object, dir);
    }
    dummy["objects"] = std::move(objects);

    json["objectgroup"] = std::move(dummy);
  }

  if (!tile.properties.empty()) {
    json["properties"] = SaveProperties(tile.properties, dir);
  }

  return json;
}

}  // namespace

auto SaveFancyTiles(const TilesetData& tileset, const std::filesystem::path& dir) -> JSON
{
  auto array = JSON::array();

  for (const auto& tile : tileset.tiles) {
    if (IsTileWorthSaving(tile)) {
      array += SaveFancyTile(tile, dir);
    }
  }

  return array;
}

}  // namespace Tactile::IO
