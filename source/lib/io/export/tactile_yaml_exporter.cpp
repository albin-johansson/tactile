// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile_yaml_exporter.hpp"

#include <sstream>  // stringstream

#include <fmt/format.h>
#include <fmt/std.h>
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

#include "io/ir/map/map_ir.hpp"
#include "io/stream.hpp"
#include "io/util/base64_tiles.hpp"
#include "io/util/yaml.hpp"
#include "model/settings.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/int.hpp"
#include "tactile/core/meta/attribute.hpp"
#include "tactile/core/platform/filesystem.hpp"

namespace tactile {
namespace {

constexpr int kTilesetNodeVersion = 1;

void emit_properties(YAML::Emitter& emitter, const ContextIR& context)
{
  if (!context.properties.empty()) {
    emitter << YAML::Key << "properties" << YAML::BeginSeq;

    for (const auto& [name, value] : context.properties) {
      emitter << YAML::BeginMap;
      emitter << YAML::Key << "name" << YAML::Value << name;
      emitter << YAML::Key << "type" << YAML::Value << value.get_type();
      emitter << YAML::Key << "value" << YAML::Value << value;
      emitter << YAML::EndMap;
    }

    emitter << YAML::EndSeq;
  }
}

void emit_components(YAML::Emitter& emitter, const ContextIR& context)
{
  if (!context.components.empty()) {
    emitter << YAML::Key << "components" << YAML::BeginSeq;

    for (const auto& [type, values] : context.components) {
      emitter << YAML::BeginMap;

      emitter << YAML::Key << "type" << YAML::Value << type;
      emitter << YAML::Key << "values" << YAML::BeginSeq;

      for (const auto& [attr_name, attr_value] : values) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "name" << YAML::Value << attr_name;
        emitter << YAML::Key << "value" << YAML::Value << attr_value;
        emitter << YAML::EndMap;
      }

      emitter << YAML::EndSeq;
      emitter << YAML::EndMap;
    }

    emitter << YAML::EndSeq;
  }
}

void emit_object_data(YAML::Emitter& emitter, const ObjectIR& data)
{
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "id" << YAML::Value << data.id;

  emitter << YAML::Key << "type";
  switch (data.type) {
    case ObjectType::Point: emitter << YAML::Value << "point"; break;

    case ObjectType::Rect: emitter << YAML::Value << "rect"; break;

    case ObjectType::Ellipse: emitter << YAML::Value << "ellipse"; break;
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

  if (data.pos.x() != 0.0f) {
    emitter << YAML::Key << "x" << YAML::Value << data.pos.x();
  }

  if (data.pos.y() != 0.0f) {
    emitter << YAML::Key << "y" << YAML::Value << data.pos.y();
  }

  if (data.size.x() != 0.0f) {
    emitter << YAML::Key << "width" << YAML::Value << data.size.x();
  }

  if (data.size.y() != 0.0f) {
    emitter << YAML::Key << "height" << YAML::Value << data.size.y();
  }

  emit_properties(emitter, data.context);
  emit_components(emitter, data.context);

  emitter << YAML::EndMap;
}

void emit_object_layer_data(YAML::Emitter& emitter, const ObjectLayerIR& data)
{
  if (data.objects.empty()) {
    return;
  }

  emitter << YAML::Key << "objects" << YAML::BeginSeq;

  for (const auto& object_data : data.objects) {
    emit_object_data(emitter, object_data);
  }

  emitter << YAML::EndSeq;
}

void emit_plain_tile_layer_data(YAML::Emitter& emitter,
                                const TileLayerIR& data,
                                const TileExtent extent)
{
  const auto& settings = get_settings();
  const bool fold_tile_data = settings.test_flag(SETTINGS_FOLD_TILE_DATA_BIT);

  emitter << YAML::Key << "data";

  std::stringstream stream;
  for (usize row = 0; row < extent.rows; ++row) {
    for (usize col = 0; col < extent.cols; ++col) {
      if ((fold_tile_data && col != 0) || (!fold_tile_data && (row != 0 || col != 0))) {
        stream << ' ';
      }

      stream << data.tiles[row][col];
    }

    if (fold_tile_data && row < (extent.rows - 1)) {
      stream << '\n';
    }
  }

  if (fold_tile_data) {
    emitter << YAML::Literal << stream.str();
  }
  else {
    emitter << YAML::Value << stream.str();
  }
}

void emit_layer(YAML::Emitter& emitter, const MapIR& map, const LayerIR& layer)
{
  emitter << YAML::BeginMap;

  emitter << YAML::Key << "name" << YAML::Value << layer.name;
  emitter << YAML::Key << "id" << YAML::Value << layer.id;

  if (layer.opacity != 1.0f) {
    emitter << YAML::Key << "opacity" << YAML::Value << layer.opacity;
  }

  if (!layer.visible) {
    emitter << YAML::Key << "visible" << YAML::Value << layer.visible;
  }

  emitter << YAML::Key << "type";
  switch (layer.type) {
    case LayerType::TileLayer: {
      emitter << YAML::Value << "tile-layer";

      const auto& tile_layer = layer.as_tile_layer();

      if (map.tile_format.encoding == TileEncoding::Plain) {
        emit_plain_tile_layer_data(emitter, tile_layer, map.extent);
      }
      else {
        emitter << YAML::Key << "data";
        emitter << YAML::Value
                << base64_encode_tiles(tile_layer.tiles,
                                       tile_layer.extent,
                                       map.tile_format.compression);
      }

      break;
    }
    case LayerType::ObjectLayer: {
      emitter << YAML::Value << "object-layer";

      const auto& object_layer = layer.as_object_layer();
      emit_object_layer_data(emitter, object_layer);
      break;
    }
    case LayerType::GroupLayer: {
      emitter << YAML::Value << "group-layer";
      emitter << YAML::Key << "layers" << YAML::BeginSeq;

      const auto& group_layer = layer.as_group_layer();
      for (const auto& child_layer_data : group_layer.children) {
        emit_layer(emitter, map, *child_layer_data);
      }

      emitter << YAML::EndSeq;
      break;
    }
  }

  emit_properties(emitter, layer.context);
  emit_components(emitter, layer.context);

  emitter << YAML::EndMap;
}

void emit_layers(YAML::Emitter& emitter, const MapIR& map)
{
  if (map.layers.empty()) {
    return;
  }

  emitter << YAML::Key << "layers" << YAML::BeginSeq;

  for (const auto& layer_data : map.layers) {
    emit_layer(emitter, map, layer_data);
  }

  emitter << YAML::EndSeq;
}

void emit_tileset_tile_animation(YAML::Emitter& emitter, const TileIR& data)
{
  emitter << YAML::Key << "animation" << YAML::BeginSeq;

  for (const auto& frame_data : data.frames) {
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "tile" << YAML::Value << frame_data.tile_index;
    emitter << YAML::Key << "duration" << YAML::Value << frame_data.duration_ms;
    emitter << YAML::EndMap;
  }

  emitter << YAML::EndSeq;
}

void emit_tileset_tiles(YAML::Emitter& emitter, const TilesetIR& tileset)
{
  emitter << YAML::Key << "tiles" << YAML::BeginSeq;

  for (const auto& [id, tile] : tileset.fancy_tiles) {
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "id" << YAML::Value << id;

    if (!tile.frames.empty()) {
      emit_tileset_tile_animation(emitter, tile);
    }

    if (!tile.objects.empty()) {
      emitter << YAML::Key << "objects" << YAML::BeginSeq;
      for (const auto& object_data : tile.objects) {
        emit_object_data(emitter, object_data);
      }
      emitter << YAML::EndSeq;
    }

    emit_properties(emitter, tile.context);
    emit_components(emitter, tile.context);

    emitter << YAML::EndMap;
  }

  emitter << YAML::EndSeq;
}

void emit_tileset_file(const Path& dir, const String& filename, const TilesetIR& tileset)
{
  YAML::Emitter emitter;
  emitter.SetIndent(2);
  emitter << YAML::BeginMap;

  emitter << YAML::Key << "version" << YAML::Value << kTilesetNodeVersion;
  emitter << YAML::Key << "name" << YAML::Value << tileset.name;

  emitter << YAML::Key << "tile-width" << YAML::Value << tileset.tile_size.x();
  emitter << YAML::Key << "tile-height" << YAML::Value << tileset.tile_size.y();

  emitter << YAML::Key << "tile-count" << YAML::Value << tileset.tile_count;
  emitter << YAML::Key << "column-count" << YAML::Value << tileset.column_count;

  const auto image_path = fs::relative(tileset.image_path, dir);
  emitter << YAML::Key << "image-path" << YAML::Value << normalize_path(image_path);
  emitter << YAML::Key << "image-width" << YAML::Value << tileset.image_size.x();
  emitter << YAML::Key << "image-height" << YAML::Value << tileset.image_size.y();

  if (!tileset.fancy_tiles.empty()) {
    emit_tileset_tiles(emitter, tileset);
  }

  emit_properties(emitter, tileset.context);
  emit_components(emitter, tileset.context);

  emitter << YAML::EndMap;

  const auto path = dir / filename;
  spdlog::debug("Saving external tileset to {}", path);

  if (!save_yaml_to_file(emitter, path).has_value()) {
    spdlog::error("Could not save YAML tileset file");
  }
}

void emit_tilesets(YAML::Emitter& emitter, const Path& dir, const MapIR& ir_map)
{
  if (ir_map.tilesets.empty()) {
    return;
  }

  emitter << YAML::Key << "tilesets" << YAML::BeginSeq;

  for (const auto& ir_tileset : ir_map.tilesets) {
    const auto source = fmt::format("{}.yaml", ir_tileset.name);
    emit_tileset_file(dir, source, ir_tileset);

    emitter << YAML::BeginMap;
    emitter << YAML::Key << "first-global-id" << YAML::Value << ir_tileset.first_tile;
    emitter << YAML::Key << "path" << YAML::Value << source;
    emitter << YAML::EndMap;
  }

  emitter << YAML::EndSeq;
}

void emit_component_definition_attribute(YAML::Emitter& emitter,
                                         const String& name,
                                         const Attribute& value)
{
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "name" << YAML::Value << name;
  emitter << YAML::Key << "type" << YAML::Value << value.get_type();

  if (!value.has_default_value()) {
    emitter << YAML::Key << "default" << YAML::Value << value;
  }

  emitter << YAML::EndMap;
}

void emit_component_definitions(YAML::Emitter& emitter, const MapIR& ir_map)
{
  if (ir_map.component_definitions.empty()) {
    return;
  }

  emitter << YAML::Key << "component-definitions" << YAML::BeginSeq;

  for (const auto& [name, attributes] : ir_map.component_definitions) {
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "name" << YAML::Value << name;

    if (!attributes.empty()) {
      emitter << YAML::Key << "attributes" << YAML::BeginSeq;

      for (const auto& [attr_name, attr_value] : attributes) {
        emit_component_definition_attribute(emitter, attr_name, attr_value);
      }

      emitter << YAML::EndSeq;
    }

    emitter << YAML::EndMap;
  }

  emitter << YAML::EndSeq;
}

void emit_tile_format(YAML::Emitter& emitter, const TileFormatIR& format)
{
  if (format.encoding != TileEncoding::Plain) {
    emitter << YAML::Key << "tile-format" << YAML::BeginMap;
    emitter << YAML::Key << "encoding" << YAML::Value << format.encoding;

    if (format.compression != TileCompression::None) {
      emitter << YAML::Key << "compression" << YAML::Value << format.compression;
    }

    if (format.compression == TileCompression::Zlib &&
        format.zlib_compression_level.has_value() &&
        format.zlib_compression_level != -1) {
      emitter << YAML::Key << "zlib-compression-level" << YAML::Value
              << *format.zlib_compression_level;
    }

    if (format.compression == TileCompression::Zstd &&
        format.zstd_compression_level.has_value()) {
      emitter << YAML::Key << "zstd-compression-level" << YAML::Value
              << *format.zstd_compression_level;
    }

    emitter << YAML::EndMap;
  }
}

}  // namespace

void save_map_as_tactile_yaml(const Path& destination, const MapIR& ir_map)
{
  YAML::Emitter emitter;
  emitter.SetIndent(2);

  emitter << YAML::BeginMap;
  emitter << YAML::Key << "version" << YAML::Value << 1;

  emitter << YAML::Key << "row-count" << YAML::Value << ir_map.extent.rows;
  emitter << YAML::Key << "column-count" << YAML::Value << ir_map.extent.cols;

  emitter << YAML::Key << "tile-width" << YAML::Value << ir_map.tile_size.x();
  emitter << YAML::Key << "tile-height" << YAML::Value << ir_map.tile_size.y();

  emitter << YAML::Key << "next-layer-id" << YAML::Value << ir_map.next_layer_id;
  emitter << YAML::Key << "next-object-id" << YAML::Value << ir_map.next_object_id;

  emit_tile_format(emitter, ir_map.tile_format);
  emit_component_definitions(emitter, ir_map);
  emit_tilesets(emitter, destination.parent_path(), ir_map);
  emit_layers(emitter, ir_map);

  emit_properties(emitter, ir_map.context);
  emit_components(emitter, ir_map.context);

  emitter << YAML::EndMap;

  if (!save_yaml_to_file(emitter, destination).has_value()) {
    spdlog::error("Could not save YAML map file");
  }
}

}  // namespace tactile
