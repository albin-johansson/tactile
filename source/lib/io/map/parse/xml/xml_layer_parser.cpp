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

#include <cstring>  // strcmp
#include <utility>  // move

#include <spdlog/spdlog.h>

#include "core/debug/panic.hpp"
#include "core/tile/tile_matrix.hpp"
#include "common/type/string.hpp"
#include "common/type/vec.hpp"
#include "core/util/str.hpp"
#include "io/map/ir/ir.hpp"
#include "io/map/parse/xml/xml_parser.hpp"
#include "io/util/base64_tiles.hpp"
#include "io/util/xml.hpp"

namespace tactile::io {
namespace {

[[nodiscard]] auto collect_layer_nodes(XMLNode map_node) -> Vec<XMLNode>
{
  Vec<XMLNode> nodes;

  for (auto node: map_node.children()) {
    if (std::strcmp(node.name(), "layer") == 0 ||
        std::strcmp(node.name(), "objectgroup") == 0 ||
        std::strcmp(node.name(), "group") == 0) {
      nodes.push_back(node);
    }
  }

  return nodes;
}

[[nodiscard]] auto parse_csv_tiles(const char* csv,
                                   const usize row_count,
                                   const usize col_count)
    -> Expected<TileMatrix, ParseError>
{
  auto tiles = make_tile_matrix(row_count, col_count);

  usize index {};
  for (const auto& token: split(csv, ',')) {
    if (const auto id = parse_i32(token)) {
      const auto [row, col] = to_matrix_coords(index, col_count);
      tiles[row][col] = *id;

      ++index;
    }
    else {
      return error(ParseError::CorruptTileLayerData);
    }
  }

  return tiles;
}

[[nodiscard]] auto parse_tile_nodes(XMLNode data_node,
                                    const usize row_count,
                                    const usize col_count)
    -> Expected<TileMatrix, ParseError>
{
  auto tiles = make_tile_matrix(row_count, col_count);

  usize index = 0;
  for (const auto tile_node: data_node.children("tile")) {
    const auto [row, col] = to_matrix_coords(index, col_count);
    tiles[row][col] = tile_node.attribute("gid").as_int(empty_tile);

    ++index;
  }

  return tiles;
}

[[nodiscard]] auto parse_tile_data(XMLNode layer_node, ir::MapData& map)
    -> Expected<TileMatrix, ParseError>
{
  const auto data = layer_node.child("data");

  if (data.empty() || data.text().empty()) {
    return error(ParseError::NoTileLayerData);
  }

  if (const auto* encoding = data.attribute("encoding").as_string(nullptr)) {
    if (std::strcmp(encoding, "csv") == 0) {
      map.tile_format.encoding = TileEncoding::Plain;
      map.tile_format.compression = TileCompression::None;

      const auto text = data.text();
      if (auto tiles = parse_csv_tiles(text.get(), map.row_count, map.col_count)) {
        return std::move(*tiles);
      }
      else {
        return pass_on_error(tiles);
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
                                 map.row_count,
                                 map.col_count,
                                 map.tile_format.compression);
    }
    else {
      return error(ParseError::UnsupportedTileLayerEncoding);
    }
  }
  else {
    map.tile_format.encoding = TileEncoding::Plain;
    map.tile_format.compression = TileCompression::None;

    if (auto tiles = parse_tile_nodes(data, map.row_count, map.col_count)) {
      return std::move(*tiles);
    }
    else {
      return pass_on_error(tiles);
    }
  }
}

[[nodiscard]] auto parse_tile_layer(XMLNode layer_node, ir::MapData& map)
    -> Expected<ir::TileLayerData, ParseError>
{
  ir::TileLayerData tile_layer;

  if (const auto width = as_uint(layer_node, "width")) {
    tile_layer.col_count = *width;

    if (tile_layer.col_count != map.col_count) {
      spdlog::warn("XML tile layer width does not match map width, '{}' vs '{}'",
                   tile_layer.col_count,
                   map.col_count);
    }
  }
  else {
    spdlog::warn("XML tile layer has no width information, assuming map width...");
    tile_layer.col_count = map.col_count;
  }

  if (const auto height = as_uint(layer_node, "height")) {
    tile_layer.row_count = *height;

    if (tile_layer.row_count != map.row_count) {
      spdlog::warn("XML tile layer height does not match map height, '{}' vs '{}'",
                   tile_layer.row_count,
                   map.row_count);
    }
  }
  else {
    spdlog::warn("XML tile layer has no height information, assuming map height...");
    tile_layer.row_count = map.row_count;
  }

  if (auto tiles = parse_tile_data(layer_node, map)) {
    tile_layer.tiles = std::move(*tiles);
  }
  else {
    return pass_on_error(tiles);
  }

  return tile_layer;
}

[[nodiscard]] auto parse_object_layer(XMLNode layer_node)
    -> Expected<ir::ObjectLayerData, ParseError>
{
  ir::ObjectLayerData object_layer;

  for (const auto object_node: layer_node.children("object")) {
    if (auto object = parse_object(object_node)) {
      object_layer.objects.push_back(std::move(*object));
    }
    else {
      return pass_on_error(object);
    }
  }

  return object_layer;
}

[[nodiscard]] auto parse_layer(XMLNode layer_node, ir::MapData& map, const usize index)
    -> Expected<ir::LayerData, ParseError>
{
  ir::LayerData layer;
  layer.index = index;

  if (const auto id = as_int(layer_node, "id")) {
    layer.id = *id;
  }
  else {
    return error(ParseError::NoLayerId);
  }

  layer.name = layer_node.attribute("name").as_string("Layer");
  layer.opacity = layer_node.attribute("opacity").as_float(1.0f);
  layer.visible = layer_node.attribute("visible").as_bool(true);

  if (std::strcmp(layer_node.name(), "layer") == 0) {
    layer.type = LayerType::TileLayer;

    if (auto tile_layer = parse_tile_layer(layer_node, map)) {
      layer.data.emplace<ir::TileLayerData>(std::move(*tile_layer));
    }
    else {
      return pass_on_error(tile_layer);
    }
  }
  else if (std::strcmp(layer_node.name(), "objectgroup") == 0) {
    layer.type = LayerType::ObjectLayer;

    if (auto object_layer = parse_object_layer(layer_node)) {
      layer.data.emplace<ir::ObjectLayerData>(std::move(*object_layer));
    }
    else {
      return pass_on_error(object_layer);
    }
  }
  else if (std::strcmp(layer_node.name(), "group") == 0) {
    layer.type = LayerType::GroupLayer;
    auto& group = layer.data.emplace<ir::GroupLayerData>();

    usize child_index = 0;
    for (auto child_node: collect_layer_nodes(layer_node)) {
      if (auto child_layer = parse_layer(child_node, map, child_index)) {
        group.children.push_back(
            std::make_unique<ir::LayerData>(std::move(*child_layer)));
      }
      else {
        return pass_on_error(child_layer);
      }

      ++child_index;
    }
  }
  else {
    // If we enter this branch, then the layer collection is broken
    throw TactileError {"Collected invalid layer node!"};
  }

  if (auto props = parse_properties(layer_node)) {
    layer.context.properties = std::move(*props);
  }
  else {
    return pass_on_error(props);
  }

  return layer;
}

}  // namespace

auto parse_object(XMLNode object_node) -> Expected<ir::ObjectData, ParseError>
{
  ir::ObjectData object;

  if (const auto id = as_int(object_node, "id")) {
    object.id = *id;
  }
  else {
    return error(ParseError::NoObjectId);
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
    return pass_on_error(props);
  }

  return object;
}

auto parse_layers(XMLNode map_node, ir::MapData& map)
    -> Expected<Vec<ir::LayerData>, ParseError>
{
  Vec<ir::LayerData> layers;

  usize index = 0;
  for (const auto layer_node: collect_layer_nodes(map_node)) {
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
