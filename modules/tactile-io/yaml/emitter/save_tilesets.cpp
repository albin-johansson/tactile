#include "save_tilesets.hpp"

#include <fstream>  // ofstream
#include <string>   // string

#include <fmt/format.h>  // format
#include <yaml-cpp/yaml.h>

#include "../../common_saving.hpp"
#include "save_object.hpp"
#include "save_properties.hpp"

namespace Tactile::IO {
namespace {

constexpr int tileset_node_version = 1;

void SaveAnimation(YAML::Emitter& emitter, const Tile& tile)
{
  emitter << YAML::Key << "animation" << YAML::BeginSeq;

  EachAnimationFrame(tile, [&](const AnimationFrame& frame) {
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "tile" << YAML::Value << GetTile(frame);
    emitter << YAML::Key << "duration" << YAML::Value << GetDuration(frame);
    emitter << YAML::EndMap;
  });

  emitter << YAML::EndSeq;
}

void SaveObjects(YAML::Emitter& emitter,
                 const Tile& tile,
                 const std::filesystem::path& dir)
{
  emitter << YAML::Key << "objects" << YAML::BeginSeq;

  EachObject(tile, [&](const Object& object) { SaveObject(emitter, object, dir); });

  emitter << YAML::EndSeq;
}

void SaveFancyTiles(YAML::Emitter& emitter,
                    const Tileset& tileset,
                    const std::filesystem::path& dir)
{
  if (GetTileInfoCount(tileset) != 0) {
    emitter << YAML::Key << "tiles" << YAML::BeginSeq;

    EachTileInfo(tileset, [&](const Tile& tile) {
      if (IsWorthSaving(tile)) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "id" << YAML::Value << GetId(tile);

        if (GetAnimationFrameCount(tile) != 0) {
          SaveAnimation(emitter, tile);
        }

        if (GetObjectCount(tile) != 0) {
          SaveObjects(emitter, tile, dir);
        }

        SaveProperties(emitter, tile, dir);
        emitter << YAML::EndMap;
      }
    });

    emitter << YAML::EndSeq;
  }
}

void SaveTileset(const Tileset& tileset,
                 const std::string& fileName,
                 const std::filesystem::path& dir)
{
  YAML::Emitter emitter;
  emitter.SetIndent(2);

  emitter << YAML::BeginMap;
  emitter << YAML::Key << "version" << YAML::Value << tileset_node_version;
  emitter << YAML::Key << "name" << YAML::Value << GetName(tileset);
  emitter << YAML::Key << "tile-count" << YAML::Value << GetTileCount(tileset);
  emitter << YAML::Key << "tile-width" << YAML::Value << GetTileWidth(tileset);
  emitter << YAML::Key << "tile-height" << YAML::Value << GetTileHeight(tileset);
  emitter << YAML::Key << "column-count" << YAML::Value << GetColumnCount(tileset);
  emitter << YAML::Key << "image-path" << YAML::Value
          << GetTilesetImagePath(GetImagePath(tileset), dir);
  emitter << YAML::Key << "image-width" << YAML::Value << GetImageWidth(tileset);
  emitter << YAML::Key << "image-height" << YAML::Value << GetImageHeight(tileset);

  SaveProperties(emitter, tileset, dir);
  SaveFancyTiles(emitter, tileset, dir);

  emitter << YAML::EndMap;

  std::ofstream stream{dir / fileName};
  stream << emitter.c_str();
}

}  // namespace

void SaveTilesets(YAML::Emitter& emitter,
                  const Map& map,
                  const std::filesystem::path& dir)
{
  if (GetTilesetCount(map) != 0) {
    emitter << YAML::Key << "tilesets" << YAML::BeginSeq;

    EachTileset(map, [&](const Tileset& tileset) {
      const auto fileName = fmt::format("{}.yaml", GetName(tileset));
      SaveTileset(tileset, fileName, dir);

      emitter << YAML::BeginMap;
      emitter << YAML::Key << "first-global-id" << YAML::Value
              << GetFirstGlobalId(tileset);
      emitter << YAML::Key << "path" << YAML::Value << fileName;
      emitter << YAML::EndMap;
    });

    emitter << YAML::EndSeq;
  }
}

}  // namespace Tactile::IO
