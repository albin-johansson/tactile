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

#include "parse_map.hpp"

#include <exception>  // exception

#include <fmt/chrono.h>
#include <fmt/std.h>
#include <spdlog/spdlog.h>

#include "common/debug/panic.hpp"
#include "common/type/chrono.hpp"
#include "io/map/parse/json/json_parser.hpp"
#include "io/map/parse/xml/xml_parser.hpp"
#include "io/map/parse/yaml/yaml_parser.hpp"

namespace tactile::io {

auto parse_map(const Path& path) -> ParseResult
{
  spdlog::debug("Parsing map {}", path);
  ParseResult result;

  try {
    const auto parse_start = Clock::now();

    if (!fs::exists(path)) {
      result.set_error(ParseError::MapDoesNotExist);
      return result;
    }

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
      spdlog::error("Unsupported save file extension: {}", ext);
      result.set_error(ParseError::UnsupportedMapExtension);
      return result;
    }

    const auto parse_end = Clock::now();
    const auto parse_duration = chrono::duration_cast<ms_t>(parse_end - parse_start);

    spdlog::info("Parsed {} in {}", path.filename(), parse_duration);
  }
  catch (const TactileError& e) {
    result.set_error(ParseError::Unknown);
    spdlog::error("Parser threw unhandled exception with message: '{}'\n{}",
                  e.what(),
                  e.get_trace());
  }
  catch (const std::exception& e) {
    result.set_error(ParseError::Unknown);
    spdlog::error("Parser threw unhandled exception with message: '{}'\n", e.what());
  }
  catch (...) {
    spdlog::error("Parser threw non-exception value!");
    result.set_error(ParseError::Unknown);
  }

  return result;
}

}  // namespace tactile::io