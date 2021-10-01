#include "save_map_document_as_json.hpp"

#include <filesystem>  // path, absolute

#include "common/json.hpp"
#include "core/ctx/map.hpp"
#include "io/saving/common_saving.hpp"
#include "save_json.hpp"
#include "save_layers.hpp"
#include "save_properties.hpp"
#include "save_tilesets.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto SaveMap(const entt::registry& registry,
                           const std::filesystem::path& dir) -> JSON
{
  auto json = JSON::object();

  const auto& map = registry.ctx<Map>();

  json["type"] = "map";
  json["width"] = map.column_count;
  json["height"] = map.row_count;
  json["nextlayerid"] = map.next_layer_id.get();
  json["nextobjectid"] = map.next_object_id.get();
  json["tilewidth"] = map.tile_width;
  json["tileheight"] = map.tile_height;

  json["infinite"] = false;
  json["orientation"] = "orthogonal";
  json["renderorder"] = "right-down";
  json["compressionlevel"] = -1;
  json["tiledversion"] = tiled_version;
  json["version"] = tiled_json_version;

  json["tilesets"] = SaveTilesets(registry, dir);
  json["layers"] = SaveLayers(registry, dir);
  json["properties"] = SaveProperties(registry, entt::null, dir);

  return json;
}

}  // namespace

void SaveMapDocumentAsJson(const Document& document)
{
  const auto path = std::filesystem::absolute(document.path);
  const auto dir = path.parent_path();
  const auto json = SaveMap(document.registry, dir);
  SaveJson(json, path);
}

}  // namespace Tactile::IO
