#include "yaml_emitter.hpp"

#include <fstream>  // ofstream
#include <ios>      // ios
#include <sstream>  // stringstream
#include <string>   // string

#include <yaml-cpp/yaml.h>

#include "core/attribute_value.hpp"
#include "io/emitter/emit_info.hpp"
#include "io/preferences.hpp"
#include "logging.hpp"
#include "tactile_def.hpp"
#include "tactile_stdlib.hpp"
#include "throw.hpp"

namespace tactile::emitter {
namespace {

constexpr int tileset_node_version = 1;

auto operator<<(YAML::Emitter& emitter, const attribute_value& value) -> YAML::Emitter&
{
  switch (value.type()) {
    case attribute_type::string:
      emitter << value.as_string();
      break;

    case attribute_type::integer:
      emitter << value.as_int();
      break;

    case attribute_type::floating:
      emitter << value.as_float();
      break;

    case attribute_type::boolean:
      emitter << value.as_bool();
      break;

    case attribute_type::file:
      emitter << value.as_file().c_str();
      break;

    case attribute_type::color:
      emitter << value.as_color().as_rgba();
      break;

    case attribute_type::object:
      emitter << value.as_object();
      break;
  }

  return emitter;
}

[[nodiscard]] auto stringify(const attribute_type type) -> std::string
{
  switch (type) {
    case attribute_type::string:
      return "string";

    case attribute_type::integer:
      return "int";

    case attribute_type::floating:
      return "float";

    case attribute_type::boolean:
      return "bool";

    case attribute_type::file:
      return "file";

    case attribute_type::color:
      return "color";

    case attribute_type::object:
      return "object";

    default:
      ThrowTraced(TactileError{"Could not recognize property type!"});
  }
}

void emit_object_data(YAML::Emitter& emitter,
                      const emit_info& info,
                      const ObjectData& object);

void emit_component_definition_attribute(YAML::Emitter& emitter,
                                         const std::string& name,
                                         const attribute_value& value)
{
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "name" << YAML::Value << name;

  const auto type = value.type();
  emitter << YAML::Key << "type" << YAML::Value << stringify(type);

  if (!value.has_default_value()) {
    emitter << YAML::Key << "default" << YAML::Value << value;
  }

  emitter << YAML::EndMap;
}

void emit_component_definitions(YAML::Emitter& emitter, const emit_info& info)
{
  const auto count = info.component_def_count();

  if (count == 0) {
    return;
  }

  emitter << YAML::Key << "component-definitions" << YAML::BeginSeq;

  for (usize index = 0; index < count; ++index) {
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "name" << YAML::Value << info.component_def_name(index);

    if (info.component_def_attr_count(index) > 0u) {
      emitter << YAML::Key << "attributes" << YAML::BeginSeq;

      info.each_component_def_attrs(
          [&](const std::string& name, const attribute_value& value) {
            emit_component_definition_attribute(emitter, name, value);
          },
          index);

      emitter << YAML::EndSeq;
    }

    emitter << YAML::EndMap;
  }

  emitter << YAML::EndSeq;
}

void emit_properties(YAML::Emitter& emitter,
                     const emit_info& info,
                     const ContextID contextId)
{
  if (info.property_count(contextId) > 0u) {
    emitter << YAML::Key << "properties" << YAML::BeginSeq;

    info.each_property(contextId,
                       [&](const std::string& name, const attribute_value& value) {
                         emitter << YAML::BeginMap;
                         emitter << YAML::Key << "name" << YAML::Value << name;
                         emitter << YAML::Key << "value" << YAML::Value << value;
                         emitter << YAML::EndMap;
                       });

    emitter << YAML::EndSeq;
  }
}

void emit_components(YAML::Emitter& emitter,
                     const emit_info& info,
                     const ContextID contextId)
{
  if (info.component_count(contextId) > 0u) {
    emitter << YAML::Key << "components" << YAML::BeginSeq;

    info.each_component(
        contextId,
        [&](const std::string& type, const component_attribute_map& values) {
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
        });

    emitter << YAML::EndSeq;
  }
}

void emit_tileset_tile_animation(YAML::Emitter& emitter,
                                 const emit_info& info,
                                 const TileID id)
{
  emitter << YAML::Key << "animation" << YAML::BeginSeq;

  info.each_fancy_tile_animation_frame(id, [&](const AnimationFrameData& frame) {
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "tile" << YAML::Value << frame.local_tile;
    emitter << YAML::Key << "duration" << YAML::Value << frame.duration_ms;
    emitter << YAML::EndMap;
  });

  emitter << YAML::EndSeq;
}

void emit_tileset_tiles(YAML::Emitter& emitter, const emit_info& info, const TilesetID id)
{
  emitter << YAML::Key << "tiles" << YAML::BeginSeq;

  info.each_tileset_fancy_tile(id, [&](const FancyTileData& tile) {
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "id" << YAML::Value << tile.local_id;

    if (info.fancy_tile_animation_frame_count(tile.global_id) != 0) {
      emit_tileset_tile_animation(emitter, info, tile.global_id);
    }

    if (info.fancy_tile_object_count(tile.global_id) != 0) {
      info.each_fancy_tile_object(tile.global_id, [&](const ObjectData& object) {
        emit_object_data(emitter, info, object);
      });
    }

    emit_properties(emitter, info, tile.context_id);
    emit_components(emitter, info, tile.context_id);

    emitter << YAML::EndMap;
  });

  emitter << YAML::EndSeq;
}

void emit_tileset_file(const emit_info& info,
                       const std::string& filename,
                       const TilesetData& tileset)
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
          << ConvertToForwardSlashes(imagePath);
  emitter << YAML::Key << "image-width" << YAML::Value << tileset.image_width;
  emitter << YAML::Key << "image-height" << YAML::Value << tileset.image_height;

