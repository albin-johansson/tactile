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

[[nodiscard]] auto _validate_map(const nlohmann::json& json) -> parse_error
{
  if (const auto iter = json.find("orientation");
      iter == json.end() || iter->get<std::string>() != "orthogonal") {
    return parse_error::unsupported_map_orientation;
  }

  if (const auto iter = json.find("infinite"); iter != json.end() && iter->get<bool>()) {
    return parse_error::unsupported_infinite_map;
  }

  return parse_error::none;
}

[[nodiscard]] auto _parse_map(const std::filesystem::path& path, ir::MapData& mapData)
    -> parse_error
{
  const auto json = read_json(path);
  if (!json) {
    return parse_error::could_not_read_file;
  }

  if (const auto err = _validate_map(*json); err != parse_error::none) {
    return err;
  }

  if (const auto width = as_uint(*json, "width")) {
    mapData.col_count = *width;
  }
  else {
    return parse_error::no_map_width;
  }

  if (const auto height = as_uint(*json, "height")) {
    mapData.row_count = *height;
  }
  else {
    return parse_error::no_map_height;
  }

  if (const auto tw = as_int(*json, "tilewidth")) {
    mapData.tile_width = *tw;
  }
  else {
    return parse_error::no_map_tile_width;
  }

  if (const auto th = as_int(*json, "tileheight")) {
    mapData.tile_height = *th;
  }
  else {
    return parse_error::no_map_tile_height;
  }

  if (const auto id = as_int(*json, "nextlayerid")) {
    mapData.next_layer_id = *id;
  }
  else {
    return parse_error::no_map_next_layer_id;
  }

  if (const auto id = as_int(*json, "nextobjectid")) {
    mapData.next_object_id = *id;
  }
  else {
    return parse_error::no_map_next_object_id;
  }

  const auto dir = path.parent_path();

  if (const auto err = parse_tilesets(*json, mapData, dir); err != parse_error::none) {
    return err;
  }

  if (const auto err = parse_layers(*json, mapData); err != parse_error::none) {
    return err;
  }

  if (const auto err = parse_properties(*json, mapData.context);
      err != parse_error::none) {
    return err;
  }

  return parse_error::none;
}

}  // namespace

auto parse_json_map(const std::filesystem::path& path) -> parse_data
{
  parse_data result;
  result.set_path(path);

  const auto err = _parse_map(path, result.data());
  result.set_error(err);

  return result;
}

}  // namespace tactile::parsing