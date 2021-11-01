#include <filesystem>  // path, absolute

#include "../json_common.hpp"
#include "common_saving.hpp"
#include "emitter.hpp"
#include "save_json.hpp"
#include "save_layers.hpp"
#include "save_properties.hpp"
#include "save_tilesets.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto SaveMap(const Map& map,
                           const std::filesystem::path& dir,
                           const EmitterOptions& options) -> JSON
{
  auto json = JSON::object();

  json["type"] = "map";
  json["width"] = GetColumnCount(map);
  json["height"] = GetRowCount(map);
  json["nextlayerid"] = GetNextLayerId(map);
  json["nextobjectid"] = GetNextObjectId(map);
  json["tilewidth"] = GetTileWidth(map);
  json["tileheight"] = GetTileHeight(map);

  json["infinite"] = false;
  json["orientation"] = "orthogonal";
  json["renderorder"] = "right-down";
  json["compressionlevel"] = -1;
  json["tiledversion"] = tiled_version;
  json["version"] = tiled_json_version;

  json["tilesets"] = SaveTilesets(map, dir, options);
  json["layers"] = SaveLayers(map, dir);
  json["properties"] = SaveProperties(map, dir);

  return json;
}

}  // namespace

void EmitJsonMap(const Map& map, const EmitterOptions& options)
{
  const std::filesystem::path path = GetPath(map);
  const auto dir = path.parent_path();

  const auto json = SaveMap(map, dir, options);
  SaveJson(json, path, options.human_readable_output);
}

}  // namespace Tactile::IO
