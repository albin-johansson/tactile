#include "save_tilesets.hpp"

#include <format>   // format
#include <fstream>  // ofstream
#include <string>   // string

#include "../../common_saving.hpp"
#include "save_properties.hpp"

#include <yaml-cpp/yaml.h>

namespace Tactile::IO {
namespace {

constexpr int tileset_node_version = 1;

void SaveAnimation(YAML::Emitter& emitter, const std::vector<FrameData>& frames)
{
  emitter << YAML::Key << "animation" << YAML::BeginSeq;

  for (const auto& frame : frames) {
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "tile" << YAML::Value << frame.tile;
    emitter << YAML::Key << "duration" << YAML::Value << frame.duration;
    emitter << YAML::EndMap;
  }

  emitter << YAML::EndSeq;
}

void SaveFancyTiles(YAML::Emitter& emitter,
                    const std::vector<TileData>& tiles,
                    const std::filesystem::path& dir)
{
  emitter << YAML::Key << "tiles" << YAML::BeginSeq;

  for (const auto& tile : tiles) {
    if (IsTileWorthSaving(tile)) {
      emitter << YAML::BeginMap;
      emitter << YAML::Key << "id" << YAML::Value << tile.id;

      if (!tile.animation.empty()) {
        SaveAnimation(emitter, tile.animation);
      }

      SaveProperties(emitter, tile.properties, dir);
      emitter << YAML::EndMap;
    }
  }

  emitter << YAML::EndSeq;
}

void SaveTileset(const TilesetData& tileset,
                 const std::string& fileName,
                 const std::filesystem::path& dir)
{
  YAML::Emitter emitter;
  emitter.SetIndent(2);

  emitter << YAML::BeginMap;
  emitter << YAML::Key << "version" << YAML::Value << tileset_node_version;
  emitter << YAML::Key << "name" << YAML::Value << tileset.name;
  emitter << YAML::Key << "tile-count" << YAML::Value << tileset.tile_count;
  emitter << YAML::Key << "tile-width" << YAML::Value << tileset.tile_width;
  emitter << YAML::Key << "tile-height" << YAML::Value << tileset.tile_height;
  emitter << YAML::Key << "column-count" << YAML::Value << tileset.column_count;
  emitter << YAML::Key << "image-path" << YAML::Value
          << GetTilesetImagePath(tileset.absolute_image_path, dir);
  emitter << YAML::Key << "image-width" << YAML::Value << tileset.image_width;
  emitter << YAML::Key << "image-height" << YAML::Value << tileset.image_height;

  SaveProperties(emitter, tileset.properties, dir);
  SaveFancyTiles(emitter, tileset.tiles, dir);

  emitter << YAML::EndMap;

  std::ofstream stream{dir / fileName};
  stream << emitter.c_str();
}

}  // namespace

void SaveTilesets(YAML::Emitter& emitter,
                  const std::vector<TilesetData>& tilesets,
                  const std::filesystem::path& dir)
{
  if (!tilesets.empty()) {
    emitter << YAML::Key << "tilesets" << YAML::BeginSeq;

    for (const auto& tileset : tilesets) {
      const auto fileName = std::format("{}.yaml", tileset.name);
      SaveTileset(tileset, fileName, dir);

      emitter << YAML::BeginMap;
      emitter << YAML::Key << "first-global-id" << YAML::Value << tileset.first_id;
      emitter << YAML::Key << "path" << YAML::Value << fileName;
      emitter << YAML::EndMap;
    }

    emitter << YAML::EndSeq;
  }
}

}  // namespace Tactile::IO
