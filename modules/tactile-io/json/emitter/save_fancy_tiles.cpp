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

[[nodiscard]] auto SaveAnimation(const Tile& tile, const usize nFrames) -> JSON
{
  auto array = JSON::array();

  for (usize index = 0; index < nFrames; ++index) {
    const auto& frame = GetAnimationFrame(tile, index);
    array += SaveFrame(frame);
  }

  return array;
}

[[nodiscard]] auto SaveFancyTile(const Tile& tile, const std::filesystem::path& dir)
    -> JSON
{
  auto json = JSON::object();
  json["id"] = GetId(tile);

  if (const auto nFrames = GetAnimationFrameCount(tile); nFrames != 0) {
    json["animation"] = SaveAnimation(tile, nFrames);
  }

  if (const auto nObjects = GetObjectCount(tile); nObjects != 0) {
    auto dummy = JSON::object();
    dummy["draworder"] = "index";
    dummy["name"] = "";
    dummy["opacity"] = 1;
    dummy["type"] = "objectgroup";
    dummy["visible"] = true;
    dummy["x"] = 0;
    dummy["y"] = 0;

    auto objects = JSON::array();
    for (usize index = 0; index < nObjects; ++index) {
      const auto& object = GetObject(tile, index);
      objects += SaveObject(object, dir);
    }
    dummy["objects"] = std::move(objects);

    json["objectgroup"] = std::move(dummy);
  }

  if (const auto nProps = GetPropertyCount(tile); nProps != 0) {
    json["properties"] = SaveProperties(tile, dir);
  }

  return json;
}

}  // namespace

auto SaveFancyTiles(const Tileset& tileset, const std::filesystem::path& dir) -> JSON
{
  auto array = JSON::array();

  const auto count = GetTileInfoCount(tileset);
  for (usize index = 0; index < count; ++index) {
    const auto& tile = GetTileInfo(tileset, index);
    if (IsTileWorthSaving(tile)) {
      array += SaveFancyTile(tile, dir);
    }
  }

  return array;
}

}  // namespace Tactile::IO
