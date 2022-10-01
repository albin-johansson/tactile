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

#include <utility>  // move

#include <spdlog/spdlog.h>

#include "core/util/filesystem.hpp"
#include "core/util/fmt.hpp"
#include "core/util/functional.hpp"
#include "core/util/string.hpp"
#include "io/map/emit/emit_info.hpp"
#include "io/map/emit/emitter.hpp"
#include "io/map/tiled_info.hpp"
#include "io/proto/preferences.hpp"
#include "io/util/base64_tiles.hpp"
#include "io/util/json.hpp"

namespace tactile::io {
namespace {

[[nodiscard]] auto emit_properties(const ir::ContextData& context) -> JSON
{
  auto array = JSON::array();

  for (const auto& [name, value] : context.properties) {
    auto json = JSON::object();

    json["name"] = to_std(name);
    json["type"] = value.type();
    json["value"] = value;

    array += json;
  }

  return array;
}

[[nodiscard]] auto emit_object(const ir::ObjectData& object) -> JSON
{
  auto json = JSON::object();

  json["id"] = object.id;
  json["name"] = to_std(object.name);
  json["x"] = object.pos.x;
  json["y"] = object.pos.y;
  json["width"] = object.size.x;
  json["height"] = object.size.y;
  json["visible"] = object.visible;
  json["type"] = to_std(object.tag);
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
  json["width"] = map.col_count;
  json["height"] = map.row_count;

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
    json["data"] = to_std(base64_encode_tiles(tile_layer.tiles,
                                              map.row_count,
                                              map.col_count,
                                              map.tile_format.compression));
  }
  else {
    auto tiles = JSON::array();

    invoke_mn(map.row_count, map.col_count, [&](const usize row, const usize col) {
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

  for (const auto& object : object_layer.objects) {
    objects += emit_object(object);
  }

  json["objects"] = std::move(objects);
}

[[nodiscard]] auto emit_layer(const ir::MapData& map, const ir::LayerData& layer) -> JSON
{
  auto json = JSON::object();

  json["id"] = layer.id;
  json["name"] = to_std(layer.name);
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

      for (const auto& child_layer : group_layer.children) {
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

  for (const auto& layer : map.layers) {
    array += emit_layer(map, layer);
  }

  return array;
}

[[nodiscard]] auto emit_fancy_tile_animation(const ir::MetaTileData& tile) -> JSON
{
  auto array = JSON::array();

  for (const auto& frame : tile.frames) {
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
    for (const auto& object : tile.objects) {
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

  for (const auto& [id, tileData] : tileset.fancy_tiles) {
    json += emit_fancy_tile(id, tileData);
  }

  return json;
}

void add_common_tileset_attributes(JSON& json,
                                   const EmitInfo& info,
                                   const ir::TilesetData& tileset)
{
  json["name"] = to_std(tileset.name);
  json["columns"] = tileset.column_count;

  json["tilewidth"] = tileset.tile_size.x;
  json["tileheight"] = tileset.tile_size.y;
  json["tilecount"] = tileset.tile_count;

  const auto image_path = fs::relative(tileset.image_path, info.destination_dir());
  json["image"] = to_std(convert_to_forward_slashes(image_path));

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

[[nodiscard]] auto emit_embedded_tileset(const EmitInfo& info,
                                         const ir::TilesetData& tileset) -> JSON
{
  auto json = JSON::object();

  json["firstgid"] = tileset.first_tile;
  add_common_tileset_attributes(json, info, tileset);

  return json;
}

[[nodiscard]] auto emit_external_tileset(const ir::TilesetData& tileset) -> JSON
{
  auto json = JSON::object();

  json["firstgid"] = tileset.first_tile;
  json["source"] = to_std(format_str("{}.json", tileset.name));

  return json;
}

void create_external_tileset_file(const EmitInfo& info, const ir::TilesetData& tileset)
{
  auto json = JSON::object();
  add_common_tileset_attributes(json, info, tileset);

  json["type"] = "tileset";
  json["tiledversion"] = tiled_version;
  json["version"] = tiled_json_format_version;

  const auto name = format_str("{}.json", tileset.name);
  const auto path = info.destination_dir() / to_std_view(name);

  write_json(json, path);
}

[[nodiscard]] auto emit_tileset(const EmitInfo& info, const ir::TilesetData& tileset)
    -> JSON
{
  if (get_preferences().embed_tilesets) {
    return emit_embedded_tileset(info, tileset);
  }
  else {
    create_external_tileset_file(info, tileset);
    return emit_external_tileset(tileset);
  }
}

[[nodiscard]] auto emit_tilesets(const EmitInfo& info) -> JSON
{
  auto json = JSON::array();

  const auto& data = info.data();
  for (const auto& tileset : data.tilesets) {
    json += emit_tileset(info, tileset);
  }

  return json;
}

}  // namespace

void emit_json_map(const EmitInfo& info)
{
  auto json = JSON::object();

  const auto& map = info.data();

  if (!map.component_definitions.empty()) {
    spdlog::warn("Component data will be ignored when saving the map as JSON!");
  }

  json["type"] = "map";
  json["width"] = map.col_count;
  json["height"] = map.row_count;

  json["tilewidth"] = map.tile_size.x;
  json["tileheight"] = map.tile_size.y;

  json["nextlayerid"] = map.next_layer_id;
  json["nextobjectid"] = map.next_object_id;

  json["infinite"] = false;
  json["orientation"] = "orthogonal";
  json["renderorder"] = "right-down";

  if (const auto& format = map.tile_format; format.zlib_compression_level) {
    json["compressionlevel"] = *format.zlib_compression_level;
  }
  else if (format.zstd_compression_level) {
    json["compressionlevel"] = *format.zstd_compression_level;
  }
  else {
    json["compressionlevel"] = -1;
  }

  json["tiledversion"] = tiled_version;
  json["version"] = tiled_json_format_version;

  json["tilesets"] = emit_tilesets(info);
  json["layers"] = emit_layers(map);

  if (!map.context.properties.empty()) {
    json["properties"] = emit_properties(map.context);
  }

  write_json(json, info.destination_file());
}

}  // namespace tactile::io