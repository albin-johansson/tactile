#include "save_document_as_yaml.hpp"

#include <filesystem>  // absolute
#include <fstream>     // ofstream

#include "save_layers.hpp"
#include "save_properties.hpp"
#include "save_tilesets.hpp"
#include "tactile/core/components/property.hpp"
#include "tactile/core/map.hpp"
#include "tactile/editor/document.hpp"

#include <yaml-cpp/yaml.h>

namespace Tactile::IO {
namespace {

constexpr int version = 1;

}  // namespace

void SaveDocumentAsYaml(const Document& document)
{
  const auto& registry = document.registry;
  const auto& map = registry.ctx<Map>();

  YAML::Emitter emitter;
  emitter.SetIndent(2);

  emitter << YAML::BeginMap;

  emitter << YAML::Key << "version" << YAML::Value << version;
  emitter << YAML::Key << "row-count" << YAML::Value << map.row_count;
  emitter << YAML::Key << "column-count" << YAML::Value << map.column_count;
  emitter << YAML::Key << "tile-width" << YAML::Value << map.tile_width;
  emitter << YAML::Key << "tile-height" << YAML::Value << map.tile_height;
  emitter << YAML::Key << "next-layer-id" << YAML::Value << map.next_layer_id;
  emitter << YAML::Key << "next-object-id" << YAML::Value << map.next_object_id;

  const auto dir = document.path.parent_path();
  SaveProperties(emitter, registry, entt::null, dir);
  SaveLayers(emitter, registry, dir);
  SaveTilesets(emitter, registry, dir);

  emitter << YAML::EndMap;

  std::ofstream stream{std::filesystem::absolute(document.path)};
  stream << emitter.c_str();
}

}  // namespace Tactile::IO