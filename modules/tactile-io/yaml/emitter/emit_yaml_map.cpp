#include <fstream>     // ofstream

#include "emitter.hpp"
#include "save_layers.hpp"
#include "save_properties.hpp"
#include "save_tilesets.hpp"

#include <yaml-cpp/yaml.h>

namespace Tactile::IO {
namespace {

constexpr int version = 1;

}  // namespace

void EmitYamlMap(const MapData& data, [[maybe_unused]] const EmitterOptions& options)
{
  YAML::Emitter emitter;
  emitter.SetIndent(2);

  emitter << YAML::BeginMap;

  emitter << YAML::Key << "version" << YAML::Value << version;
  emitter << YAML::Key << "row-count" << YAML::Value << data.row_count;
  emitter << YAML::Key << "column-count" << YAML::Value << data.column_count;
  emitter << YAML::Key << "tile-width" << YAML::Value << data.tile_width;
  emitter << YAML::Key << "tile-height" << YAML::Value << data.tile_height;
  emitter << YAML::Key << "next-layer-id" << YAML::Value << data.next_layer_id;
  emitter << YAML::Key << "next-object-id" << YAML::Value << data.next_object_id;

  const auto dir = data.absolute_path.parent_path();
  SaveProperties(emitter, data.properties, dir);
  SaveLayers(emitter, data.layers, dir);
  SaveTilesets(emitter, data.tilesets, dir);

  emitter << YAML::EndMap;

  std::ofstream stream{data.absolute_path};
  stream << emitter.c_str();
}

}  // namespace Tactile::IO