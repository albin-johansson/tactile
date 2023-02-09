/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "tiled_json_exporter.hpp"

#include <utility>  // move

#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "common/util/filesystem.hpp"
#include "common/util/functional.hpp"
#include "io/export/tiled_info.hpp"
#include "io/ir/map/map_ir.hpp"
#include "io/util/base64_tiles.hpp"
#include "io/util/json.hpp"
#include "model/settings.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto emit_properties(const ir::ContextData& context) -> JSON
{
  auto array = JSON::array();

  for (const auto& [name, value]: context.properties) {
    auto json = JSON::object();

    json["name"] = name;
    json["type"] = value.get_type();
    json["value"] = value;

    array += json;
  }

  return array;
}

[[nodiscard]] auto emit_object(const ir::ObjectData& object) -> JSON
{
  auto json = JSON::object();

  json["id"] = object.id;
  json["name"] = object.name;
  json["x"] = object.pos.x;
  json["y"] = object.pos.y;
  json["width"] = object.size.x;
  json["height"] = object.size.y;
  json["visible"] = object.visible;
  json["type"] = object.tag;
  json["rotation"] = 0;

  switch (object.type) {
    case ObjectType::Rect:
      // Assumed to be rectangle if no point/ellipse indicators
      break;

    case ObjectType::Point: {
      json["point"] = true;
      break;
    }
    case ObjectType::Ellipse: {
      json["ellipse"] = true;
      break;
    }
  }

  if (!object.context.properties.empty()) {
    json["properties"] = emit_properties(object.context);
  }

  return json;
}

void emit_tile_layer(JSON& json, const ir::MapData& map, const ir::LayerData& layer)
{
  const auto& tile_layer = layer.as_tile_layer();

  json["type"] = "tilelayer";
  json["width"] = map.extent.cols;
  json["height"] = map.extent.rows;

  switch (map.tile_format.encoding) {
    case TileEncoding::Base64: {
      json["encoding"] = "base64";
      break;
    }
    default:
      // Do nothing
      break;
  }

  switch (map.tile_format.compression) {
    case TileCompression::Zlib: {
      json["compression"] = "zlib";
      break;
    }
    case TileCompression::Zstd: {
      json["compression"] = "zstd";
      break;
    }
    default:
      // Do nothing
      break;
  }

  if (map.tile_format.encoding == TileEncoding::Base64) {
    json["data"] =
        base64_encode_tiles(tile_layer.tiles, map.extent, map.tile_format.compression);
  }
  else {
    auto tiles = JSON::array();

    invoke_mn(map.extent.rows, map.extent.cols, [&](const usize row, const usize col) {
      tiles += tile_layer.tiles[row][col];
    });

    json["data"] = std::move(tiles);
  }
}

void emit_object_layer(JSON& json, const ir::LayerData& layer)
{
  const auto& object_layer = layer.as_object_layer();

  json["type"] = "objectgroup";

  auto objects = JSON::array();

  for (const auto& object: object_layer.objects) {
    objects += emit_object(object);
  }

  json["objects"] = std::move(objects);
}

[[nodiscard]] auto emit_layer(const ir::MapData& map, const ir::LayerData& layer) -> JSON
{
  auto json = JSON::object();

  json["id"] = layer.id;
  json["name"] = layer.name;
  json["opacity"] = layer.opacity;
  json["visible"] = layer.visible;
  json["x"] = 0;
  json["y"] = 0;

  switch (layer.type) {
    case LayerType::TileLayer:
      emit_tile_layer(json, map, layer);
      break;

    case LayerType::ObjectLayer:
      emit_object_layer(json, layer);
      break;

    case LayerType::GroupLayer: {
      const auto& group_layer = layer.as_group_layer();

      json["type"] = "group";

      auto layers = JSON::array();

      for (const auto& child_layer: group_layer.children) {
        layers += emit_layer(map, *child_layer);
      }

      json["layers"] = std::move(layers);
      break;
    }
  }

  if (!layer.context.properties.empty()) {
    json["properties"] = emit_properties(layer.context);
  }

  return json;
}

[[nodiscard]] auto emit_layers(const ir::MapData& map) -> JSON
{
  auto array = JSON::array();

  for (const auto& layer: map.layers) {
    array += emit_layer(map, layer);
  }

  return array;
}

[[nodiscard]] auto emit_fancy_tile_animation(const ir::MetaTileData& tile) -> JSON
{
  auto array = JSON::array();

  for (const auto& frame: tile.frames) {
    auto json = JSON::object();

    json["tileid"] = frame.tile_index;
    json["duration"] = frame.duration_ms;

    array += json;
  }

  return array;
}

