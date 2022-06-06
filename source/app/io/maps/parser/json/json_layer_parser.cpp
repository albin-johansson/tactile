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

#include "core/utils/tiles.hpp"
#include "io/maps/ir.hpp"
#include "io/maps/json_utils.hpp"
#include "io/maps/parser/json/json_attribute_parser.hpp"

namespace tactile::io {
namespace {

[[nodiscard]] auto _parse_object_layer(const nlohmann::json& json,
                                       ir::LayerData& layerData) -> ParseError
{
  auto& objectLayerData = layerData.data.emplace<ir::ObjectLayerData>();

  if (const auto iter = json.find("objects"); iter != json.end()) {
    objectLayerData.objects.reserve(iter->size());

    for (const auto& [_, value] : iter->items()) {
      auto& objectData = objectLayerData.objects.emplace_back();
      if (const auto err = parse_object(value, objectData); err != ParseError::None) {
        return err;
      }
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto _parse_tile_layer_data(const nlohmann::json& json,
                                          TileMatrix& tiles,
                                          const usize columns) -> ParseError
{
  /* We only support the CSV tile encoding, which is the implicit default */
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

[[nodiscard]] auto _parse_tile_layer(const nlohmann::json& json,
                                     ir::LayerData& layerData,
                                     const usize rows,
                                     const usize columns) -> ParseError
{
  auto& tileLayerData = layerData.data.emplace<ir::TileLayerData>();

  if (const auto width = as_uint(json, "width")) {
    tileLayerData.col_count = *width;

    if (tileLayerData.col_count != columns) {
      spdlog::warn("JSON tile layer width does not match map width, '{}' vs '{}'",
                   tileLayerData.col_count,
                   columns);
    }
  }
  else {
    spdlog::warn("JSON tile layer has no width information, assuming map width...");
    tileLayerData.col_count = columns;
  }

  if (const auto height = as_uint(json, "height")) {
    tileLayerData.row_count = *height;

    if (tileLayerData.row_count != rows) {
      spdlog::warn("JSON tile layer height does not match map height, '{}' vs '{}'",
                   tileLayerData.row_count,
                   rows);
    }
  }
  else {
    spdlog::warn("JSON tile layer has no height information, assuming map height...");
    tileLayerData.row_count = rows;
  }

  tileLayerData.tiles =
      make_tile_matrix(tileLayerData.row_count, tileLayerData.col_count);

  if (const auto err =
          _parse_tile_layer_data(json, tileLayerData.tiles, tileLayerData.col_count);
      err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

[[nodiscard]] auto _parse_layer(const nlohmann::json& json,
                                ir::LayerData& layerData,
                                const usize index,
                                const usize rows,
                                const usize columns) -> ParseError
{
  layerData.index = index;

  if (const auto id = as_int(json, "id")) {
    layerData.id = *id;
  }
  else {
    return ParseError::NoLayerId;
  }

  layerData.name = as_string(json, "name").value_or("Layer");
  layerData.opacity = as_float(json, "opacity").value_or(1.0f);
  layerData.visible = as_bool(json, "visible").value_or(true);

  if (auto type = as_string(json, "type")) {
    if (type == "tilelayer") {
      layerData.type = LayerType::TileLayer;
      if (const auto err = _parse_tile_layer(json, layerData, rows, columns);
          err != ParseError::None) {
        return err;
      }
    }
    else if (type == "objectgroup") {
      layerData.type = LayerType::ObjectLayer;
      if (const auto err = _parse_object_layer(json, layerData);
          err != ParseError::None) {
        return err;
      }
    }
    else if (type == "group") {
      layerData.type = LayerType::GroupLayer;
      auto& groupLayerData = layerData.data.emplace<ir::GroupLayerData>();

      usize childIndex = 0;
      for (const auto& [_, value] : json.at("layers").items()) {
        auto& childLayerData =
            groupLayerData.children.emplace_back(std::make_unique<ir::LayerData>());

        if (const auto err =
                _parse_layer(value, *childLayerData, childIndex, rows, columns);
            err != ParseError::None) {
          return err;
        }

        ++childIndex;
      }
    }
    else {
      return ParseError::UnsupportedLayerType;
    }
  }
  else {
    return ParseError::NoLayerType;
  }

  if (const auto err = parse_properties(json, layerData.context);
      err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

}  // namespace

auto parse_object(const nlohmann::json& json, ir::ObjectData& objectData) -> ParseError
{
  if (const auto id = as_int(json, "id")) {
    objectData.id = *id;
  }
  else {
    return ParseError::NoObjectId;
  }

  objectData.name = as_string(json, "name").value_or("");
  objectData.tag = as_string(json, "type").value_or("");

  objectData.pos.x = as_float(json, "x").value_or(0.0f);
  objectData.pos.y = as_float(json, "y").value_or(0.0f);
  objectData.size.x = as_float(json, "width").value_or(0.0f);
  objectData.size.y = as_float(json, "height").value_or(0.0f);

  objectData.visible = as_bool(json, "visible").value_or(true);

  if (json.contains("point")) {
    objectData.type = ObjectType::Point;
  }
  else if (json.contains("ellipse")) {
    objectData.type = ObjectType::Ellipse;
  }
  else {
    objectData.type = ObjectType::Rect;
  }

  if (const auto err = parse_properties(json, objectData.context);
      err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

auto parse_layers(const nlohmann::json& json, ir::MapData& mapData) -> ParseError
{
  const auto iter = json.find("layers");

  if (iter == json.end()) {
    spdlog::warn("JSON map has no \"layers\" attribute, which is required!");
    return ParseError::None;
  }

  mapData.layers.reserve(iter->size());

  usize index = 0;
  for (const auto& [key, value] : iter->items()) {
    auto& layerData = mapData.layers.emplace_back();

    if (const auto err =
            _parse_layer(value, layerData, index, mapData.row_count, mapData.col_count);
        err != ParseError::None) {
      return err;
    }

    ++index;
  }

  return ParseError::None;
}

}  // namespace tactile::io