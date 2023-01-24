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

#include "xml_parser.hpp"

#include <utility>  // move

#include "io/util/xml.hpp"

namespace tactile::io {
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

[[nodiscard]] auto parse_map(const Path& path, ir::MapData& data) -> ParseError
{
  pugi::xml_document document;
  if (!document.load_file(path.c_str(), pugi::parse_default | pugi::parse_trim_pcdata)) {
    return ParseError::CouldNotReadFile;
  }

  auto map_node = document.child("map");

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
    data.tile_size.x = *tw;
  }
  else {
    return ParseError::NoMapTileWidth;
  }

  if (const auto th = get_int_attr(map_node, "tileheight")) {
    data.tile_size.y = *th;
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
  ParseResult result;
  result.set_path(path);

  const auto err = parse_map(path, result.data());
  result.set_error(err);

  return result;
}

}  // namespace tactile::io