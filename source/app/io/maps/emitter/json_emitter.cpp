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

#include "json_emitter.hpp"

#include <utility>  // move
#include <variant>  // get

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include "core/utils/strings.hpp"
#include "emit_info.hpp"
#include "io/maps/json_utils.hpp"
#include "io/maps/tiled_info.hpp"
#include "io/persistence/preferences.hpp"
#include "misc/logging.hpp"

namespace tactile::emitter {
namespace {

[[nodiscard]] auto _emit_properties(const ir::AttributeContextData& data)
    -> nlohmann::json
{
  auto array = nlohmann::json::array();

  for (const auto& [name, value] : data.properties) {
    auto json = nlohmann::json::object();

    json["name"] = name;
    json["type"] = value.type();
    json["value"] = value;

    array += json;
  }

  return array;
}

[[nodiscard]] auto _emit_object(const ir::ObjectData& data) -> nlohmann::json
{
  auto json = nlohmann::json::object();

  json["id"] = data.id;
  json["name"] = data.name;
  json["x"] = data.x;
  json["y"] = data.y;
  json["width"] = data.width;
  json["height"] = data.height;
  json["visible"] = data.visible;
  json["type"] = data.tag;
  json["rotation"] = 0;

  switch (data.type) {
    case object_type::rect:
      /* Assumed to be rectangle if no point/ellipse indicators */
      break;

    case object_type::point: {
      json["point"] = true;
      break;
    }
    case object_type::ellipse: {
      json["ellipse"] = true;
      break;
    }
  }

  if (!data.context.properties.empty()) {
    json["properties"] = _emit_properties(data.context);
  }

  return json;
}

[[nodiscard]] auto _emit_layer(const ir::LayerData& data,
                               const usize rows,
                               const usize columns) -> nlohmann::json
{
  auto json = nlohmann::json::object();

  json["id"] = data.id;
  json["name"] = data.name;
  json["opacity"] = data.opacity;
  json["visible"] = data.visible;
  json["x"] = 0;
  json["y"] = 0;

  switch (data.type) {
    case LayerType::tile_layer: {
      const auto& tileLayerData = std::get<ir::TileLayerData>(data.data);

      json["type"] = "tilelayer";
      json["width"] = columns;
      json["height"] = rows;

      auto tiles = nlohmann::json::array();

      for (usize row = 0; row < rows; ++row) {
        for (usize col = 0; col < columns; ++col) {
          tiles += tileLayerData.tiles[row][col];
        }
      }

      json["data"] = std::move(tiles);
      break;
    }
    case LayerType::object_layer: {
      const auto& objectLayerData = std::get<ir::ObjectLayerData>(data.data);

      json["type"] = "objectgroup";

      auto objects = nlohmann::json::array();

      for (const auto& objectData : objectLayerData.objects) {
        objects += _emit_object(objectData);
      }

      json["objects"] = std::move(objects);
      break;
    }
    case LayerType::group_layer: {
      const auto& groupLayerData = std::get<ir::GroupLayerData>(data.data);

      json["type"] = "group";

      auto layers = nlohmann::json::array();

      for (const auto& childLayerData : groupLayerData.children) {
        layers += _emit_layer(*childLayerData, rows, columns);
      }

      json["layers"] = std::move(layers);
      break;
    }
  }

  if (!data.context.properties.empty()) {
    json["properties"] = _emit_properties(data.context);
  }

  return json;
}

[[nodiscard]] auto _emit_layers(const ir::MapData& data) -> nlohmann::json
{
  auto array = nlohmann::json::array();

  for (const auto& layerData : data.layers) {
    array += _emit_layer(layerData, data.row_count, data.col_count);
  }

  return array;
}

[[nodiscard]] auto _emit_fancy_tile_animation(const ir::MetaTileData& data)
    -> nlohmann::json
{
  auto array = nlohmann::json::array();

  for (const auto& frameData : data.frames) {
    auto json = nlohmann::json::object();

    json["tileid"] = frameData.local_id;
    json["duration"] = frameData.duration_ms;

    array += json;
  }

  return array;
}

[[nodiscard]] auto _emit_fancy_tile(const tile_id id, const ir::MetaTileData& data)
    -> nlohmann::json
{
  auto json = nlohmann::json::object();

  json["id"] = id;

  if (!data.frames.empty()) {
    json["animation"] = _emit_fancy_tile_animation(data);
  }

  if (!data.objects.empty()) {
    auto dummy = nlohmann::json::object();
    dummy["draworder"] = "index";
    dummy["name"] = "";
    dummy["opacity"] = 1;
    dummy["type"] = "objectgroup";
    dummy["visible"] = true;
    dummy["x"] = 0;
    dummy["y"] = 0;

    auto objects = nlohmann::json::array();
    for (const auto& objectData : data.objects) {
      objects += _emit_object(objectData);
    }

    dummy["objects"] = std::move(objects);
    json["objectgroup"] = std::move(dummy);
  }

  if (!data.context.properties.empty()) {
    json["properties"] = _emit_properties(data.context);
  }

  return json;
}

[[nodiscard]] auto _emit_fancy_tiles(const ir::TilesetData& data) -> nlohmann::json
{
  auto json = nlohmann::json::array();

  for (const auto& [id, tileData] : data.fancy_tiles) {
    json += _emit_fancy_tile(id, tileData);
  }

  return json;
}

void _add_common_tileset_attributes(nlohmann::json& json,
                                    const emit_info& info,
                                    const ir::TilesetData& data)
{
  json["name"] = data.name;
  json["columns"] = data.column_count;

  json["tilewidth"] = data.tile_width;
  json["tileheight"] = data.tile_height;
  json["tilecount"] = data.tile_count;

  const auto imagePath =
      std::filesystem::relative(data.image_path, info.destination_dir());
  json["image"] = convert_to_forward_slashes(imagePath);

  json["imagewidth"] = data.image_width;
  json["imageheight"] = data.image_height;

  json["margin"] = 0;
  json["spacing"] = 0;

  if (!data.fancy_tiles.empty()) {
    json["tiles"] = _emit_fancy_tiles(data);
  }

  if (!data.context.properties.empty()) {
    json["properties"] = _emit_properties(data.context);
  }
}

[[nodiscard]] auto _emit_embedded_tileset(const emit_info& info,
                                          const ir::TilesetData& data) -> nlohmann::json
{
  auto json = nlohmann::json::object();

  json["firstgid"] = data.first_tile;
  _add_common_tileset_attributes(json, info, data);

  return json;
}

[[nodiscard]] auto _emit_external_tileset(const ir::TilesetData& data) -> nlohmann::json
{
  auto json = nlohmann::json::object();

  json["firstgid"] = data.first_tile;
  json["source"] = fmt::format("{}.json", data.name);

  return json;
}

void _create_external_tileset_file(const emit_info& info, const ir::TilesetData& data)
{
  auto json = nlohmann::json::object();
  _add_common_tileset_attributes(json, info, data);

  json["type"] = "tileset";
  json["tiledversion"] = tiled_version;
  json["version"] = tiled_json_format_version;

  const auto name = fmt::format("{}.json", data.name);
  const auto path = info.destination_dir() / name;

  write_json(json, path);
}

[[nodiscard]] auto _emit_tileset(const emit_info& info, const ir::TilesetData& data)
    -> nlohmann::json
{
  if (get_preferences().embed_tilesets()) {
    return _emit_embedded_tileset(info, data);
  }
  else {
    _create_external_tileset_file(info, data);
    return _emit_external_tileset(data);
  }
}

[[nodiscard]] auto _emit_tilesets(const emit_info& info) -> nlohmann::json
{
  auto json = nlohmann::json::array();

  const auto& data = info.data();
  for (const auto& tilesetData : data.tilesets) {
    json += _emit_tileset(info, tilesetData);
  }

  return json;
}

}  // namespace

void emit_json_map(const emit_info& info)
{
  auto json = nlohmann::json::object();

  const auto& data = info.data();

  if (!data.component_definitions.empty()) {
    log_warning("Component data will be ignored when saving the map as JSON!");
  }

  json["type"] = "map";
  json["width"] = data.col_count;
  json["height"] = data.row_count;

  json["tilewidth"] = data.tile_width;
  json["tileheight"] = data.tile_height;

  json["nextlayerid"] = data.next_layer_id;
  json["nextobjectid"] = data.next_object_id;

  json["infinite"] = false;
  json["orientation"] = "orthogonal";
  json["renderorder"] = "right-down";
  json["compressionlevel"] = -1;
  json["tiledversion"] = tiled_version;
  json["version"] = tiled_json_format_version;

  json["tilesets"] = _emit_tilesets(info);
  json["layers"] = _emit_layers(data);

  if (!data.context.properties.empty()) {
    json["properties"] = _emit_properties(data.context);
  }

  write_json(json, info.destination_file());
}

}  // namespace tactile::emitter