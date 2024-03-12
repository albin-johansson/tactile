// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "parse_map.hpp"

#include <exception>  // exception

#include <fmt/chrono.h>
#include <fmt/std.h>
#include <spdlog/spdlog.h>

#include "io/map/parse/json/json_parser.hpp"
#include "io/map/parse/xml/xml_parser.hpp"
#include "io/map/parse/yaml/yaml_parser.hpp"
#include "tactile/base/util/chrono.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile {

auto parse_map(const Path& path) -> ParseResult
{
  try {
    const auto parse_start = SystemClock::now();
    spdlog::debug("Parsing map {}", path);

    if (!fs::exists(path)) {
      return unexpected(ParseError::MapDoesNotExist);
    }

    ParseResult result;
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
      return unexpected(ParseError::UnsupportedMapExtension);
    }

    const auto parse_end = SystemClock::now();
    const auto parse_duration = duration_cast<Milliseconds>(parse_end - parse_start);

    spdlog::info("Parsed {} in {}", path.filename(), parse_duration);

    return result;
  }
  catch (const Exception& e) {
    spdlog::error("Parser threw unhandled exception with message: '{}'\n{}",
                  e.what(),
                  e.trace());
    return unexpected(ParseError::Unknown);
  }
  catch (const std::exception& e) {
    spdlog::error("Parser threw unhandled exception with message: '{}'\n", e.what());
    return unexpected(ParseError::Unknown);
  }
  catch (...) {
    spdlog::error("Parser threw non-exception value!");
    return unexpected(ParseError::Unknown);
  }
}

}  // namespace tactile
