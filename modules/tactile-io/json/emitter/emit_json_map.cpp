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

[[nodiscard]] auto SaveMap(const MapData& data,
                           const std::filesystem::path& dir,
                           const EmitterOptions& options) -> JSON
{
  auto json = JSON::object();

  json["type"] = "map";
  json["width"] = data.column_count;
  json["height"] = data.row_count;
  json["nextlayerid"] = data.next_layer_id;
  json["nextobjectid"] = data.next_object_id;
  json["tilewidth"] = data.tile_width;
  json["tileheight"] = data.tile_height;

  json["infinite"] = false;
  json["orientation"] = "orthogonal";
  json["renderorder"] = "right-down";
  json["compressionlevel"] = -1;
  json["tiledversion"] = tiled_version;
  json["version"] = tiled_json_version;

  json["tilesets"] = SaveTilesets(data.tilesets, dir, options);
  json["layers"] = SaveLayers(data.layers, dir);
  json["properties"] = SaveProperties(data.properties, dir);

  return json;
}

}  // namespace

void EmitJsonMap(const MapData& data, const EmitterOptions& options)
{
  const auto path = data.absolute_path;
  const auto dir = path.parent_path();
  const auto json = SaveMap(data, dir, options);
  SaveJson(json, path, options.human_readable_output);
}

}  // namespace Tactile::IO
