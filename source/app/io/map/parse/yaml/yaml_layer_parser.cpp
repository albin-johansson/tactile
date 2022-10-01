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
#include <utility>    // move

#include "core/type/expected.hpp"
#include "core/type/string.hpp"
#include "core/util/string.hpp"
#include "core/util/tiles.hpp"
#include "io/map/ir/ir.hpp"
#include "io/map/parse/yaml/yaml_parser.hpp"
#include "io/util/base64_tiles.hpp"
#include "io/util/yaml.hpp"

using namespace eastl::string_literals;

namespace tactile::io {
namespace {

[[nodiscard]] auto parse_layer(const YAML::Node& node,
                               const ir::MapData& map,
                               usize index) -> Expected<ir::LayerData, ParseError>;

[[nodiscard]] auto parse_plain_tile_layer_data(const String& tile_data,
                                               const usize rows,
                                               const usize columns)
    -> Expected<TileMatrix, ParseError>
{
  auto tiles = make_tile_matrix(rows, columns);

  usize index = 0;
  for (const auto& token : split(tile_data, ' ')) {
    if (const auto id = parse_i32(token)) {
      const auto [row, col] = to_matrix_coords(index, columns);
      tiles[row][col] = *id;
      ++index;
    }
    else {
      return error(ParseError::CorruptTileLayerData);
    }
  }

  return tiles;
}

[[nodiscard]] auto parse_tile_layer(const YAML::Node& node,
                                    const ir::MapData& map,
                                    const usize rows,
                                    const usize columns)
    -> Expected<ir::TileLayerData, ParseError>
{
  ir::TileLayerData tile_layer;
  tile_layer.row_count = rows;
  tile_layer.col_count = columns;

  String str_data;
  if (!read_attribute(node, "data", str_data)) {
    return error(ParseError::NoTileLayerData);
  }

  if (map.tile_format.encoding == TileEncoding::Plain) {
    std::replace(str_data.begin(), str_data.end(), '\n', ' ');
    if (auto matrix = parse_plain_tile_layer_data(str_data, rows, columns)) {
      tile_layer.tiles = std::move(*matrix);
    }
    else {
      return pass_on_error(matrix);
    }
  }
  else if (map.tile_format.encoding == TileEncoding::Base64) {
    tile_layer.tiles =
        base64_decode_tiles(str_data, rows, columns, map.tile_format.compression);
  }
  else {
    return error(ParseError::UnsupportedTileLayerEncoding);
  }

  return tile_layer;
}

[[nodiscard]] auto parse_object_layer(const YAML::Node& node, const ir::MapData& map)
    -> Expected<ir::ObjectLayerData, ParseError>
{
  ir::ObjectLayerData object_layer;

  if (auto sequence = node["objects"]) {
    object_layer.objects.reserve(sequence.size());

    for (const auto& object_node : sequence) {
      if (auto object = parse_object(object_node, map)) {
        object_layer.objects.push_back(std::move(*object));
      }
      else {
        return pass_on_error(object);
      }
    }
  }

  return object_layer;
}

[[nodiscard]] auto parse_group_layer(const YAML::Node& node, const ir::MapData& map)
    -> Expected<ir::GroupLayerData, ParseError>
{
  ir::GroupLayerData group;

  if (auto sequence = node["layers"]) {
    group.children.reserve(sequence.size());

    usize index = 0;
    for (const auto& layer_node : sequence) {
      if (auto child = parse_layer(layer_node, map, index)) {
        group.children.push_back(std::make_unique<ir::LayerData>(std::move(*child)));
      }
      else {
        return pass_on_error(child);
      }

      ++index;
    }
  }

  return group;
}

[[nodiscard]] auto parse_layer(const YAML::Node& node,
                               const ir::MapData& map,
                               const usize index) -> Expected<ir::LayerData, ParseError>
{
  ir::LayerData layer;
  layer.index = index;

  if (!read_attribute(node, "id", layer.id)) {
    return error(ParseError::NoLayerId);
  }

  read_attribute(node, "opacity", layer.opacity, 1.0f);
  read_attribute(node, "visible", layer.visible, true);
  read_attribute(node, "name", layer.name, "Layer"s);

  String type;
  if (!read_attribute(node, "type", type)) {
    return error(ParseError::NoLayerType);
  }

  if (type == "tile-layer") {
    layer.type = LayerType::TileLayer;
    if (auto tile_layer = parse_tile_layer(node, map, map.row_count, map.col_count)) {
      layer.data.emplace<ir::TileLayerData>(std::move(*tile_layer));
    }
    else {
      return pass_on_error(tile_layer);
    }
  }
  else if (type == "object-layer") {
    layer.type = LayerType::ObjectLayer;
    if (auto object_layer = parse_object_layer(node, map)) {
      layer.data.emplace<ir::ObjectLayerData>(std::move(*object_layer));
    }
    else {
      return pass_on_error(object_layer);
    }
  }
  else if (type == "group-layer") {
    layer.type = LayerType::GroupLayer;
    if (auto group = parse_group_layer(node, map)) {
      layer.data.emplace<ir::GroupLayerData>(std::move(*group));
    }
    else {
      return pass_on_error(group);
    }
  }
  else {
    return error(ParseError::UnsupportedLayerType);
  }

  if (auto context = parse_context(node, map)) {
    layer.context = std::move(*context);
  }
  else {
    return pass_on_error(context);
  }

  return layer;
}

}  // namespace

auto parse_object(const YAML::Node& node, const ir::MapData& map)
    -> Expected<ir::ObjectData, ParseError>
{
  ir::ObjectData object;

  if (!read_attribute(node, "id", object.id)) {
    return error(ParseError::NoObjectId);
  }

  String type;
  if (!read_attribute(node, "type", type)) {
    return error(ParseError::NoObjectType);
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
    return error(ParseError::UnsupportedObjectType);
  }

  read_attribute(node, "name", object.name);
  read_attribute(node, "tag", object.tag);

  read_attribute(node, "visible", object.visible, true);

  read_attribute(node, "x", object.pos.x, 0.0f);
  read_attribute(node, "y", object.pos.y, 0.0f);
  read_attribute(node, "width", object.size.x, 0.0f);
  read_attribute(node, "height", object.size.y, 0.0f);

  if (auto context = parse_context(node, map)) {
    object.context = std::move(*context);
  }
  else {
    return pass_on_error(context);
  }

  return object;
}

auto parse_layers(const YAML::Node& sequence, const ir::MapData& map)
    -> Expected<Vec<ir::LayerData>, ParseError>
{
  Vec<ir::LayerData> layers;
  layers.reserve(sequence.size());

  usize index = 0;
  for (const auto& layer_node : sequence) {
    if (auto layer = parse_layer(layer_node, map, index)) {
      layers.push_back(std::move(*layer));
    }
    else {
      return pass_on_error(layer);
    }

    ++index;
  }

  return layers;
}

}  // namespace tactile::io