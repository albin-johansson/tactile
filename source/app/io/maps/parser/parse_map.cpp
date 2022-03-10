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

#include "json/json_parser.hpp"
#include "meta/build.hpp"
#include "meta/profile.hpp"
#include "misc/logging.hpp"
#include "misc/throw.hpp"
#include "xml/xml_parser.hpp"
#include "yaml/yaml_parser.hpp"

namespace tactile::parsing {

auto parse_map(const std::filesystem::path& path) -> ParseData
{
  log_info("Parsing map {}", path);
  ParseData result;

  try {
    TACTILE_PROFILE_START

    if (!std::filesystem::exists(path)) {
      result.set_error(ParseError::map_does_not_exist);
      return result;
    }

    const auto ext = path.extension();
    if (ext == ".yaml" || ext == ".yml") {
      result = parse_yaml_map(path);
    }
    else if (ext == ".xml" || ext == ".tmx") {
      result = parse_xml_map(path);
    }
    else if (ext == ".json") {
      result = parse_json_map(path);
    }
    else {
      log_error("Unsupported save file extension: {}", ext);
      result.set_error(ParseError::unsupported_map_extension);
      return result;
    }

    TACTILE_PROFILE_END("Parsed map")
  }
  catch (const std::exception& e) {
    log_error("Parser threw unhandled exception with message: '{}'\n", e.what());
    result.set_error(ParseError::unknown);

    if constexpr (is_debug_build) {
      if (const auto* stacktrace = boost::get_error_info<trace_info>(e)) {
        print(fmt::color::orange, "{}\n", *stacktrace);
      }
    }
  }
  catch (...) {
    log_error("Parser threw non-exception value!");
    result.set_error(ParseError::unknown);
  }

  return result;
}

}  // namespace tactile::parsing