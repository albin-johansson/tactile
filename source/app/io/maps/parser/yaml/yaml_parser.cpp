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

#include "yaml_parser.hpp"

#include <yaml-cpp/yaml.h>

#include "yaml_attribute_parser.hpp"
#include "yaml_layer_parser.hpp"
#include "yaml_tileset_parser.hpp"

namespace tactile::parsing {
namespace {

[[nodiscard]] auto _parse_map(const std::filesystem::path& path, ir::MapData& data)
    -> ParseError
{
  const auto node = YAML::LoadFile(path.string());
  if (!node) {
    return ParseError::could_not_read_file;
  }

  if (auto rows = node["row-count"]) {
    data.row_count = rows.as<usize>();
  }
  else {
    return ParseError::no_map_height;
  }

  if (auto cols = node["column-count"]) {
    data.col_count = cols.as<usize>();
  }
  else {
    return ParseError::no_map_width;
  }

  if (auto tw = node["tile-width"]) {
    data.tile_width = tw.as<int32>();
  }
  else {
    return ParseError::no_map_tile_width;
  }

  if (auto th = node["tile-height"]) {
    data.tile_height = th.as<int32>();
  }
  else {
    return ParseError::no_map_tile_height;
  }

  if (auto id = node["next-layer-id"]) {
    data.next_layer_id = id.as<layer_id>();
  }
  else {
    return ParseError::no_map_next_layer_id;
  }

  if (auto id = node["next-object-id"]) {
    data.next_object_id = id.as<object_id>();
  }
  else {
    return ParseError::no_map_next_object_id;
  }

  const auto dir = path.parent_path();

  if (const auto err = parse_component_definitions(node, data);
      err != ParseError::none) {
    return err;
  }

  if (auto seq = node["tilesets"]) {
    if (const auto err = parse_tilesets(seq, data, dir); err != ParseError::none) {
      return err;
    }
  }

  if (auto seq = node["layers"]) {
    if (const auto err = parse_layers(seq, data); err != ParseError::none) {
      return err;
    }
  }

  if (const auto err = parse_properties(node, data.context); err != ParseError::none) {
    return err;
  }

  if (const auto err = parse_components(node, data, data.context);
      err != ParseError::none) {
    return err;
  }

  return ParseError::none;
}

}  // namespace

auto parse_yaml_map(const std::filesystem::path& path) -> ParseData
{
  ParseData result;
  result.set_path(path);

  const auto error = _parse_map(path, result.data());
  result.set_error(error);

  return result;
}

}  // namespace tactile::parsing