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

#include "xml_parser.hpp"

#include "core/common/filesystem.hpp"
#include "io/util/xml.hpp"

namespace tactile::io {
namespace {

[[nodiscard]] auto validate_map(XMLNode map_node) -> ParseError
{
  if (as_string(map_node, "orientation") != "orthogonal") {
    return ParseError::UnsupportedMapOrientation;
  }

  if (as_bool(map_node, "infinite").value_or(false)) {
    return ParseError::UnsupportedInfiniteMap;
  }

  return ParseError::None;
}

[[nodiscard]] auto parse_map(const fs::path& path, ir::MapData& data) -> ParseError
{
  pugi::xml_document document;
  if (!document.load_file(path.c_str())) {
    return ParseError::CouldNotReadFile;
  }

  auto map_node = document.child("map");

  if (const auto err = validate_map(map_node); err != ParseError::None) {
    return err;
  }

  if (const auto width = as_uint(map_node, "width")) {
    data.col_count = *width;
  }
  else {
    return ParseError::NoMapWidth;
  }

  if (const auto height = as_uint(map_node, "height")) {
    data.row_count = *height;
  }
  else {
    return ParseError::NoMapHeight;
  }

  if (const auto tw = as_int(map_node, "tilewidth")) {
    data.tile_size.x = *tw;
  }
  else {
    return ParseError::NoMapTileWidth;
  }

  if (const auto th = as_int(map_node, "tileheight")) {
    data.tile_size.y = *th;
  }
  else {
    return ParseError::NoMapTileHeight;
  }

  if (const auto id = as_int(map_node, "nextlayerid")) {
    data.next_layer_id = *id;
  }
  else {
    return ParseError::NoMapNextLayerId;
  }

  if (const auto id = as_int(map_node, "nextobjectid")) {
    data.next_object_id = *id;
  }
  else {
    return ParseError::NoMapNextObjectId;
  }

  const auto dir = path.parent_path();

  for (auto tileset_node : map_node.children("tileset")) {
    auto& tileset_data = data.tilesets.emplace_back();
    if (const auto err = parse_tileset(tileset_node, tileset_data, dir);
        err != ParseError::None) {
      return err;
    }
  }

  if (const auto err = parse_layers(map_node, data); err != ParseError::None) {
    return err;
  }

  if (const auto err = parse_properties(map_node, data.context);
      err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

}  // namespace

auto parse_xml_map(const fs::path& path) -> ParseResult
{
  ParseResult result;
  result.set_path(path);

  const auto err = parse_map(path, result.data());
  result.set_error(err);

  return result;
}

}  // namespace tactile::io