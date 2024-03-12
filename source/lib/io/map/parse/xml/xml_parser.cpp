// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "xml_parser.hpp"

#include <utility>  // move

#include "io/util/xml.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto validate_map(XmlNode map_node) -> ParseError
{
  if (get_string_attr(map_node, "orientation") != "orthogonal") {
    return ParseError::UnsupportedMapOrientation;
  }

  if (get_bool_attr(map_node, "infinite").value_or(false)) {
    return ParseError::UnsupportedInfiniteMap;
  }

  return ParseError::None;
}

[[nodiscard]] auto parse_map(const Path& path, MapIR& data) -> ParseError
{
  auto document = parse_xml_file(path);
  if (!document) {
    return ParseError::CouldNotReadFile;
  }

  auto map_node = document->child("map");

  if (const auto err = validate_map(map_node); err != ParseError::None) {
    return err;
  }

  if (const auto width = get_uint_attr(map_node, "width")) {
    data.extent.cols = *width;
  }
  else {
    return ParseError::NoMapWidth;
  }

  if (const auto height = get_uint_attr(map_node, "height")) {
    data.extent.rows = *height;
  }
  else {
    return ParseError::NoMapHeight;
  }

  if (const auto tw = get_int_attr(map_node, "tilewidth")) {
    data.tile_size[0] = *tw;
  }
  else {
    return ParseError::NoMapTileWidth;
  }

  if (const auto th = get_int_attr(map_node, "tileheight")) {
    data.tile_size[1] = *th;
  }
  else {
    return ParseError::NoMapTileHeight;
  }

  if (const auto id = get_int_attr(map_node, "nextlayerid")) {
    data.next_layer_id = *id;
  }
  else {
    return ParseError::NoMapNextLayerId;
  }

  if (const auto id = get_int_attr(map_node, "nextobjectid")) {
    data.next_object_id = *id;
  }
  else {
    return ParseError::NoMapNextObjectId;
  }

  const auto dir = path.parent_path();

  for (auto tileset_node: map_node.children("tileset")) {
    if (auto tileset = parse_tileset(tileset_node, dir)) {
      data.tilesets.push_back(std::move(*tileset));
    }
    else {
      return tileset.error();
    }
  }

  if (auto layers = parse_layers(map_node, data)) {
    data.layers = std::move(*layers);
  }
  else {
    return layers.error();
  }

  if (auto props = parse_properties(map_node)) {
    data.context.properties = std::move(*props);
  }
  else {
    return props.error();
  }

  return ParseError::None;
}

}  // namespace

auto parse_xml_map(const Path& path) -> ParseResult
{
  MapIR ir_map;
  const auto res = parse_map(path, ir_map);

  if (res == ParseError::None) {
    return ir_map;
  }
  else {
    return unexpected(res);
  }
}

}  // namespace tactile
