#include "save_tilesets.hpp"

#include <utility>  // move

#include <fmt/format.h>  // format

#include "common_saving.hpp"
#include "emit.hpp"
#include "ir_helpers.hpp"
#include "save_fancy_tiles.hpp"
#include "save_json.hpp"
#include "save_properties.hpp"

namespace Tactile::IO {
namespace {

void AddCommonAttributes(JSON& json,
                         const Tileset& tileset,
                         const std::filesystem::path& dir)
{
  json["name"] = GetName(tileset);
  json["columns"] = GetColumnCount(tileset);
  json["image"] = GetTilesetImagePath(GetImagePath(tileset), dir);  // TODO check
  json["imagewidth"] = GetImageWidth(tileset);
  json["imageheight"] = GetImageHeight(tileset);
  json["margin"] = 0;
  json["spacing"] = 0;
  json["tilecount"] = GetTileCount(tileset);
  json["tilewidth"] = GetTileWidth(tileset);
  json["tileheight"] = GetTileHeight(tileset);
  json["tiles"] = SaveFancyTiles(tileset, dir);

  if (GetPropertyCount(tileset) != 0) {
    json["properties"] = SaveProperties(tileset, dir);
  }
}

[[nodiscard]] auto SaveEmbeddedTileset(const Tileset& tileset,
                                       const std::filesystem::path& dir) -> JSON
{
  auto json = JSON::object();

  json["firstgid"] = GetFirstGlobalId(tileset);
  AddCommonAttributes(json, tileset, dir);

  return json;
}

[[nodiscard]] auto SaveExternalTileset(const Tileset& tileset) -> JSON
{
  auto json = JSON::object();

  json["firstgid"] = GetFirstGlobalId(tileset);
  json["source"] = fmt::format("{}.json", GetName(tileset));

  return json;
}

void CreateExternalTilesetFile(const Tileset& tileset,
                               const std::filesystem::path& dir,
                               const bool indent)
{
  auto json = JSON::object();

  AddCommonAttributes(json, tileset, dir);

  json["type"] = "tileset";
  json["tiledversion"] = tiled_version;
  json["version"] = tiled_json_version;

  const auto name = fmt::format("{}.json", GetName(tileset));
  const auto path = dir / name;

  SaveJson(json, path, indent);
}

[[nodiscard]] auto SaveTileset(const Tileset& tileset,
                               const std::filesystem::path& dir,
                               const uint32 options) -> JSON
{
  if (options & EmitterOptions_EmbedTilesets) {
    return SaveEmbeddedTileset(tileset, dir);
  }
  else {
    CreateExternalTilesetFile(tileset, dir, options & EmitterOptions_IndentOutput);
    return SaveExternalTileset(tileset);
  }
}

}  // namespace

auto SaveTilesets(const Map& map, const std::filesystem::path& dir, const uint32 options)
    -> JSON
{
  auto json = JSON::array();

  EachTileset(map, [&](const Tileset& tileset) {
    json += SaveTileset(tileset, dir, options);
  });

  return json;
}

}  // namespace Tactile::IO