  if (info.tileset_fancy_tile_count(tileset.id) != 0) {
    emit_tileset_tiles(emitter, info, tileset.id);
  }

  const auto contextId = info.tileset_context(tileset.id);
  emit_properties(emitter, info, contextId);
  emit_components(emitter, info, contextId);

  emitter << YAML::EndMap;

  const auto path = info.destination_dir() / filename;
  log_debug("Saving external tileset to {}", path);

  std::ofstream stream{path, std::ios::out};
  stream << emitter.c_str();
}

void emit_tilesets(YAML::Emitter& emitter, const emit_info& info)
{
  if (info.tileset_count() == 0) {
    return;
  }

  emitter << YAML::Key << "tilesets" << YAML::BeginSeq;

  info.each_tileset([&](const TilesetData& tileset) {
    const auto source = fmt::format("{}.yaml", tileset.name);
    emit_tileset_file(info, source, tileset);

    emitter << YAML::BeginMap;
    emitter << YAML::Key << "first-global-id" << YAML::Value << tileset.first_tile;
    emitter << YAML::Key << "path" << YAML::Value << source;
    emitter << YAML::EndMap;
  });

  emitter << YAML::EndSeq;
}

void emit_object_data(YAML::Emitter& emitter,
                      const emit_info& info,
                      const ObjectData& object)
{
  emitter << YAML::Key << "id" << YAML::Value << object.id;

  emitter << YAML::Key << "type";
  switch (object.type) {
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

  if (!object.name.empty()) {
    emitter << YAML::Key << "name" << YAML::Value << object.name;
  }

  if (!object.tag.empty()) {
    emitter << YAML::Key << "tag" << YAML::Value << object.tag;
  }

  if (!object.visible) {
    emitter << YAML::Key << "visible" << YAML::Value << object.visible;
  }

  if (object.x != 0.0f) {
    emitter << YAML::Key << "x" << YAML::Value << object.x;
  }

  if (object.y != 0.0f) {
    emitter << YAML::Key << "y" << YAML::Value << object.y;
  }

  if (object.width != 0.0f) {
    emitter << YAML::Key << "width" << YAML::Value << object.width;
  }

  if (object.height != 0.0f) {
    emitter << YAML::Key << "height" << YAML::Value << object.height;
  }

  const auto contextId = info.object_context(object.id);
  emit_properties(emitter, info, contextId);
  emit_components(emitter, info, contextId);
}

void emit_object_layer_data(YAML::Emitter& emitter,
                            const emit_info& info,
                            const LayerID id)
{
  if (info.layer_object_count(id) == 0) {
    return;
  }

  emitter << YAML::Key << "objects" << YAML::BeginSeq;

  info.each_layer_object(id, [&](const ObjectData& object) {
    emit_object_data(emitter, info, object);
  });

  emitter << YAML::EndSeq;
}

void emit_tile_layer_data(YAML::Emitter& emitter, const emit_info& info, const LayerID id)
{
  const bool fold = prefs::GetFoldTileData();

  const auto nRows = info.row_count();
  const auto nCols = info.column_count();

  emitter << YAML::Key << "data";

  std::stringstream stream;
  info.each_layer_tile(id, [&](const usize row, const usize col, const TileID tile) {
    if ((fold && col != 0) || (!fold && (row != 0 || col != 0))) {
      stream << ' ';
    }

    stream << tile;

    if (col == nCols - 1) {
      if (fold && row < (nRows - 1)) {
        stream << '\n';
      }
    }
  });

  if (fold) {
    emitter << YAML::Literal << stream.str();
  }
  else {
    emitter << YAML::Value << stream.str();
  }
}

void emit_layers(YAML::Emitter& emitter,
                 const emit_info& info,
                 const Maybe<LayerID> parent = {})
{
  if (info.layer_count(parent) == 0u) {
    return;
  }

  emitter << YAML::Key << "layers" << YAML::BeginSeq;

  info.each_layer(parent, [&](const LayerID id) {
    emitter << YAML::BeginMap;

    const auto layer = info.layer_data(id);

    emitter << YAML::Key << "name" << YAML::Value << layer.name;
    emitter << YAML::Key << "id" << YAML::Value << id;

    if (layer.opacity != 1.0f) {
      emitter << YAML::Key << "opacity" << YAML::Value << layer.opacity;
    }

    if (!layer.visible) {
      emitter << YAML::Key << "visible" << YAML::Value << layer.visible;
    }

    emitter << YAML::Key << "type";
    switch (layer.type) {
      case layer_type::tile_layer:
        emitter << YAML::Value << "tile-layer";
        emit_tile_layer_data(emitter, info, id);
        break;

      case layer_type::object_layer:
        emitter << YAML::Value << "object-layer";
        emit_object_layer_data(emitter, info, id);
        break;

      case layer_type::group_layer:
        emitter << YAML::Value << "group-layer";
        emit_layers(emitter, info, id);
        break;
    }

    const auto contextId = info.layer_context(id);
    emit_properties(emitter, info, contextId);
    emit_components(emitter, info, contextId);

    emitter << YAML::EndMap;
  });

  emitter << YAML::EndSeq;
}

}  // namespace

void emit_yaml_map(const emit_info& info)
{
  YAML::Emitter emitter;
  emitter.SetIndent(2);

  emitter << YAML::BeginMap;
  emitter << YAML::Key << "version" << YAML::Value << 1;

  emitter << YAML::Key << "row-count" << YAML::Value << info.row_count();
  emitter << YAML::Key << "column-count" << YAML::Value << info.column_count();

  emitter << YAML::Key << "tile-width" << YAML::Value << info.tile_width();
  emitter << YAML::Key << "tile-height" << YAML::Value << info.tile_height();

  emitter << YAML::Key << "next-layer-id" << YAML::Value << info.next_layer_id();
  emitter << YAML::Key << "next-object-id" << YAML::Value << info.next_object_id();

  emit_component_definitions(emitter, info);

  emit_tilesets(emitter, info);
  emit_layers(emitter, info);

  const auto contextId = info.root_context();
  emit_components(emitter, info, contextId);
  emit_properties(emitter, info, contextId);

  emitter << YAML::EndMap;

  std::ofstream stream{info.destination_file(), std::ios::out};
  stream << emitter.c_str();
}

}  // namespace tactile::emitter