/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include <cstring>  // strcmp
#include <utility>  // move

#include <spdlog/spdlog.h>

#include "common/debug/panic.hpp"
#include "common/tile_matrix.hpp"
#include "common/type/string.hpp"
#include "common/type/vector.hpp"
#include "common/util/strings.hpp"
#include "io/base64_tiles.hpp"
#include "io/ir/map/map_ir.hpp"
#include "io/map/parse/xml/xml_parser.hpp"
#include "ui/xml_utils.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto _collect_layer_nodes(XmlNode map_node) -> Vector<XmlNode>
{
  Vector<XmlNode> nodes;

  for (auto node: map_node.children()) {
    if (std::strcmp(node.name(), "layer") == 0 ||
        std::strcmp(node.name(), "objectgroup") == 0 ||
        std::strcmp(node.name(), "group") == 0) {
      nodes.push_back(node);
    }
  }

  return nodes;
}

[[nodiscard]] auto _parse_csv_tiles(const char* csv, const TileExtent extent)
    -> Parsed<TileMatrix>
{
  auto tiles = make_tile_matrix(extent);

  usize index {};
  for (const auto& token: split(csv, ',')) {
    if (const auto id = parse_i32(token)) {
      const auto [row, col] = to_matrix_coords(index, extent.cols);
      tiles[row][col] = *id;

      ++index;
    }
    else {
      return unexpected(ParseError::CorruptTileLayerData);
    }
  }

  return tiles;
}

[[nodiscard]] auto _parse_tile_nodes(XmlNode data_node, const TileExtent extent)
    -> Parsed<TileMatrix>
{
  auto tiles = make_tile_matrix(extent);

  usize index = 0;
  for (const auto tile_node: data_node.children("tile")) {
    const auto [row, col] = to_matrix_coords(index, extent.cols);
    tiles[row][col] = tile_node.attribute("gid").as_int(kEmptyTile);

    ++index;
  }

  return tiles;
}

[[nodiscard]] auto _parse_tile_data(XmlNode layer_node, MapIR& map) -> Parsed<TileMatrix>
{
  const auto data = layer_node.child("data");

  if (data.empty() || data.text().empty()) {
    return unexpected(ParseError::NoTileLayerData);
  }

  if (const auto* encoding = data.attribute("encoding").as_string(nullptr)) {
    if (std::strcmp(encoding, "csv") == 0) {
      map.tile_format.encoding = TileEncoding::Plain;
      map.tile_format.compression = TileCompression::None;

      const auto text = data.text();
      if (auto tiles = _parse_csv_tiles(text.get(), map.extent)) {
        return std::move(*tiles);
      }
      else {
        return propagate_unexpected(tiles);
      }
    }
    else if (std::strcmp(encoding, "base64") == 0) {
      map.tile_format.encoding = TileEncoding::Base64;

      if (const auto* compression = data.attribute("compression").as_string(nullptr);
          !compression || std::strcmp(compression, "") == 0) {
        map.tile_format.compression = TileCompression::None;
      }
      else if (std::strcmp(compression, "zlib") == 0) {
        map.tile_format.compression = TileCompression::Zlib;
      }
      else if (std::strcmp(compression, "zstd") == 0) {
        map.tile_format.compression = TileCompression::Zstd;
      }

      return base64_decode_tiles(data.text().get(),
                                 map.extent,
                                 map.tile_format.compression);
    }
    else {
      return unexpected(ParseError::UnsupportedTileLayerEncoding);
    }
  }
  else {
    map.tile_format.encoding = TileEncoding::Plain;
    map.tile_format.compression = TileCompression::None;

    if (auto tiles = _parse_tile_nodes(data, map.extent)) {
      return std::move(*tiles);
    }
    else {
      return propagate_unexpected(tiles);
    }
  }
}

[[nodiscard]] auto _parse_tile_layer(XmlNode layer_node, MapIR& map)
    -> Parsed<TileLayerIR>
{
  TileLayerIR tile_layer;

  if (const auto width = get_uint_attr(layer_node, "width")) {
    tile_layer.extent.cols = *width;

    if (tile_layer.extent.cols != map.extent.cols) {
      spdlog::warn("[XML] Tile layer width does not match map width, '{}' vs '{}'",
                   tile_layer.extent.cols,
                   map.extent.cols);
    }
  }
  else {
    spdlog::warn("[XML] Tile layer has no width information, assuming map width");
    tile_layer.extent.cols = map.extent.cols;
  }

  if (const auto height = get_uint_attr(layer_node, "height")) {
    tile_layer.extent.rows = *height;

    if (tile_layer.extent.rows != map.extent.rows) {
      spdlog::warn("[XML] Tile layer height does not match map height, '{}' vs '{}'",
                   tile_layer.extent.rows,
                   map.extent.rows);
    }
  }
  else {
    spdlog::warn("[XML] Tile layer has no height information, assuming map height");
    tile_layer.extent.rows = map.extent.rows;
  }

  if (auto tiles = _parse_tile_data(layer_node, map)) {
    tile_layer.tiles = std::move(*tiles);
  }
  else {
    return propagate_unexpected(tiles);
  }

  return tile_layer;
}

