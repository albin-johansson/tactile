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

#include "map_parser.hpp"

#include <exception>  // exception

#include <fmt/chrono.h>
#include <fmt/std.h>
#include <spdlog/spdlog.h>

#include "common/debug/panic.hpp"
#include "common/type/chrono.hpp"
#include "io/map/parse/json/json_parser.hpp"
#include "io/map/parse/xml/xml_parser.hpp"
#include "io/map/parse/yaml/yaml_parser.hpp"

namespace tactile {

auto parse_map_with_yaml_format(const Path&) -> Parsed<MapIR>
{
  return unexpected(ParseError::Unknown);
}

auto parse_map_with_tiled_tmj_format(const Path&) -> Parsed<MapIR>
{
  return unexpected(ParseError::Unknown);
}

auto parse_map_with_tiled_tmx_format(const Path&) -> Parsed<MapIR>
{
  return unexpected(ParseError::Unknown);
}

auto parse_map(const Path& path) -> Parsed<MapIR>
{
  try {
    const auto parse_start = Clock::now();
    spdlog::debug("[IO] Parsing map {}", path);

    if (!fs::exists(path)) {
      return unexpected(ParseError::MapDoesNotExist);
    }

    Parsed<MapIR> result;
    const auto ext = path.extension();

    if (ext == ".yaml" || ext == ".yml") {
      result = parse_yaml_map(path);
    }
    else if (ext == ".xml" || ext == ".tmx") {
      result = parse_xml_map(path);
    }
    else if (ext == ".json" || ext == ".tmj") {
      result = parse_json_map(path);
    }
    else {
      spdlog::error("[IO] Unsupported save file extension: {}", ext);
      return unexpected(ParseError::UnsupportedMapExtension);
    }

    const auto parse_end = Clock::now();
    const auto parse_duration = chrono::duration_cast<ms_t>(parse_end - parse_start);

    spdlog::info("[IO] Parsed {} in {}", path.filename(), parse_duration);

    return result;
  }
  catch (const TactileError& e) {
    spdlog::error("[IO] Parser crashed with message: '{}'\n{}", e.what(), e.get_trace());
    return unexpected(ParseError::Unknown);
  }
  catch (const std::exception& e) {
    spdlog::error("[IO] Parser crashed with message: '{}'\n", e.what());
    return unexpected(ParseError::Unknown);
  }
  catch (...) {
    spdlog::error("[IO] Parser crashed");
    return unexpected(ParseError::Unknown);
  }
}

}  // namespace tactile