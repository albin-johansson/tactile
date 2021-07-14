#include "save_map_document_as_json.hpp"

#include <filesystem>  // path
#include <json.hpp>    // json

#include "io/saving/json/save_json.hpp"
#include "io/saving/json/save_layers.hpp"
#include "io/saving/json/save_properties.hpp"
#include "io/saving/json/save_tilesets.hpp"
#include "io/saving/json/tiled_info.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto SaveMap(const MapDocument& document,
                           const std::filesystem::path& dir) -> nlohmann::json
{
  auto json = nlohmann::json::object();

  const auto& map = document.GetMap();

  json["type"] = "map";
  json["width"] = map.GetColumnCount().get();
  json["height"] = map.GetRowCount().get();
  json["nextlayerid"] = map.GetNextLayerId().get();
  json["nextobjectid"] = map.GetNextObjectId().get();
  // TODO json["tilewidth"]
  // TODO json["tileheight"]

  json["infinite"] = false;
  json["orientation"] = "orthogonal";
  json["renderorder"] = "right-down";
  json["compressionlevel"] = "-1";
  json["tiledversion"] = tiled_version;
  json["version"] = format_version;

  json["tilesets"] = SaveTilesets(document, dir);
  json["layers"] = SaveLayers(document.GetMap(), dir);
  json["properties"] = SaveProperties(document, dir);

  return json;
}

}  // namespace

void SaveMapDocumentAsJson(const MapDocument& document)
{
  const auto path = document.GetAbsolutePath();
  const auto dir = path.parent_path();

  const auto json = SaveMap(document, dir);
  SaveJson(json, path);
}

}  // namespace Tactile::IO
