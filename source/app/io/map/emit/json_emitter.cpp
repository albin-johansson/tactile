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
#include <variant>  // get

#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "core/common/filesystem.hpp"
#include "core/common/functional.hpp"
#include "io/map/emit/emit_info.hpp"
#include "io/map/emit/emitter.hpp"
#include "io/map/tiled_info.hpp"
#include "io/persistence/preferences.hpp"
#include "io/util/json.hpp"

namespace tactile::io {
namespace {

[[nodiscard]] auto emit_properties(const ir::ContextData& data) -> JSON
{
  auto array = JSON::array();

  for (const auto& [name, value] : data.properties) {
    auto json = JSON::object();

    json["name"] = name;
    json["type"] = value.type();
    json["value"] = value;

    array += json;
  }

  return array;
}

[[nodiscard]] auto emit_object(const ir::ObjectData& data) -> JSON
{
  auto json = JSON::object();

  json["id"] = data.id;
  json["name"] = data.name;
  json["x"] = data.pos.x;
  json["y"] = data.pos.y;
  json["width"] = data.size.x;
  json["height"] = data.size.y;
  json["visible"] = data.visible;
  json["type"] = data.tag;
  json["rotation"] = 0;

  switch (data.type) {
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

  if (!data.context.properties.empty()) {
    json["properties"] = emit_properties(data.context);
  }

  return json;
}

[[nodiscard]] auto emit_layer(const ir::LayerData& data,
                              const usize          rows,
                              const usize          columns) -> JSON
{
  auto json = JSON::object();

  json["id"] = data.id;
  json["name"] = data.name;
  json["opacity"] = data.opacity;
  json["visible"] = data.visible;
  json["x"] = 0;
  json["y"] = 0;

  switch (data.type) {
    case LayerType::TileLayer: {
      const auto& tile_layer_data = std::get<ir::TileLayerData>(data.data);

      json["type"] = "tilelayer";
      json["width"] = columns;
      json["height"] = rows;

      auto tiles = JSON::array();

      invoke_mn(rows, columns, [&](const usize row, const usize col) {
        tiles += tile_layer_data.tiles[row][col];
      });

      json["data"] = std::move(tiles);
      break;
    }
    case LayerType::ObjectLayer: {
      const auto& object_layer_data = std::get<ir::ObjectLayerData>(data.data);

      json["type"] = "objectgroup";

      auto objects = JSON::array();

      for (const auto& object_data : object_layer_data.objects) {
        objects += emit_object(object_data);
      }

      json["objects"] = std::move(objects);
      break;
    }
    case LayerType::GroupLayer: {
      const auto& group_layer_data = std::get<ir::GroupLayerData>(data.data);

      json["type"] = "group";

      auto layers = JSON::array();

      for (const auto& child_layer_data : group_layer_data.children) {
        layers += emit_layer(*child_layer_data, rows, columns);
      }

      json["layers"] = std::move(layers);
      break;
    }
  }

  if (!data.context.properties.empty()) {
    json["properties"] = emit_properties(data.context);
  }

  return json;
}

[[nodiscard]] auto emit_layers(const ir::MapData& data) -> JSON
{
  auto array = JSON::array();

  for (const auto& layer_data : data.layers) {
    array += emit_layer(layer_data, data.row_count, data.col_count);
  }

  return array;
}

[[nodiscard]] auto emit_fancy_tile_animation(const ir::MetaTileData& data) -> JSON
{
  auto array = JSON::array();

  for (const auto& frame_data : data.frames) {
    auto json = JSON::object();

    json["tileid"] = frame_data.local_id;
    json["duration"] = frame_data.duration_ms;

    array += json;
  }

  return array;
}

[[nodiscard]] auto emit_fancy_tile(const TileID id, const ir::MetaTileData& data) -> JSON
{
  auto json = JSON::object();

  json["id"] = id;

  if (!data.frames.empty()) {
    json["animation"] = emit_fancy_tile_animation(data);
  }

  if (!data.objects.empty()) {
    auto dummy = JSON::object();
    dummy["draworder"] = "index";
    dummy["name"] = "";
    dummy["opacity"] = 1;
    dummy["type"] = "objectgroup";
    dummy["visible"] = true;
    dummy["x"] = 0;
    dummy["y"] = 0;

    auto objects = JSON::array();
    for (const auto& object_data : data.objects) {
      objects += emit_object(object_data);
    }

    dummy["objects"] = std::move(objects);
    json["objectgroup"] = std::move(dummy);
  }

  if (!data.context.properties.empty()) {
    json["properties"] = emit_properties(data.context);
  }

  return json;
}

[[nodiscard]] auto emit_fancy_tiles(const ir::TilesetData& data) -> JSON
{
  auto json = JSON::array();

  for (const auto& [id, tileData] : data.fancy_tiles) {
    json += emit_fancy_tile(id, tileData);
  }

  return json;
}

void add_common_tileset_attributes(JSON&                  json,
                                   const EmitInfo&        info,
                                   const ir::TilesetData& data)
{
  json["name"] = data.name;
  json["columns"] = data.column_count;

  json["tilewidth"] = data.tile_size.x;
  json["tileheight"] = data.tile_size.y;
  json["tilecount"] = data.tile_count;

  const auto image_path = fs::relative(data.image_path, info.destination_dir());
  json["image"] = convert_to_forward_slashes(image_path);

  json["imagewidth"] = data.image_size.x;
  json["imageheight"] = data.image_size.y;

  json["margin"] = 0;
  json["spacing"] = 0;

  if (!data.fancy_tiles.empty()) {
    json["tiles"] = emit_fancy_tiles(data);
  }

  if (!data.context.properties.empty()) {
    json["properties"] = emit_properties(data.context);
  }
}

[[nodiscard]] auto emit_embedded_tileset(const EmitInfo&        info,
                                         const ir::TilesetData& data) -> JSON
{
  auto json = JSON::object();

  json["firstgid"] = data.first_tile;
  add_common_tileset_attributes(json, info, data);

  return json;
}

[[nodiscard]] auto emit_external_tileset(const ir::TilesetData& data) -> JSON
{
  auto json = JSON::object();

  json["firstgid"] = data.first_tile;
  json["source"] = fmt::format("{}.json", data.name);

  return json;
}

void create_external_tileset_file(const EmitInfo& info, const ir::TilesetData& data)
{
  auto json = JSON::object();
  add_common_tileset_attributes(json, info, data);

  json["type"] = "tileset";
  json["tiledversion"] = tiled_version;
  json["version"] = tiled_json_format_version;

  const auto name = fmt::format("{}.json", data.name);
  const auto path = info.destination_dir() / name;

  write_json(json, path);
}

[[nodiscard]] auto emit_tileset(const EmitInfo& info, const ir::TilesetData& data) -> JSON
{
  if (get_preferences().embed_tilesets) {
    return emit_embedded_tileset(info, data);
  }
  else {
    create_external_tileset_file(info, data);
    return emit_external_tileset(data);
  }
}

[[nodiscard]] auto emit_tilesets(const EmitInfo& info) -> JSON
{
  auto json = JSON::array();

  const auto& data = info.data();
  for (const auto& tileset_data : data.tilesets) {
    json += emit_tileset(info, tileset_data);
  }

  return json;
}

}  // namespace

void emit_json_map(const EmitInfo& info)
{
  auto json = JSON::object();

  const auto& data = info.data();

  if (!data.component_definitions.empty()) {
    spdlog::warn("Component data will be ignored when saving the map as JSON!");
  }

  json["type"] = "map";
  json["width"] = data.col_count;
  json["height"] = data.row_count;

  json["tilewidth"] = data.tile_size.x;
  json["tileheight"] = data.tile_size.y;

  json["nextlayerid"] = data.next_layer_id;
  json["nextobjectid"] = data.next_object_id;

  json["infinite"] = false;
  json["orientation"] = "orthogonal";
  json["renderorder"] = "right-down";
  json["compressionlevel"] = -1;
  json["tiledversion"] = tiled_version;
  json["version"] = tiled_json_format_version;

  json["tilesets"] = emit_tilesets(info);
  json["layers"] = emit_layers(data);

  if (!data.context.properties.empty()) {
    json["properties"] = emit_properties(data.context);
  }

  write_json(json, info.destination_file());
}

}  // namespace tactile::io