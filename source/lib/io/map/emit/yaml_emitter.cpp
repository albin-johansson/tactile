/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <sstream>  // stringstream

#include <fmt/format.h>
#include <fmt/std.h>
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

#include "common/numeric.hpp"
#include "common/type/string.hpp"
#include "common/util/filesystem.hpp"
#include "core/attribute.hpp"
#include "io/compression.hpp"
#include "io/map/emit/emit_info.hpp"
#include "io/map/emit/emitter.hpp"
#include "io/stream.hpp"
#include "io/util/base64_tiles.hpp"
#include "io/util/yaml.hpp"
#include "model/settings.hpp"

namespace tactile::io {
namespace {

constexpr int tileset_node_version = 1;

void emit_properties(YAML::Emitter& emitter, const ir::ContextData& context)
{
  if (!context.properties.empty()) {
    emitter << YAML::Key << "properties" << YAML::BeginSeq;

    for (const auto& [name, value]: context.properties) {
      emitter << YAML::BeginMap;
      emitter << YAML::Key << "name" << YAML::Value << name;
      emitter << YAML::Key << "type" << YAML::Value << value.type();
      emitter << YAML::Key << "value" << YAML::Value << value;
      emitter << YAML::EndMap;
    }

    emitter << YAML::EndSeq;
  }
}

void emit_components(YAML::Emitter& emitter, const ir::ContextData& context)
{
  if (!context.components.empty()) {
    emitter << YAML::Key << "components" << YAML::BeginSeq;

    for (const auto& [type, values]: context.components) {
      emitter << YAML::BeginMap;

      emitter << YAML::Key << "type" << YAML::Value << type;
      emitter << YAML::Key << "values" << YAML::BeginSeq;

      for (const auto& [attr_name, attr_value]: values) {
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

void emit_object_data(YAML::Emitter& emitter, const ir::ObjectData& data)
{
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "id" << YAML::Value << data.id;

  emitter << YAML::Key << "type";
  switch (data.type) {
    case ObjectType::Point:
      emitter << YAML::Value << "point";
      break;

    case ObjectType::Rect:
      emitter << YAML::Value << "rect";
      break;

    case ObjectType::Ellipse:
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

  if (data.pos.x != 0.0f) {
    emitter << YAML::Key << "x" << YAML::Value << data.pos.x;
  }

  if (data.pos.y != 0.0f) {
    emitter << YAML::Key << "y" << YAML::Value << data.pos.y;
  }

  if (data.size.x != 0.0f) {
    emitter << YAML::Key << "width" << YAML::Value << data.size.x;
  }

  if (data.size.y != 0.0f) {
    emitter << YAML::Key << "height" << YAML::Value << data.size.y;
  }

  emit_properties(emitter, data.context);
  emit_components(emitter, data.context);

  emitter << YAML::EndMap;
}

void emit_object_layer_data(YAML::Emitter& emitter, const ir::ObjectLayerData& data)
{
  if (data.objects.empty()) {
    return;
  }

  emitter << YAML::Key << "objects" << YAML::BeginSeq;

  for (const auto& object_data: data.objects) {
    emit_object_data(emitter, object_data);
  }

  emitter << YAML::EndSeq;
}

void emit_plain_tile_layer_data(YAML::Emitter& emitter,
                                const ir::TileLayerData& data,
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

void emit_layer(YAML::Emitter& emitter,
                const ir::MapData& map,
                const ir::LayerData& layer)
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
      for (const auto& child_layer_data: group_layer.children) {
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

void emit_layers(YAML::Emitter& emitter, const ir::MapData& map)
{
  if (map.layers.empty()) {
    return;
  }

  emitter << YAML::Key << "layers" << YAML::BeginSeq;

  for (const auto& layer_data: map.layers) {
    emit_layer(emitter, map, layer_data);
  }

  emitter << YAML::EndSeq;
}

void emit_tileset_tile_animation(YAML::Emitter& emitter, const ir::MetaTileData& data)
{
  emitter << YAML::Key << "animation" << YAML::BeginSeq;

  for (const auto& frame_data: data.frames) {
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "tile" << YAML::Value << frame_data.tile_index;
    emitter << YAML::Key << "duration" << YAML::Value << frame_data.duration_ms;
    emitter << YAML::EndMap;
  }

  emitter << YAML::EndSeq;
}

void emit_tileset_tiles(YAML::Emitter& emitter, const ir::TilesetData& tileset)
{
  emitter << YAML::Key << "tiles" << YAML::BeginSeq;

  for (const auto& [id, tile]: tileset.fancy_tiles) {
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "id" << YAML::Value << id;

    if (!tile.frames.empty()) {
      emit_tileset_tile_animation(emitter, tile);
    }

    if (!tile.objects.empty()) {
      emitter << YAML::Key << "objects" << YAML::BeginSeq;
      for (const auto& object_data: tile.objects) {
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

void emit_tileset_file(const EmitInfo& info,
                       const String& filename,
                       const ir::TilesetData& tileset)
{
  YAML::Emitter emitter;
  emitter.SetIndent(2);
  emitter << YAML::BeginMap;

  emitter << YAML::Key << "version" << YAML::Value << tileset_node_version;
  emitter << YAML::Key << "name" << YAML::Value << tileset.name;

  emitter << YAML::Key << "tile-width" << YAML::Value << tileset.tile_size.x;
  emitter << YAML::Key << "tile-height" << YAML::Value << tileset.tile_size.y;

  emitter << YAML::Key << "tile-count" << YAML::Value << tileset.tile_count;
  emitter << YAML::Key << "column-count" << YAML::Value << tileset.column_count;

  const auto image_path = fs::relative(tileset.image_path, info.destination_dir());
  emitter << YAML::Key << "image-path" << YAML::Value
          << convert_to_forward_slashes(image_path);
  emitter << YAML::Key << "image-width" << YAML::Value << tileset.image_size.x;
  emitter << YAML::Key << "image-height" << YAML::Value << tileset.image_size.y;

  if (!tileset.fancy_tiles.empty()) {
    emit_tileset_tiles(emitter, tileset);
  }

  emit_properties(emitter, tileset.context);
  emit_components(emitter, tileset.context);

  emitter << YAML::EndMap;

  const auto path = info.destination_dir() / filename;
  spdlog::debug("Saving external tileset to {}", path);

  auto stream = open_output_stream(path, FileType::Text).value();
  stream << emitter.c_str();
}

void emit_tilesets(YAML::Emitter& emitter, const EmitInfo& info)
{
  const auto& data = info.data();

  if (data.tilesets.empty()) {
    return;
  }

  emitter << YAML::Key << "tilesets" << YAML::BeginSeq;

  for (const auto& tileset: data.tilesets) {
    const auto source = fmt::format("{}.yaml", tileset.name);
    emit_tileset_file(info, source, tileset);

    emitter << YAML::BeginMap;
    emitter << YAML::Key << "first-global-id" << YAML::Value << tileset.first_tile;
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
  emitter << YAML::Key << "type" << YAML::Value << value.type();

  if (!value.has_default_value()) {
    emitter << YAML::Key << "default" << YAML::Value << value;
  }

  emitter << YAML::EndMap;
}

void emit_component_definitions(YAML::Emitter& emitter, const EmitInfo& info)
{
  const auto& data = info.data();

  if (data.component_definitions.empty()) {
    return;
  }

  emitter << YAML::Key << "component-definitions" << YAML::BeginSeq;

  for (const auto& [name, attributes]: data.component_definitions) {
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "name" << YAML::Value << name;

    if (!attributes.empty()) {
      emitter << YAML::Key << "attributes" << YAML::BeginSeq;

      for (const auto& [attr_name, attr_value]: attributes) {
        emit_component_definition_attribute(emitter, attr_name, attr_value);
      }

      emitter << YAML::EndSeq;
    }

    emitter << YAML::EndMap;
  }

  emitter << YAML::EndSeq;
}

void emit_tile_format(YAML::Emitter& emitter, const ir::TileFormatData& format)
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

void emit_yaml_map(const EmitInfo& info)
{
  const auto& map = info.data();

  YAML::Emitter emitter;
  emitter.SetIndent(2);

  emitter << YAML::BeginMap;
  emitter << YAML::Key << "version" << YAML::Value << 1;

  emitter << YAML::Key << "row-count" << YAML::Value << map.extent.rows;
  emitter << YAML::Key << "column-count" << YAML::Value << map.extent.cols;

  emitter << YAML::Key << "tile-width" << YAML::Value << map.tile_size.x;
  emitter << YAML::Key << "tile-height" << YAML::Value << map.tile_size.y;

  emitter << YAML::Key << "next-layer-id" << YAML::Value << map.next_layer_id;
  emitter << YAML::Key << "next-object-id" << YAML::Value << map.next_object_id;

  emit_tile_format(emitter, map.tile_format);
  emit_component_definitions(emitter, info);
  emit_tilesets(emitter, info);
  emit_layers(emitter, map);

  emit_properties(emitter, map.context);
  emit_components(emitter, map.context);

  emitter << YAML::EndMap;

  auto stream = open_output_stream(info.destination_file(), FileType::Text).value();
  stream << emitter.c_str();
}

}  // namespace tactile::io