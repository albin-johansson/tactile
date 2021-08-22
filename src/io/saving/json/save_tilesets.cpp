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

[[nodiscard]] auto SaveFancyTiles(const TilesetManager& manager,
                                  const Tileset& tileset,
                                  const std::filesystem::path& dir) -> JSON
{
  auto array = JSON::array();

  for (const auto& [id, tile] : tileset.GetFancyTiles())
  {
    auto& tileJson = array.emplace_back();
    tileJson["id"] = manager.ToLocal(id).value().get();

    if (const auto& animation = tile.GetAnimation())
    {
      auto animationJson = JSON::array();
      for (const auto& frame : animation->GetFrames())
      {
        auto frameJson = JSON::object();

        frameJson["tileid"] = manager.ToLocal(frame.tile).value().get();
        frameJson["duration"] = frame.duration.count();

        animationJson += std::move(frameJson);
      }

      tileJson["animation"] = std::move(animationJson);
    }

    if (tile.GetPropertyCount() != 0)
    {
      tileJson["properties"] = SaveProperties(tile, dir);
    }
  }

  return array;
}

void AddCommonAttributes(JSON& json,
                         const TilesetManager& manager,
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
  json["tiles"] = SaveFancyTiles(manager, tileset, dir);

  if (tileset.GetPropertyCount() != 0)
  {
    json["properties"] = SaveProperties(tileset, dir);
  }
}

[[nodiscard]] auto SaveEmbeddedTileset(const TilesetManager& manager,
                                       const Tileset& tileset,
                                       const std::filesystem::path& dir) -> JSON
{
  auto json = JSON::object();

  json["firstgid"] = tileset.GetFirstId().get();
  AddCommonAttributes(json, manager, tileset, dir);

  return json;
}

[[nodiscard]] auto SaveExternalTileset(const Tileset& tileset) -> JSON
{
  auto json = JSON::object();

  json["firstgid"] = tileset.GetFirstId().get();
  json["source"] = std::format("{}.json", tileset.GetName());

  return json;
}

void CreateExternalTilesetFile(const TilesetManager& manager,
                               const Tileset& tileset,
                               const std::filesystem::path& dir)
{
  auto json = JSON::object();

  AddCommonAttributes(json, manager, tileset, dir);

  json["type"] = "tileset";
  json["tiledversion"] = tiled_version;
  json["version"] = tiled_json_version;

  const auto name = std::format("{}.json", tileset.GetName());
  const auto path = dir / name;

  CENTURION_LOG_INFO("Saving external tileset in \"%s\"", path.string().c_str());
  SaveJson(json, path);
}

[[nodiscard]] auto SaveTileset(const TilesetManager& manager,
                               const Tileset& tileset,
                               const std::filesystem::path& dir) -> JSON
{
  if (Prefs::GetEmbedTilesets())
  {
    return SaveEmbeddedTileset(manager, tileset, dir);
  }
  else
  {
    CreateExternalTilesetFile(manager, tileset, dir);
    return SaveExternalTileset(tileset);
  }
}

}  // namespace

[[nodiscard]] auto SaveTilesets(const MapDocument& document,
                                const std::filesystem::path& dir) -> JSON
{
  auto json = JSON::array();

  const auto& tilesets = document.GetTilesets();
  for (const auto& [id, tileset] : tilesets)
  {
    json += SaveTileset(tilesets, *tileset, dir);
  }

  return json;
}

}  // namespace Tactile::IO
