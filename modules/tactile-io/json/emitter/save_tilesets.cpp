#include "save_tilesets.hpp"

#include <format>   // format
#include <utility>  // move

#include "common_saving.hpp"
#include "save_fancy_tiles.hpp"
#include "save_json.hpp"
#include "save_properties.hpp"

namespace Tactile::IO {
namespace {

void AddCommonAttributes(JSON& json,
                         const TilesetData& tileset,
                         const std::filesystem::path& dir)
{
  json["name"] = tileset.name;
  json["columns"] = tileset.column_count;
  json["image"] = GetTilesetImagePath(tileset.absolute_image_path, dir);  // TODO check
  json["imagewidth"] = tileset.image_width;
  json["imageheight"] = tileset.image_height;
  json["margin"] = 0;
  json["spacing"] = 0;
  json["tilecount"] = tileset.tile_count;
  json["tilewidth"] = tileset.tile_width;
  json["tileheight"] = tileset.tile_height;
  json["tiles"] = SaveFancyTiles(tileset, dir);

  if (!tileset.properties.empty()) {
    json["properties"] = SaveProperties(tileset.properties, dir);
  }
}

[[nodiscard]] auto SaveEmbeddedTileset(const TilesetData& tileset,
                                       const std::filesystem::path& dir) -> JSON
{
  auto json = JSON::object();

  json["firstgid"] = tileset.first_id.get();
  AddCommonAttributes(json, tileset, dir);

  return json;
}

[[nodiscard]] auto SaveExternalTileset(const TilesetData& tileset) -> JSON
{
  auto json = JSON::object();

  json["firstgid"] = tileset.first_id.get();
  json["source"] = std::format("{}.json", tileset.name);

  return json;
}

void CreateExternalTilesetFile(const TilesetData& tileset,
                               const std::filesystem::path& dir)
{
  auto json = JSON::object();

  AddCommonAttributes(json, tileset, dir);

  json["type"] = "tileset";
  json["tiledversion"] = tiled_version;
  json["version"] = tiled_json_version;

  const auto name = std::format("{}.json", tileset.name);
  const auto path = dir / name;

  CENTURION_LOG_INFO("Saving external tileset in \"%s\"", path.string().c_str());
  SaveJson(json, path);
}

[[nodiscard]] auto SaveTileset(const TilesetData& tileset,
                               const std::filesystem::path& dir,
                               const bool embed) -> JSON
{
  if (embed) {
    return SaveEmbeddedTileset(tileset, dir);
  }
  else {
    CreateExternalTilesetFile(tileset, dir);
    return SaveExternalTileset(tileset);
  }
}

}  // namespace

[[nodiscard]] auto SaveTilesets(const std::vector<TilesetData>& tilesets,
                                const std::filesystem::path& dir,
                                const bool embed) -> JSON
{
  auto json = JSON::array();

  for (const auto& tileset : tilesets) {
    json += SaveTileset(tileset, dir, embed);
  }

  return json;
}

}  // namespace Tactile::IO
