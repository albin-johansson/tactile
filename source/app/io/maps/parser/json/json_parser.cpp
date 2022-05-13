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

#include "json_parser.hpp"

#include <string>  // string

#include <nlohmann/json.hpp>

#include "io/maps/json_utils.hpp"
#include "json_attribute_parser.hpp"
#include "json_layer_parser.hpp"
#include "json_tileset_parser.hpp"

namespace tactile::parsing {
namespace {

[[nodiscard]] auto _validate_map(const nlohmann::json& json) -> ParseError
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

[[nodiscard]] auto _parse_map(const std::filesystem::path& path, ir::MapData& mapData)
    -> ParseError
{
  const auto json = read_json(path);
  if (!json) {
    return ParseError::CouldNotReadFile;
  }

  if (const auto err = _validate_map(*json); err != ParseError::None) {
    return err;
  }

  if (const auto width = as_uint(*json, "width")) {
    mapData.col_count = *width;
  }
  else {
    return ParseError::NoMapWidth;
  }

  if (const auto height = as_uint(*json, "height")) {
    mapData.row_count = *height;
  }
  else {
    return ParseError::NoMapHeight;
  }

  if (const auto tw = as_int(*json, "tilewidth")) {
    mapData.tile_width = *tw;
  }
  else {
    return ParseError::NoMapTileWidth;
  }

  if (const auto th = as_int(*json, "tileheight")) {
    mapData.tile_height = *th;
  }
  else {
    return ParseError::NoMapTileHeight;
  }

  if (const auto id = as_int(*json, "nextlayerid")) {
    mapData.next_layer_id = *id;
  }
  else {
    return ParseError::NoMapNextLayerId;
  }

  if (const auto id = as_int(*json, "nextobjectid")) {
    mapData.next_object_id = *id;
  }
  else {
    return ParseError::NoMapNextObjectId;
  }

  const auto dir = path.parent_path();

  if (const auto err = parse_tilesets(*json, mapData, dir); err != ParseError::None) {
    return err;
  }

  if (const auto err = parse_layers(*json, mapData); err != ParseError::None) {
    return err;
  }

  if (const auto err = parse_properties(*json, mapData.context);
      err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

}  // namespace

auto parse_json_map(const std::filesystem::path& path) -> ParseData
{
  ParseData result;
  result.set_path(path);

  const auto err = _parse_map(path, result.data());
  result.set_error(err);

  return result;
}

}  // namespace tactile::parsing