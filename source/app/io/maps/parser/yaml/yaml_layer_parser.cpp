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

#include "yaml_layer_parser.hpp"

#include <algorithm>  // replace
#include <string>     // string

#include "core/utils/strings.hpp"
#include "core/utils/tiles.hpp"
#include "io/maps/ir.hpp"
#include "io/maps/parser/yaml/yaml_attribute_parser.hpp"

namespace tactile::parsing {
namespace {

[[nodiscard]] auto _parse_layer(const YAML::Node& node,
                                const ir::MapData& map,
                                ir::LayerData& layer,
                                usize index) -> ParseError;

[[nodiscard]] auto _parse_tile_layer_data(ir::TileLayerData& layer,
                                          const usize columns,
                                          const std::string& tileData) -> ParseError
{
  usize index = 0;
  for (const auto& token : split(tileData.c_str(), ' ')) {
    if (const auto id = from_string<TileID>(token.c_str())) {
      const auto [row, col] = to_matrix_coords(index, columns);
      layer.tiles[row][col] = *id;
      ++index;
    }
    else {
      return ParseError::CorruptTileLayerData;
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto _parse_tile_layer(const YAML::Node& node,
                                     ir::LayerData& data,
                                     const usize rows,
                                     const usize columns) -> ParseError
{
  data.type = LayerType::TileLayer;

  auto& tileLayer = data.data.emplace<ir::TileLayerData>();
  tileLayer.row_count = rows;
  tileLayer.col_count = columns;
  tileLayer.tiles = make_tile_matrix(rows, columns);

  if (auto tiles = node["data"]) {
    auto rawTiles = tiles.as<std::string>();
    std::replace(rawTiles.begin(), rawTiles.end(), '\n', ' ');

    if (const auto err = _parse_tile_layer_data(tileLayer, columns, rawTiles);
        err != ParseError::None) {
      return err;
    }
  }
  else {
    return ParseError::NoTileLayerData;
  }

  return ParseError::None;
}

[[nodiscard]] auto _parse_object_layer(const YAML::Node& node,
                                       const ir::MapData& map,
                                       ir::LayerData& data) -> ParseError
{
  data.type = LayerType::ObjectLayer;
  auto& objectLayer = data.data.emplace<ir::ObjectLayerData>();

  if (auto sequence = node["objects"]) {
    objectLayer.objects.reserve(sequence.size());

    for (const auto& objectNode : sequence) {
      auto& object = objectLayer.objects.emplace_back();
      if (const auto err = parse_object(objectNode, map, &object);
          err != ParseError::None) {
        return err;
      }
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto _parse_group_layer(const YAML::Node& node,
                                      const ir::MapData& map,
                                      ir::LayerData& data) -> ParseError
{
  data.type = LayerType::GroupLayer;
  auto& group = data.data.emplace<ir::GroupLayerData>();

  if (auto sequence = node["layers"]) {
    group.children.reserve(sequence.size());

    usize index = 0;
    for (const auto& layerNode : sequence) {
      auto& child = group.children.emplace_back(std::make_unique<ir::LayerData>());

      if (const auto err = _parse_layer(layerNode, map, *child, index);
          err != ParseError::None) {
        return err;
      }

      ++index;
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto _parse_layer(const YAML::Node& node,
                                const ir::MapData& map,
                                ir::LayerData& layer,
                                const usize index) -> ParseError
{
  layer.index = index;

  if (auto identifier = node["id"]) {
    layer.id = identifier.as<LayerID>();
  }
  else {
    return ParseError::NoLayerId;
  }

  if (auto opacity = node["opacity"]) {
    layer.opacity = opacity.as<float>();
  }
  else {
    layer.opacity = 1.0f;
  }

  if (auto visible = node["visible"]) {
    layer.visible = visible.as<bool>();
  }
  else {
    layer.visible = true;
  }

  if (auto name = node["name"]) {
    layer.name = name.as<std::string>();
  }
  else {
    layer.name = "Layer";
  }

  if (auto type = node["type"]) {
    const auto value = type.as<std::string>();

    if (value == "tile-layer") {
      if (const auto err = _parse_tile_layer(node, layer, map.row_count, map.col_count);
          err != ParseError::None) {
        return err;
      }
    }
    else if (value == "object-layer") {
      if (const auto err = _parse_object_layer(node, map, layer);
          err != ParseError::None) {
        return err;
      }
    }
    else if (value == "group-layer") {
      if (const auto err = _parse_group_layer(node, map, layer);
          err != ParseError::None) {
        return err;
      }
    }
    else {
      return ParseError::UnsupportedLayerType;
    }
  }
  else {
    return ParseError::NoLayerType;
  }

  if (const auto err = parse_properties(node, layer.context); err != ParseError::None) {
    return err;
  }

  if (const auto err = parse_components(node, map, layer.context);
      err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

}  // namespace

auto parse_object(const YAML::Node& node, const ir::MapData& map, ir::ObjectData* object)
    -> ParseError
{
  if (auto id = node["id"]) {
    object->id = id.as<ObjectID>();
  }
  else {
    return ParseError::NoObjectId;
  }

  if (auto type = node["type"]) {
    const auto str = type.as<std::string>();
    if (str == "point") {
      object->type = ObjectType::Point;
    }
    else if (str == "rect") {
      object->type = ObjectType::Rect;
    }
    else if (str == "ellipse") {
      object->type = ObjectType::Ellipse;
    }
    else {
      return ParseError::UnsupportedObjectType;
    }
  }
  else {
    return ParseError::NoObjectType;
  }

  if (auto name = node["name"]) {
    object->name = name.as<std::string>();
  }

  if (auto tag = node["tag"]) {
    object->tag = tag.as<std::string>();
  }

  if (auto visible = node["visible"]) {
    object->visible = visible.as<bool>();
  }
  else {
    object->visible = true;
  }

  if (auto x = node["x"]) {
    object->x = x.as<float>();
  }
  else {
    object->x = 0;
  }

  if (auto y = node["y"]) {
    object->y = y.as<float>();
  }
  else {
    object->y = 0;
  }

  if (auto width = node["width"]) {
    object->width = width.as<float>();
  }
  else {
    object->width = 0;
  }

  if (auto height = node["height"]) {
    object->height = height.as<float>();
  }
  else {
    object->height = 0;
  }

  if (const auto err = parse_properties(node, object->context); err != ParseError::None) {
    return err;
  }

  if (const auto err = parse_components(node, map, object->context);
      err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

auto parse_layers(const YAML::Node& sequence, ir::MapData& map) -> ParseError
{
  map.layers.reserve(sequence.size());

  usize index = 0;
  for (const auto& layerNode : sequence) {
    auto& layer = map.layers.emplace_back();
    if (const auto err = _parse_layer(layerNode, map, layer, index);
        err != ParseError::None) {
      return err;
    }

    ++index;
  }

  return ParseError::None;
}

}  // namespace tactile::parsing