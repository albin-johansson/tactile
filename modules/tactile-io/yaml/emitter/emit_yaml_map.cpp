#include <fstream>  // ofstream

#include <yaml-cpp/yaml.h>

#include "emitter.hpp"
#include "save_layers.hpp"
#include "save_properties.hpp"
#include "save_tilesets.hpp"

namespace Tactile::IO {
namespace {

constexpr int32 version = 1;

}  // namespace

void EmitYamlMap(const Map& map, const EmitterOptions& options)
{
  YAML::Emitter emitter;
  emitter.SetIndent(2);

  emitter << YAML::BeginMap;

  emitter << YAML::Key << "version" << YAML::Value << version;
  emitter << YAML::Key << "row-count" << YAML::Value << GetRowCount(map);
  emitter << YAML::Key << "column-count" << YAML::Value << GetColumnCount(map);
  emitter << YAML::Key << "tile-width" << YAML::Value << GetTileWidth(map);
  emitter << YAML::Key << "tile-height" << YAML::Value << GetTileHeight(map);
  emitter << YAML::Key << "next-layer-id" << YAML::Value << GetNextLayerId(map);
  emitter << YAML::Key << "next-object-id" << YAML::Value << GetNextObjectId(map);

  const std::filesystem::path path = GetAbsolutePath(map);
  const auto dir = path.parent_path();

  SaveProperties(emitter, map, dir);
  SaveLayers(emitter, map, dir, options);
  SaveTilesets(emitter, map, dir);

  emitter << YAML::EndMap;

  std::ofstream stream{path};
  stream << emitter.c_str();
}

}  // namespace Tactile::IO