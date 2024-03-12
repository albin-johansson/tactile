// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include <algorithm>  // replace
#include <utility>    // move

#include "core/tile/tile_matrix.hpp"
#include "io/ir/map/map_ir.hpp"
#include "io/map/parse/yaml/yaml_parser.hpp"
#include "io/util/base64_tiles.hpp"
#include "io/util/yaml.hpp"
#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/core/util/string_conv.hpp"
#include "tactile/core/util/string_ops.hpp"

using namespace std::string_literals;

namespace tactile {
namespace {

[[nodiscard]] auto parse_layer(const YAML::Node& node, const MapIR& map, usize index)
    -> Expected<LayerIR, ParseError>;

[[nodiscard]] auto parse_plain_tile_layer_data(const String& tile_data,
                                               const TileExtent extent)
    -> Expected<TileMatrix, ParseError>
{
  auto tiles = make_tile_matrix(extent);

  usize index = 0;
  const auto split_ok = split_string(tile_data, ' ', [&](const StringView token) {
    if (const auto id = parse_int(token)) {
      const auto [row, col] = to_matrix_coords(index, extent.cols);
      tiles[row][col] = *id;

      ++index;
      return true;
    }

    return false;
  });

  if (!split_ok) {
    return unexpected(ParseError::CorruptTileLayerData);
  }

  return tiles;
}

[[nodiscard]] auto parse_tile_layer(const YAML::Node& node,
                                    const MapIR& map,
                                    const TileExtent extent)
    -> Expected<TileLayerIR, ParseError>
{
  TileLayerIR tile_layer;
  tile_layer.extent = extent;

  String str_data;
  if (!read_attr(node, "data", str_data)) {
    return unexpected(ParseError::NoTileLayerData);
  }

  if (map.tile_format.encoding == TileEncoding::Plain) {
    std::replace(str_data.begin(), str_data.end(), '\n', ' ');
    if (auto matrix = parse_plain_tile_layer_data(str_data, extent)) {
      tile_layer.tiles = std::move(*matrix);
    }
    else {
      return propagate_unexpected(matrix);
    }
  }
  else if (map.tile_format.encoding == TileEncoding::Base64) {
    tile_layer.tiles = base64_decode_tiles(str_data, extent, map.tile_format.compression);
  }
  else {
    return unexpected(ParseError::UnsupportedTileLayerEncoding);
  }

  return tile_layer;
}

[[nodiscard]] auto parse_object_layer(const YAML::Node& node, const MapIR& map)
    -> Expected<ObjectLayerIR, ParseError>
{
  ObjectLayerIR object_layer;

  if (auto sequence = node["objects"]) {
    object_layer.objects.reserve(sequence.size());

    for (const auto& object_node: sequence) {
      if (auto object = parse_object(object_node, map)) {
        object_layer.objects.push_back(std::move(*object));
      }
      else {
        return propagate_unexpected(object);
      }
    }
  }

  return object_layer;
}

[[nodiscard]] auto parse_group_layer(const YAML::Node& node, const MapIR& map)
    -> Expected<GroupLayerIR, ParseError>
{
  GroupLayerIR group;

  if (auto sequence = node["layers"]) {
    group.children.reserve(sequence.size());

    usize index = 0;
    for (const auto& layer_node: sequence) {
      if (auto child = parse_layer(layer_node, map, index)) {
        group.children.push_back(std::make_unique<LayerIR>(std::move(*child)));
      }
      else {
        return propagate_unexpected(child);
      }

      ++index;
    }
  }

  return group;
}

[[nodiscard]] auto parse_layer(const YAML::Node& node,
                               const MapIR& map,
                               const usize index) -> Expected<LayerIR, ParseError>
{
  LayerIR layer;
  layer.index = index;

  if (!read_attr(node, "id", layer.id)) {
    return unexpected(ParseError::NoLayerId);
  }

  read_attr_or(node, "opacity", layer.opacity, 1.0f);
  read_attr_or(node, "visible", layer.visible, true);
  read_attr_or(node, "name", layer.name, "Layer"s);

  String type;
  if (!read_attr(node, "type", type)) {
    return unexpected(ParseError::NoLayerType);
  }

  if (type == "tile-layer") {
    layer.type = LayerType::TileLayer;
    if (auto tile_layer = parse_tile_layer(node, map, map.extent)) {
      layer.data.emplace<TileLayerIR>(std::move(*tile_layer));
    }
    else {
      return propagate_unexpected(tile_layer);
    }
  }
  else if (type == "object-layer") {
    layer.type = LayerType::ObjectLayer;
    if (auto object_layer = parse_object_layer(node, map)) {
      layer.data.emplace<ObjectLayerIR>(std::move(*object_layer));
    }
    else {
      return propagate_unexpected(object_layer);
    }
  }
  else if (type == "group-layer") {
    layer.type = LayerType::GroupLayer;
    if (auto group = parse_group_layer(node, map)) {
      layer.data.emplace<GroupLayerIR>(std::move(*group));
    }
    else {
      return propagate_unexpected(group);
    }
  }
  else {
    return unexpected(ParseError::UnsupportedLayerType);
  }

  if (auto context = parse_context(node, map)) {
    layer.context = std::move(*context);
  }
  else {
    return propagate_unexpected(context);
  }

  return layer;
}

}  // namespace

auto parse_object(const YAML::Node& node, const MapIR& map)
    -> Expected<ObjectIR, ParseError>
{
  ObjectIR object;

  if (!read_attr(node, "id", object.id)) {
    return unexpected(ParseError::NoObjectId);
  }

  String type;
  if (!read_attr(node, "type", type)) {
    return unexpected(ParseError::NoObjectType);
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
    return unexpected(ParseError::UnsupportedObjectType);
  }

  read_attr(node, "name", object.name);
  read_attr(node, "tag", object.tag);

  read_attr_or(node, "visible", object.visible, true);

  read_attr_or(node, "x", object.pos[0], 0.0f);
  read_attr_or(node, "y", object.pos[1], 0.0f);
  read_attr_or(node, "width", object.size[0], 0.0f);
  read_attr_or(node, "height", object.size[1], 0.0f);

  if (auto context = parse_context(node, map)) {
    object.context = std::move(*context);
  }
  else {
    return propagate_unexpected(context);
  }

  return object;
}

auto parse_layers(const YAML::Node& sequence, const MapIR& map)
    -> Expected<Vector<LayerIR>, ParseError>
{
  Vector<LayerIR> layers;
  layers.reserve(sequence.size());

  usize index = 0;
  for (const auto& layer_node: sequence) {
    if (auto layer = parse_layer(layer_node, map, index)) {
      layers.push_back(std::move(*layer));
    }
    else {
      return propagate_unexpected(layer);
    }

    ++index;
  }

  return layers;
}

}  // namespace tactile
