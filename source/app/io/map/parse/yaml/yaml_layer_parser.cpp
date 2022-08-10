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

#include <algorithm>  // replace
#include <string>     // string

#include "core/util/string.hpp"
#include "core/util/tiles.hpp"
#include "io/map/ir/ir.hpp"
#include "io/map/parse/yaml/yaml_parser.hpp"
#include "io/util/tile_format.hpp"
#include "io/util/yaml.hpp"

using namespace std::string_literals;

namespace tactile::io {
namespace {

[[nodiscard]] auto parse_layer(const YAML::Node&  node,
                               const ir::MapData& map,
                               ir::LayerData&     layer,
                               usize              index) -> ParseError;

[[nodiscard]] auto parse_plain_tile_layer_data(ir::TileLayerData& layer,
                                               const usize        columns,
                                               const std::string& tile_data) -> ParseError
{
  usize index = 0;
  for (const auto& token : split(tile_data.c_str(), ' ')) {
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

[[nodiscard]] auto parse_tile_layer(const YAML::Node&  node,
                                    const ir::MapData& map,
                                    ir::LayerData&     layer,
                                    const usize        rows,
                                    const usize        columns) -> ParseError
{
  layer.type = LayerType::TileLayer;

  auto& tile_layer = layer.data.emplace<ir::TileLayerData>();
  tile_layer.row_count = rows;
  tile_layer.col_count = columns;
  tile_layer.tiles = make_tile_matrix(rows, columns);

  std::string data;
  if (!read_attribute(node, "data", data)) {
    return ParseError::NoTileLayerData;
  }

  if (map.tile_format.encoding == TileEncoding::Plain) {
    std::replace(data.begin(), data.end(), '\n', ' ');
    if (const auto err = parse_plain_tile_layer_data(tile_layer, columns, data);
        err != ParseError::None) {
      return err;
    }
  }
  else if (map.tile_format.encoding == TileEncoding::Base64) {
    tile_layer.tiles =
        base64_decode_tiles(data, rows, columns, map.tile_format.compression);
  }
  else {
    // TODO
    return ParseError::Unknown;
  }

  return ParseError::None;
}

[[nodiscard]] auto parse_object_layer(const YAML::Node&  node,
                                      const ir::MapData& map,
                                      ir::LayerData&     data) -> ParseError
{
  data.type = LayerType::ObjectLayer;
  auto& object_layer = data.data.emplace<ir::ObjectLayerData>();

  if (auto sequence = node["objects"]) {
    object_layer.objects.reserve(sequence.size());

    for (const auto& object_node : sequence) {
      auto& object = object_layer.objects.emplace_back();
      if (const auto err = parse_object(object_node, map, object);
          err != ParseError::None) {
        return err;
      }
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto parse_group_layer(const YAML::Node&  node,
                                     const ir::MapData& map,
                                     ir::LayerData&     data) -> ParseError
{
  data.type = LayerType::GroupLayer;
  auto& group = data.data.emplace<ir::GroupLayerData>();

  if (auto sequence = node["layers"]) {
    group.children.reserve(sequence.size());

    usize index = 0;
    for (const auto& layer_node : sequence) {
      auto& child = group.children.emplace_back(std::make_unique<ir::LayerData>());

      if (const auto err = parse_layer(layer_node, map, *child, index);
          err != ParseError::None) {
        return err;
      }

      ++index;
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto parse_layer(const YAML::Node&  node,
                               const ir::MapData& map,
                               ir::LayerData&     layer,
                               const usize        index) -> ParseError
{
  layer.index = index;

  if (!read_attribute(node, "id", layer.id)) {
    return ParseError::NoLayerId;
  }

  read_attribute(node, "opacity", layer.opacity, 1.0f);
  read_attribute(node, "visible", layer.visible, true);
  read_attribute(node, "name", layer.name, "Layer"s);

  std::string type;
  if (!read_attribute(node, "type", type)) {
    return ParseError::NoLayerType;
  }

  if (type == "tile-layer") {
    if (const auto err = parse_tile_layer(node, map, layer, map.row_count, map.col_count);
        err != ParseError::None) {
      return err;
    }
  }
  else if (type == "object-layer") {
    if (const auto err = parse_object_layer(node, map, layer); err != ParseError::None) {
      return err;
    }
  }
  else if (type == "group-layer") {
    if (const auto err = parse_group_layer(node, map, layer); err != ParseError::None) {
      return err;
    }
  }
  else {
    return ParseError::UnsupportedLayerType;
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

auto parse_object(const YAML::Node& node, const ir::MapData& map, ir::ObjectData& object)
    -> ParseError
{
  if (!read_attribute(node, "id", object.id)) {
    return ParseError::NoObjectId;
  }

  std::string type;
  if (!read_attribute(node, "type", type)) {
    return ParseError::NoObjectType;
  }

  if (type == "point") {
    object.type = ObjectType::Point;
  }
  else if (type == "rect") {
    object.type = ObjectType::Rect;
  }
  else if (type == "ellipse") {
    object.type = ObjectType::Ellipse;
  }
  else {
    return ParseError::UnsupportedObjectType;
  }

  read_attribute(node, "name", object.name);
  read_attribute(node, "tag", object.tag);

  read_attribute(node, "visible", object.visible, true);

  read_attribute(node, "x", object.pos.x, 0.0f);
  read_attribute(node, "y", object.pos.y, 0.0f);
  read_attribute(node, "width", object.size.x, 0.0f);
  read_attribute(node, "height", object.size.y, 0.0f);

  if (const auto err = parse_properties(node, object.context); err != ParseError::None) {
    return err;
  }

  if (const auto err = parse_components(node, map, object.context);
      err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

auto parse_layers(const YAML::Node& sequence, ir::MapData& map) -> ParseError
{
  map.layers.reserve(sequence.size());

  usize index = 0;
  for (const auto& layer_node : sequence) {
    auto& layer = map.layers.emplace_back();

    if (const auto err = parse_layer(layer_node, map, layer, index);
        err != ParseError::None) {
      return err;
    }

    ++index;
  }

  return ParseError::None;
}

}  // namespace tactile::io