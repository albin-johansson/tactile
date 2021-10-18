#include "save_fancy_tiles.hpp"

#include <vector>  // vector

#include "common_saving.hpp"
#include "save_properties.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto SaveFrame(const FrameData& frame) -> JSON
{
  auto json = JSON::object();

  json["tileid"] = frame.tile.get();
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
  json["id"] = tile.id.get();

  if (!tile.animation.empty()) {
    json["animation"] = SaveAnimation(tile.animation);
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
    if (tile.id >= tileset.first_id &&
        tile.id <= tileset.first_id + TileID{tileset.tile_count})
    {
      if (IsTileWorthSaving(tile)) {
        array += SaveFancyTile(tile, dir);
      }
    }
  }

  return array;
}

}  // namespace Tactile::IO
