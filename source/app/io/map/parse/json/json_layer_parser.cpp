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

#include "json_layer_parser.hpp"

#include <spdlog/spdlog.h>

#include "core/util/tiles.hpp"
#include "io/map/ir/ir.hpp"
#include "io/map/parse/json/json_attribute_parser.hpp"

namespace tactile::io {
namespace {

[[nodiscard]] auto _parse_object_layer(const JSON& json, ir::LayerData& layer_data)
    -> ParseError
{
  auto& object_layer_data = layer_data.data.emplace<ir::ObjectLayerData>();

  if (const auto iter = json.find("objects"); iter != json.end()) {
    object_layer_data.objects.reserve(iter->size());

    for (const auto& [_, value] : iter->items()) {
      auto& object_data = object_layer_data.objects.emplace_back();
      if (const auto err = parse_object(value, object_data); err != ParseError::None) {
        return err;
      }
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto _parse_tile_layer_data(const JSON& json,
                                          TileMatrix& tiles,
                                          const usize columns) -> ParseError
{
  // We only support the CSV tile encoding, which is the implicit default
  if (auto encoding = as_string(json, "encoding")) {
    if (encoding != "csv") {
      return ParseError::UnsupportedTileLayerEncoding;
    }
  }

  if (!json.contains("data")) {
    return ParseError::NoTileLayerData;
  }

  const auto data = json.at("data");
  if (!data.is_array()) {
    return ParseError::CorruptTileLayerData;
  }

  usize index = 0;
  for (const auto& [_, value] : data.items()) {
    if (value.is_number_integer()) {
      const auto [row, col] = to_matrix_coords(index, columns);
      tiles[row][col] = value.get<TileID>();
      ++index;
    }
    else {
      return ParseError::CorruptTileLayerData;
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto _parse_tile_layer(const JSON&    json,
                                     ir::LayerData& layer_data,
                                     const usize    rows,
                                     const usize    columns) -> ParseError
{
  auto& tile_layer_data = layer_data.data.emplace<ir::TileLayerData>();

  if (const auto width = as_uint(json, "width")) {
    tile_layer_data.col_count = *width;

    if (tile_layer_data.col_count != columns) {
      spdlog::warn("JSON tile layer width does not match map width, '{}' vs '{}'",
                   tile_layer_data.col_count,
                   columns);
    }
  }
  else {
    spdlog::warn("JSON tile layer has no width information, assuming map width...");
    tile_layer_data.col_count = columns;
  }

  if (const auto height = as_uint(json, "height")) {
    tile_layer_data.row_count = *height;

    if (tile_layer_data.row_count != rows) {
      spdlog::warn("JSON tile layer height does not match map height, '{}' vs '{}'",
                   tile_layer_data.row_count,
                   rows);
    }
  }
  else {
    spdlog::warn("JSON tile layer has no height information, assuming map height...");
    tile_layer_data.row_count = rows;
  }

  tile_layer_data.tiles =
      make_tile_matrix(tile_layer_data.row_count, tile_layer_data.col_count);

  if (const auto err =
          _parse_tile_layer_data(json, tile_layer_data.tiles, tile_layer_data.col_count);
      err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

[[nodiscard]] auto _parse_layer(const JSON&    json,
                                ir::LayerData& layer_data,
                                const usize    index,
                                const usize    rows,
                                const usize    columns) -> ParseError
{
  layer_data.index = index;

  if (const auto id = as_int(json, "id")) {
    layer_data.id = *id;
  }
  else {
    return ParseError::NoLayerId;
  }

  layer_data.name = as_string(json, "name").value_or("Layer");
  layer_data.opacity = as_float(json, "opacity").value_or(1.0f);
  layer_data.visible = as_bool(json, "visible").value_or(true);

  if (auto type = as_string(json, "type")) {
    if (type == "tilelayer") {
      layer_data.type = LayerType::TileLayer;
      if (const auto err = _parse_tile_layer(json, layer_data, rows, columns);
          err != ParseError::None) {
        return err;
      }
    }
    else if (type == "objectgroup") {
      layer_data.type = LayerType::ObjectLayer;
      if (const auto err = _parse_object_layer(json, layer_data);
          err != ParseError::None) {
        return err;
      }
    }
    else if (type == "group") {
      layer_data.type = LayerType::GroupLayer;
      auto& group_layer_data = layer_data.data.emplace<ir::GroupLayerData>();

      usize child_index = 0;
      for (const auto& [_, value] : json.at("layers").items()) {
        auto& child_layer_data =
            group_layer_data.children.emplace_back(std::make_unique<ir::LayerData>());

        if (const auto err =
                _parse_layer(value, *child_layer_data, child_index, rows, columns);
            err != ParseError::None) {
          return err;
        }

        ++child_index;
      }
    }
    else {
      return ParseError::UnsupportedLayerType;
    }
  }
  else {
    return ParseError::NoLayerType;
  }

  if (const auto err = parse_properties(json, layer_data.context);
      err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

}  // namespace

auto parse_object(const JSON& json, ir::ObjectData& object_data) -> ParseError
{
  if (const auto id = as_int(json, "id")) {
    object_data.id = *id;
  }
  else {
    return ParseError::NoObjectId;
  }

  object_data.name = as_string(json, "name").value_or("");
  object_data.tag = as_string(json, "type").value_or("");

  object_data.pos.x = as_float(json, "x").value_or(0.0f);
  object_data.pos.y = as_float(json, "y").value_or(0.0f);
  object_data.size.x = as_float(json, "width").value_or(0.0f);
  object_data.size.y = as_float(json, "height").value_or(0.0f);

  object_data.visible = as_bool(json, "visible").value_or(true);

  if (json.contains("point")) {
    object_data.type = ObjectType::Point;
  }
  else if (json.contains("ellipse")) {
    object_data.type = ObjectType::Ellipse;
  }
  else {
    object_data.type = ObjectType::Rect;
  }

  if (const auto err = parse_properties(json, object_data.context);
      err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

auto parse_layers(const JSON& json, ir::MapData& map_data) -> ParseError
{
  const auto iter = json.find("layers");

  if (iter == json.end()) {
    spdlog::warn("JSON map has no \"layers\" attribute, which is required!");
    return ParseError::None;
  }

  map_data.layers.reserve(iter->size());

  usize index = 0;
  for (const auto& [key, value] : iter->items()) {
    auto& layer_data = map_data.layers.emplace_back();

    if (const auto err = _parse_layer(value,
                                      layer_data,
                                      index,
                                      map_data.row_count,
                                      map_data.col_count);
        err != ParseError::None) {
      return err;
    }

    ++index;
  }

  return ParseError::None;
}

}  // namespace tactile::io