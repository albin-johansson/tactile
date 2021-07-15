#include "save_tilesets.hpp"

#include <algorithm>  // replace
#include <format>     // format
#include <utility>    // move

#include "io/preferences.hpp"
#include "io/saving/common.hpp"
#include "io/saving/json/save_json.hpp"
#include "io/saving/json/save_properties.hpp"

namespace Tactile::IO {
namespace {

void AddCommonAttributes(nlohmann::json& json,
                         const Tileset& tileset,
                         const std::filesystem::path& dir)
{
  json["name"] = tileset.GetName();
  json["columns"] = tileset.GetColumnCount().get();
  json["image"] = GetTilesetImagePath(tileset, dir);
  json["imagewidth"] = tileset.GetWidth();
  json["imageheight"] = tileset.GetHeight();
  json["margin"] = 0;
  json["spacing"] = 0;
  json["tilecount"] = tileset.GetTileCount();
  json["tilewidth"] = tileset.GetTileWidth();
  json["tileheight"] = tileset.GetTileHeight();
  json["properties"] = SaveProperties(tileset, dir);
}

[[nodiscard]] auto SaveEmbeddedTileset(const Tileset& tileset,
                                       const std::filesystem::path& dir)
    -> nlohmann::json
{
  auto json = nlohmann::json::object();

  json["firstgid"] = tileset.GetFirstId().get();
  AddCommonAttributes(json, tileset, dir);

  return json;
}

[[nodiscard]] auto SaveExternalTileset(const Tileset& tileset) -> nlohmann::json
{
  auto json = nlohmann::json::object();

  json["firstgid"] = tileset.GetFirstId().get();
  json["source"] = std::format("{}.json", tileset.GetName());

  return json;
}

void CreateExternalTilesetFile(const Tileset& tileset,
                               const std::filesystem::path& dir)
{
  auto json = nlohmann::json::object();

  AddCommonAttributes(json, tileset, dir);

  json["type"] = "tileset";
  json["tiledversion"] = tiled_version;
  json["version"] = tiled_json_version;

  const auto name = std::format("{}.json", tileset.GetName());
  const auto path = dir / name;

  CENTURION_LOG_INFO("Saving external tileset in \"%s\"", path.string().c_str());
  SaveJson(json, path);
}

[[nodiscard]] auto SaveTileset(const Tileset& tileset,
                               const std::filesystem::path& dir) -> nlohmann::json
{
  if (Prefs::GetEmbedTilesets())
  {
    return SaveEmbeddedTileset(tileset, dir);
  }
  else
  {
    CreateExternalTilesetFile(tileset, dir);
    return SaveExternalTileset(tileset);
  }
}

}  // namespace

[[nodiscard]] auto SaveTilesets(const MapDocument& document,
                                const std::filesystem::path& dir) -> nlohmann::json
{
  auto json = nlohmann::json::array();

  for (const auto& [id, tileset] : document.GetTilesets())
  {
    json += SaveTileset(*tileset, dir);
  }

  return json;
}

}  // namespace Tactile::IO