[[nodiscard]] auto _parse_object_layer(XmlNode layer_node) -> Parsed<ObjectLayerIR>
{
  ObjectLayerIR object_layer;

  for (const auto object_node: layer_node.children("object")) {
    if (auto object = parse_object(object_node)) {
      object_layer.objects.push_back(std::move(*object));
    }
    else {
      return propagate_unexpected(object);
    }
  }

  return object_layer;
}

[[nodiscard]] auto _parse_layer(XmlNode layer_node, MapIR& map, const usize index)
    -> Parsed<LayerIR>
{
  LayerIR layer;
  layer.index = index;

  if (const auto id = get_int_attr(layer_node, "id")) {
    layer.id = *id;
  }
  else {
    return unexpected(ParseError::NoLayerId);
  }

  layer.name = layer_node.attribute("name").as_string("Layer");
  layer.opacity = layer_node.attribute("opacity").as_float(1.0f);
  layer.visible = layer_node.attribute("visible").as_bool(true);

  if (std::strcmp(layer_node.name(), "layer") == 0) {
    layer.type = LayerType::TileLayer;

    if (auto tile_layer = _parse_tile_layer(layer_node, map)) {
      layer.data.emplace<TileLayerIR>(std::move(*tile_layer));
    }
    else {
      return propagate_unexpected(tile_layer);
    }
  }
  else if (std::strcmp(layer_node.name(), "objectgroup") == 0) {
    layer.type = LayerType::ObjectLayer;

    if (auto object_layer = _parse_object_layer(layer_node)) {
      layer.data.emplace<ObjectLayerIR>(std::move(*object_layer));
    }
    else {
      return propagate_unexpected(object_layer);
    }
  }
  else if (std::strcmp(layer_node.name(), "group") == 0) {
    layer.type = LayerType::GroupLayer;
    auto& group = layer.data.emplace<GroupLayerIR>();

    usize child_index = 0;
    for (auto child_node: _collect_layer_nodes(layer_node)) {
      if (auto child_layer = _parse_layer(child_node, map, child_index)) {
        group.children.push_back(std::make_unique<LayerIR>(std::move(*child_layer)));
      }
      else {
        return propagate_unexpected(child_layer);
      }

      ++child_index;
    }
  }
  else {
    // If we enter this branch, then the layer collection is broken
    throw TactileError {"Collected invalid layer node"};
  }

  if (auto props = parse_properties(layer_node)) {
    layer.context.properties = std::move(*props);
  }
  else {
    return propagate_unexpected(props);
  }

  return layer;
}

}  // namespace

auto parse_object(XmlNode object_node) -> Parsed<ObjectIR>
{
  ObjectIR object;

  if (const auto id = get_int_attr(object_node, "id")) {
    object.id = *id;
  }
  else {
    return unexpected(ParseError::NoObjectId);
  }

  object.name = object_node.attribute("name").as_string("");
  object.tag = object_node.attribute("type").as_string("");

  object.pos.x = object_node.attribute("x").as_float(0);
  object.pos.y = object_node.attribute("y").as_float(0);
  object.size.x = object_node.attribute("width").as_float(0);
  object.size.y = object_node.attribute("height").as_float(0);

  object.visible = object_node.attribute("visible").as_bool(true);

  if (!object_node.child("point").empty()) {
    object.type = ObjectType::Point;
  }
  else if (!object_node.child("ellipse").empty()) {
    object.type = ObjectType::Ellipse;
  }
  else {
    object.type = ObjectType::Rect;
  }

  if (auto props = parse_properties(object_node)) {
    object.context.properties = std::move(*props);
  }
  else {
    return propagate_unexpected(props);
  }

  return object;
}

auto parse_layers(XmlNode map_node, MapIR& map) -> Parsed<Vector<LayerIR>>
{
  Vector<LayerIR> layers;

  usize index = 0;
  for (const auto layer_node: _collect_layer_nodes(map_node)) {
    if (auto layer = _parse_layer(layer_node, map, index)) {
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
