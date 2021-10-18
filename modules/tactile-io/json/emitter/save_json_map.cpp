#include <filesystem>  // path, absolute

#include "../json_common.hpp"
#include "common_saving.hpp"
#include "save_json.hpp"
#include "save_layers.hpp"
#include "save_properties.hpp"
#include "save_tilesets.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto SaveMap(const MapData& data, const std::filesystem::path& dir) -> JSON
{
  auto json = JSON::object();

  json["type"] = "map";
  json["width"] = data.column_count;
  json["height"] = data.row_count;
  json["nextlayerid"] = data.next_layer_id.get();
  json["nextobjectid"] = data.next_object_id.get();
  json["tilewidth"] = data.tile_width;
  json["tileheight"] = data.tile_height;

  json["infinite"] = false;
  json["orientation"] = "orthogonal";
  json["renderorder"] = "right-down";
  json["compressionlevel"] = -1;
  json["tiledversion"] = tiled_version;
  json["version"] = tiled_json_version;

  json["tilesets"] = SaveTilesets(data.tilesets, dir, true); // TODO pass option
  json["layers"] = SaveLayers(data.layers, dir);
  json["properties"] = SaveProperties(data.properties, dir);

  return json;
}

}  // namespace

void EmitJsonMap(const MapData& data)
{
  const auto path = data.absolute_path;
  const auto dir = path.parent_path();
  const auto json = SaveMap(data, dir);
  SaveJson(json, path);
}

}  // namespace Tactile::IO
