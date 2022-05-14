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

#include "parse_map.hpp"

#include <exception>  // exception

#include <fmt/color.h>
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

#include "io/maps/parser/json/json_parser.hpp"
#include "io/maps/parser/xml/xml_parser.hpp"
#include "io/maps/parser/yaml/yaml_parser.hpp"
#include "meta/build.hpp"
#include "meta/profile.hpp"
#include "misc/logging.hpp"
#include "misc/stacktrace.hpp"
#include "misc/throw.hpp"

namespace tactile::parsing {

auto parse_map(const std::filesystem::path& path) -> ParseData
{
  spdlog::info("Parsing map {}", path);
  ParseData result;

  try {
    TACTILE_PROFILE_START

    if (!std::filesystem::exists(path)) {
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

    TACTILE_PROFILE_END("Parsed map")
  }
  catch (const std::exception& e) {
    spdlog::error("Parser threw unhandled exception with message: '{}'\n", e.what());
    result.set_error(ParseError::Unknown);

    if constexpr (is_debug_build) {
      if (const auto* stacktrace = boost::get_error_info<TraceInfo>(e)) {
        print(fmt::color::orange, "{}\n", *stacktrace);
      }
    }
  }
  catch (...) {
    spdlog::error("Parser threw non-exception value!");
    result.set_error(ParseError::Unknown);
  }

  return result;
}

}  // namespace tactile::parsing