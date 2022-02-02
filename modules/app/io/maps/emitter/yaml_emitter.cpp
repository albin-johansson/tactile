#include "yaml_emitter.hpp"

#include <fstream>  // ofstream
#include <ios>      // ios
#include <sstream>  // stringstream
#include <string>   // string

#include <yaml-cpp/yaml.h>

#include "core/attribute_value.hpp"
#include "emit_info.hpp"
#include "io/maps/yaml_utils.hpp"
#include "io/persistence/preferences.hpp"
#include "logging.hpp"
#include "tactile_def.hpp"
#include "tactile_stdlib.hpp"

namespace tactile::emitter {
namespace {

constexpr int tileset_node_version = 1;

void _emit_properties(YAML::Emitter& emitter, const ir::attribute_context_data& context)
{
  if (!context.properties.empty()) {
    emitter << YAML::Key << "properties" << YAML::BeginSeq;

    for (const auto& [name, value] : context.properties) {
      emitter << YAML::BeginMap;
      emitter << YAML::Key << "name" << YAML::Value << name;
      emitter << YAML::Key << "type" << YAML::Value << value.type();
      emitter << YAML::Key << "value" << YAML::Value << value;
      emitter << YAML::EndMap;
    }

    emitter << YAML::EndSeq;
  }
}

void _emit_components(YAML::Emitter& emitter, const ir::attribute_context_data& context)
{
  if (!context.components.empty()) {
    emitter << YAML::Key << "components" << YAML::BeginSeq;

    for (const auto& [type, values] : context.components) {
      emitter << YAML::BeginMap;

      emitter << YAML::Key << "type" << YAML::Value << type;
      emitter << YAML::Key << "values" << YAML::BeginSeq;

      for (const auto& [attrName, attrValue] : values) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "name" << YAML::Value << attrName;
        emitter << YAML::Key << "value" << YAML::Value << attrValue;
        emitter << YAML::EndMap;
      }

      emitter << YAML::EndSeq;
      emitter << YAML::EndMap;
    }

    emitter << YAML::EndSeq;
  }
}

void _emit_object_data(YAML::Emitter& emitter, const ir::object_data& data)
{
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "id" << YAML::Value << data.id;

  emitter << YAML::Key << "type";
  switch (data.type) {
    case object_type::point:
      emitter << YAML::Value << "point";
      break;

    case object_type::rect:
      emitter << YAML::Value << "rect";
      break;

    case object_type::ellipse:
      emitter << YAML::Value << "ellipse";
      break;
  }

  if (!data.name.empty()) {
    emitter << YAML::Key << "name" << YAML::Value << data.name;
  }

  if (!data.tag.empty()) {
    emitter << YAML::Key << "tag" << YAML::Value << data.tag;
  }

  if (!data.visible) {
    emitter << YAML::Key << "visible" << YAML::Value << data.visible;
  }

  if (data.x != 0.0f) {
    emitter << YAML::Key << "x" << YAML::Value << data.x;
  }

  if (data.y != 0.0f) {
    emitter << YAML::Key << "y" << YAML::Value << data.y;
  }

  if (data.width != 0.0f) {
    emitter << YAML::Key << "width" << YAML::Value << data.width;
  }

  if (data.height != 0.0f) {
    emitter << YAML::Key << "height" << YAML::Value << data.height;
  }

  _emit_properties(emitter, data.context);
  _emit_components(emitter, data.context);

  emitter << YAML::EndMap;
}

void _emit_object_layer_data(YAML::Emitter& emitter, const ir::object_layer_data& data)
{
  if (data.objects.empty()) {
    return;
  }

  emitter << YAML::Key << "objects" << YAML::BeginSeq;

  for (const auto& objectData : data.objects) {
    _emit_object_data(emitter, objectData);
  }

  emitter << YAML::EndSeq;
}

void _emit_tile_layer_data(YAML::Emitter& emitter,
                           const ir::tile_layer_data& data,
                           const usize rows,
                           const usize columns)
{
  const bool fold = get_preferences().fold_tile_data();

  emitter << YAML::Key << "data";

  std::stringstream stream;
  for (usize row = 0; row < rows; ++row) {
    for (usize col = 0; col < columns; ++col) {
      if ((fold && col != 0) || (!fold && (row != 0 || col != 0))) {
        stream << ' ';
      }

      stream << data.tiles[row][col];
    }

    if (fold && row < (rows - 1)) {
      stream << '\n';
    }
  }

  if (fold) {
    emitter << YAML::Literal << stream.str();
  }
  else {
    emitter << YAML::Value << stream.str();
  }
}

void _emit_layer(YAML::Emitter& emitter,
                 const ir::layer_data& data,
                 const usize rows,
                 const usize columns)
{
  emitter << YAML::BeginMap;

  emitter << YAML::Key << "name" << YAML::Value << data.name;
  emitter << YAML::Key << "id" << YAML::Value << data.id;

  if (data.opacity != 1.0f) {
    emitter << YAML::Key << "opacity" << YAML::Value << data.opacity;
  }

  if (!data.visible) {
    emitter << YAML::Key << "visible" << YAML::Value << data.visible;
  }

  emitter << YAML::Key << "type";
  switch (data.type) {
    case layer_type::tile_layer:
      emitter << YAML::Value << "tile-layer";
      _emit_tile_layer_data(emitter,
                            std::get<ir::tile_layer_data>(data.data),
                            rows,
                            columns);
      break;

    case layer_type::object_layer:
      emitter << YAML::Value << "object-layer";
      _emit_object_layer_data(emitter, std::get<ir::object_layer_data>(data.data));
      break;

    case layer_type::group_layer: {
      emitter << YAML::Value << "group-layer";
      emitter << YAML::Key << "layers" << YAML::BeginSeq;

      for (const auto& childLayerData :
           std::get<ir::group_layer_data>(data.data).children) {
        _emit_layer(emitter, *childLayerData, rows, columns);
      }

      emitter << YAML::EndSeq;
      break;
    }
  }

  _emit_properties(emitter, data.context);
  _emit_components(emitter, data.context);

  emitter << YAML::EndMap;
}

void _emit_layers(YAML::Emitter& emitter, const ir::map_data& data)
{
  if (data.layers.empty()) {
    return;
  }

  emitter << YAML::Key << "layers" << YAML::BeginSeq;

  for (const auto& layerData : data.layers) {
    _emit_layer(emitter, layerData, data.row_count, data.col_count);
  }

  emitter << YAML::EndSeq;
}

void _emit_tileset_tile_animation(YAML::Emitter& emitter, const ir::fancy_tile_data& data)
{
  emitter << YAML::Key << "animation" << YAML::BeginSeq;

  for (const auto& frameData : data.frames) {
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "tile" << YAML::Value << frameData.local_id;
    emitter << YAML::Key << "duration" << YAML::Value << frameData.duration_ms;
    emitter << YAML::EndMap;
  }

  emitter << YAML::EndSeq;
}

void _emit_tileset_tiles(YAML::Emitter& emitter, const ir::tileset_data& tileset)
{
  emitter << YAML::Key << "tiles" << YAML::BeginSeq;

  for (const auto& [id, tile] : tileset.fancy_tiles) {
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "id" << YAML::Value << id;

    if (!tile.frames.empty()) {
      _emit_tileset_tile_animation(emitter, tile);
    }

    if (!tile.objects.empty()) {
      for (const auto& objectData : tile.objects) {
        _emit_object_data(emitter, objectData);
      }
    }

    _emit_properties(emitter, tile.context);
    _emit_components(emitter, tile.context);

    emitter << YAML::EndMap;
  }

  emitter << YAML::EndSeq;
}

void _emit_tileset_file(const emit_info& info,
                        const std::string& filename,
                        const ir::tileset_data& tileset)
{
  YAML::Emitter emitter;
  emitter.SetIndent(2);
  emitter << YAML::BeginMap;

  emitter << YAML::Key << "version" << YAML::Value << tileset_node_version;
  emitter << YAML::Key << "name" << YAML::Value << tileset.name;

  emitter << YAML::Key << "tile-width" << YAML::Value << tileset.tile_width;
  emitter << YAML::Key << "tile-height" << YAML::Value << tileset.tile_height;

  emitter << YAML::Key << "tile-count" << YAML::Value << tileset.tile_count;
  emitter << YAML::Key << "column-count" << YAML::Value << tileset.column_count;

  const auto imagePath =
      std::filesystem::relative(tileset.image_path, info.destination_dir());
  emitter << YAML::Key << "image-path" << YAML::Value
          << convert_to_forward_slashes(imagePath);
  emitter << YAML::Key << "image-width" << YAML::Value << tileset.image_width;
  emitter << YAML::Key << "image-height" << YAML::Value << tileset.image_height;

  if (!tileset.fancy_tiles.empty()) {
    _emit_tileset_tiles(emitter, tileset);
  }

  _emit_properties(emitter, tileset.context);
  _emit_components(emitter, tileset.context);

  emitter << YAML::EndMap;

  const auto path = info.destination_dir() / filename;
  log_debug("Saving external tileset to {}", path);

  std::ofstream stream{path, std::ios::out};
  stream << emitter.c_str();
}

void _emit_tilesets(YAML::Emitter& emitter, const emit_info& info)
{
  const auto& data = info.data();

  if (data.tilesets.empty()) {
    return;
  }

  emitter << YAML::Key << "tilesets" << YAML::BeginSeq;

  for (const auto& tileset : data.tilesets) {
    const auto source = fmt::format("{}.yaml", tileset.name);
    _emit_tileset_file(info, source, tileset);

    emitter << YAML::BeginMap;
    emitter << YAML::Key << "first-global-id" << YAML::Value << tileset.first_tile;
    emitter << YAML::Key << "path" << YAML::Value << source;
    emitter << YAML::EndMap;
  }

  emitter << YAML::EndSeq;
}

void _emit_component_definition_attribute(YAML::Emitter& emitter,
                                          const std::string& name,
                                          const attribute_value& value)
{
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "name" << YAML::Value << name;
  emitter << YAML::Key << "type" << YAML::Value << value.type();

  if (!value.has_default_value()) {
    emitter << YAML::Key << "default" << YAML::Value << value;
  }

  emitter << YAML::EndMap;
}

void _emit_component_definitions(YAML::Emitter& emitter, const emit_info& info)
{
  const auto& data = info.data();

  if (data.component_definitions.empty()) {
    return;
  }

  emitter << YAML::Key << "component-definitions" << YAML::BeginSeq;

  for (const auto& [name, attributes] : data.component_definitions) {
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "name" << YAML::Value << name;

    if (!attributes.empty()) {
      emitter << YAML::Key << "attributes" << YAML::BeginSeq;

      for (const auto& [attrName, attrValue] : attributes) {
        _emit_component_definition_attribute(emitter, attrName, attrValue);
      }

      emitter << YAML::EndSeq;
    }

    emitter << YAML::EndMap;
  }

  emitter << YAML::EndSeq;
}

}  // namespace

void emit_yaml_map(const emit_info& info)
{
  const auto& data = info.data();

  YAML::Emitter emitter;
  emitter.SetIndent(2);

  emitter << YAML::BeginMap;
  emitter << YAML::Key << "version" << YAML::Value << 1;

  emitter << YAML::Key << "row-count" << YAML::Value << data.row_count;
  emitter << YAML::Key << "column-count" << YAML::Value << data.col_count;

  emitter << YAML::Key << "tile-width" << YAML::Value << data.tile_width;
  emitter << YAML::Key << "tile-height" << YAML::Value << data.tile_height;

  emitter << YAML::Key << "next-layer-id" << YAML::Value << data.next_layer_id;
  emitter << YAML::Key << "next-object-id" << YAML::Value << data.next_object_id;

  _emit_component_definitions(emitter, info);
  _emit_tilesets(emitter, info);
  _emit_layers(emitter, data);

  _emit_properties(emitter, data.context);
  _emit_components(emitter, data.context);

  emitter << YAML::EndMap;

  std::ofstream stream{info.destination_file(), std::ios::out};
  stream << emitter.c_str();
}

}  // namespace tactile::emitter