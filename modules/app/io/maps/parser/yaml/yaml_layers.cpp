#include "yaml_layers.hpp"

#include <algorithm>  // replace
#include <memory>     // make_unique
#include <string>     // string

#include "core/utils/strings.hpp"
#include "core/utils/tiles.hpp"
#include "yaml_attributes.hpp"

namespace tactile::parsing {
namespace {

[[nodiscard]] auto _parse_layer(const YAML::Node& node,
                                const ir::map_data& map,
                                ir::layer_data& layer,
                                usize index) -> parse_error;

[[nodiscard]] auto _parse_tile_layer_data(ir::tile_layer_data& layer,
                                          const usize columns,
                                          const std::string& tileData) -> parse_error
{
  usize index = 0;
  for (const auto& token : split(tileData.c_str(), ' ')) {
    if (const auto id = from_string<tile_id>(token.c_str())) {
      const auto [row, col] = to_matrix_coords(index, columns);
      layer.tiles[row][col] = *id;
      ++index;
    }
    else {
      return parse_error::corrupt_tile_layer_data;
    }
  }

  return parse_error::none;
}

[[nodiscard]] auto _parse_tile_layer(const YAML::Node& node,
                                     ir::layer_data& data,
                                     const usize rows,
                                     const usize columns) -> parse_error
{
  data.type = layer_type::tile_layer;

  auto& tileLayer = data.data.emplace<ir::tile_layer_data>();
  tileLayer.row_count = rows;
  tileLayer.col_count = columns;
  tileLayer.tiles = make_tile_matrix(rows, columns);

  if (auto tiles = node["data"]) {
    auto rawTiles = tiles.as<std::string>();
    std::replace(rawTiles.begin(), rawTiles.end(), '\n', ' ');

    if (const auto err = _parse_tile_layer_data(tileLayer, columns, rawTiles);
        err != parse_error::none) {
      return err;
    }
  }
  else {
    return parse_error::no_tile_layer_data;
  }

  return parse_error::none;
}

[[nodiscard]] auto _parse_object_layer(const YAML::Node& node,
                                       const ir::map_data& map,
                                       ir::layer_data& data) -> parse_error
{
  data.type = layer_type::object_layer;
  auto& objectLayer = data.data.emplace<ir::object_layer_data>();

  if (auto sequence = node["objects"]) {
    objectLayer.objects.reserve(sequence.size());

    for (const auto& objectNode : sequence) {
      auto& object = objectLayer.objects.emplace_back();
      if (const auto err = parse_object(objectNode, map, &object);
          err != parse_error::none) {
        return err;
      }
    }
  }

  return parse_error::none;
}

[[nodiscard]] auto _parse_group_layer(const YAML::Node& node,
                                      const ir::map_data& map,
                                      ir::layer_data& data) -> parse_error
{
  data.type = layer_type::group_layer;
  auto& group = data.data.emplace<ir::group_layer_data>();

  if (auto sequence = node["layers"]) {
    group.children.reserve(sequence.size());

    usize index = 0;
    for (const auto& layerNode : sequence) {
      auto& child = group.children.emplace_back(std::make_unique<ir::layer_data>());

      if (const auto err = _parse_layer(layerNode, map, *child, index);
          err != parse_error::none) {
        return err;
      }

      ++index;
    }
  }

  return parse_error::none;
}

[[nodiscard]] auto _parse_layer(const YAML::Node& node,
                                const ir::map_data& map,
                                ir::layer_data& layer,
                                const usize index) -> parse_error
{
  layer.index = index;

  if (auto identifier = node["id"]) {
    layer.id = identifier.as<layer_id>();
  }
  else {
    return parse_error::no_layer_id;
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
          err != parse_error::none) {
        return err;
      }
    }
    else if (value == "object-layer") {
      if (const auto err = _parse_object_layer(node, map, layer);
          err != parse_error::none) {
        return err;
      }
    }
    else if (value == "group-layer") {
      if (const auto err = _parse_group_layer(node, map, layer);
          err != parse_error::none) {
        return err;
      }
    }
    else {
      return parse_error::unsupported_layer_type;
    }
  }
  else {
    return parse_error::no_layer_type;
  }

  if (const auto err = parse_properties(node, layer.context); err != parse_error::none) {
    return err;
  }

  if (const auto err = parse_components(node, map, layer.context);
      err != parse_error::none) {
    return err;
  }

  return parse_error::none;
}

}  // namespace

auto parse_object(const YAML::Node& node,
                  const ir::map_data& map,
                  ir::object_data* object) -> parse_error
{
  if (auto id = node["id"]) {
    object->id = id.as<object_id>();
  }
  else {
    return parse_error::no_object_id;
  }

  if (auto type = node["type"]) {
    const auto str = type.as<std::string>();
    if (str == "point") {
      object->type = object_type::point;
    }
    else if (str == "rect") {
      object->type = object_type::rect;
    }
    else if (str == "ellipse") {
      object->type = object_type::ellipse;
    }
    else {
      return parse_error::unsupported_object_type;
    }
  }
  else {
    return parse_error::no_object_type;
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

  if (const auto err = parse_properties(node, object->context);
      err != parse_error::none) {
    return err;
  }

  if (const auto err = parse_components(node, map, object->context);
      err != parse_error::none) {
    return err;
  }

  return parse_error::none;
}

auto parse_layers(const YAML::Node& sequence, ir::map_data& map) -> parse_error
{
  map.layers.reserve(sequence.size());

  usize index = 0;
  for (const auto& layerNode : sequence) {
    auto& layer = map.layers.emplace_back();
    if (const auto err = _parse_layer(layerNode, map, layer, index);
        err != parse_error::none) {
      return err;
    }

    ++index;
  }

  return parse_error::none;
}

}  // namespace tactile::parsing