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

#include "json_parser.hpp"

#include <string>  // string

#include "io/util/json.hpp"
#include "tactile/core/type/string.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto validate_map(const JSON& json) -> ParseError
{
  if (const auto iter = json.find("orientation");
      iter == json.end() || iter->get<std::string>() != "orthogonal") {
    return ParseError::UnsupportedMapOrientation;
  }

  if (const auto iter = json.find("infinite"); iter != json.end() && iter->get<bool>()) {
    return ParseError::UnsupportedInfiniteMap;
  }

  return ParseError::None;
}

[[nodiscard]] auto parse_map(const Path& path, MapIR& map_data) -> ParseError
{
  const auto json = parse_json_file(path);
  if (!json) {
    return ParseError::CouldNotReadFile;
  }

  if (const auto err = validate_map(*json); err != ParseError::None) {
    return err;
  }

  if (const auto width = as_uint(*json, "width")) {
    map_data.extent.cols = *width;
  }
  else {
    return ParseError::NoMapWidth;
  }

  if (const auto height = as_uint(*json, "height")) {
    map_data.extent.rows = *height;
  }
  else {
    return ParseError::NoMapHeight;
  }

  if (const auto tw = as_int(*json, "tilewidth")) {
    map_data.tile_size.x = *tw;
  }
  else {
    return ParseError::NoMapTileWidth;
  }

  if (const auto th = as_int(*json, "tileheight")) {
    map_data.tile_size.y = *th;
  }
  else {
    return ParseError::NoMapTileHeight;
  }

  if (const auto id = as_int(*json, "nextlayerid")) {
    map_data.next_layer_id = *id;
  }
  else {
    return ParseError::NoMapNextLayerId;
  }

  if (const auto id = as_int(*json, "nextobjectid")) {
    map_data.next_object_id = *id;
  }
  else {
    return ParseError::NoMapNextObjectId;
  }

  // TODO compression level?

  const auto dir = path.parent_path();

  if (const auto err = parse_tilesets(*json, map_data, dir); err != ParseError::None) {
    return err;
  }

  if (const auto err = parse_layers(*json, map_data); err != ParseError::None) {
    return err;
  }

  if (const auto err = parse_properties(*json, map_data.context);
      err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

}  // namespace

auto parse_json_map(const Path& path) -> ParseResult
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