[[nodiscard]] auto emit_fancy_tile(const TileID id, const ir::MetaTileData& tile) -> JSON
{
  auto json = JSON::object();

  json["id"] = id;

  if (!tile.frames.empty()) {
    json["animation"] = emit_fancy_tile_animation(tile);
  }

  if (!tile.objects.empty()) {
    auto dummy = JSON::object();
    dummy["draworder"] = "index";
    dummy["name"] = "";
    dummy["opacity"] = 1;
    dummy["type"] = "objectgroup";
    dummy["visible"] = true;
    dummy["x"] = 0;
    dummy["y"] = 0;

    auto objects = JSON::array();
    for (const auto& object: tile.objects) {
      objects += emit_object(object);
    }

    dummy["objects"] = std::move(objects);
    json["objectgroup"] = std::move(dummy);
  }

  if (!tile.context.properties.empty()) {
    json["properties"] = emit_properties(tile.context);
  }

  return json;
}

[[nodiscard]] auto emit_fancy_tiles(const ir::TilesetData& tileset) -> JSON
{
  auto json = JSON::array();

  for (const auto& [id, tile_data]: tileset.fancy_tiles) {
    json += emit_fancy_tile(id, tile_data);
  }

  return json;
}

void add_common_tileset_attributes(JSON& json,
                                   const Path& dir,
                                   const ir::TilesetData& tileset)
{
  json["name"] = tileset.name;
  json["columns"] = tileset.column_count;

  json["tilewidth"] = tileset.tile_size.x;
  json["tileheight"] = tileset.tile_size.y;
  json["tilecount"] = tileset.tile_count;

  const auto image_path = fs::relative(tileset.image_path, dir);
  json["image"] = use_forward_slashes(image_path);

  json["imagewidth"] = tileset.image_size.x;
  json["imageheight"] = tileset.image_size.y;

  json["margin"] = 0;
  json["spacing"] = 0;

  if (!tileset.fancy_tiles.empty()) {
    json["tiles"] = emit_fancy_tiles(tileset);
  }

  if (!tileset.context.properties.empty()) {
    json["properties"] = emit_properties(tileset.context);
  }
}

[[nodiscard]] auto emit_embedded_tileset(const Path& dir, const ir::TilesetData& tileset)
    -> JSON
{
  auto json = JSON::object();

  json["firstgid"] = tileset.first_tile;
  add_common_tileset_attributes(json, dir, tileset);

  return json;
}

[[nodiscard]] auto emit_external_tileset(const ir::TilesetData& tileset) -> JSON
{
  auto json = JSON::object();

  json["firstgid"] = tileset.first_tile;
  json["source"] = fmt::format("{}.json", tileset.name);

  return json;
}

void create_external_tileset_file(const Path& dir, const ir::TilesetData& tileset)
{
  auto json = JSON::object();
  add_common_tileset_attributes(json, dir, tileset);

  json["type"] = "tileset";
  json["tiledversion"] = kTiledVersion;
  json["version"] = kTiledJsonFormatVersion;

  const auto name = fmt::format("{}.json", tileset.name);
  const auto path = dir / name;

  if (save_json_to_file(json, path).failed()) {
    spdlog::error("Could not save JSON tileset file");
  }
}

[[nodiscard]] auto emit_tileset(const Path& dir, const ir::TilesetData& tileset) -> JSON
{
  if (get_settings().test_flag(SETTINGS_EMBED_TILESETS_BIT)) {
    return emit_embedded_tileset(dir, tileset);
  }
  else {
    create_external_tileset_file(dir, tileset);
    return emit_external_tileset(tileset);
  }
}

[[nodiscard]] auto emit_tilesets(const Path& dir, const ir::MapData& ir_map) -> JSON
{
  auto json = JSON::array();

  for (const auto& ir_tileset: ir_map.tilesets) {
    json += emit_tileset(dir, ir_tileset);
  }

  return json;
}

}  // namespace

void save_map_as_tiled_json(const Path& destination, const ir::MapData& ir_map)
{
  auto json = JSON::object();

  if (!ir_map.component_definitions.empty()) {
    spdlog::warn("Component data will be ignored when saving the map as JSON!");
  }

  json["type"] = "map";
  json["width"] = ir_map.extent.cols;
  json["height"] = ir_map.extent.rows;

  json["tilewidth"] = ir_map.tile_size.x;
  json["tileheight"] = ir_map.tile_size.y;

  json["nextlayerid"] = ir_map.next_layer_id;
  json["nextobjectid"] = ir_map.next_object_id;

  json["infinite"] = false;
  json["orientation"] = "orthogonal";
  json["renderorder"] = "right-down";

  if (const auto& format = ir_map.tile_format; format.zlib_compression_level) {
    json["compressionlevel"] = *format.zlib_compression_level;
  }
  else if (format.zstd_compression_level) {
    json["compressionlevel"] = *format.zstd_compression_level;
  }
  else {
    json["compressionlevel"] = -1;
  }

  json["tiledversion"] = kTiledVersion;
  json["version"] = kTiledJsonFormatVersion;

  json["tilesets"] = emit_tilesets(destination.parent_path(), ir_map);
  json["layers"] = emit_layers(ir_map);

  if (!ir_map.context.properties.empty()) {
    json["properties"] = emit_properties(ir_map.context);
  }

  if (save_json_to_file(json, destination).failed()) {
    spdlog::error("Could not save JSON map file");
  }
}

}  // namespace tactile
