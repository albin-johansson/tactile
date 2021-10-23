#pragma once

#include <filesystem>  // path

#include <tactile-base/tactile_std.hpp>

#include "api.hpp"
#include "parse_error.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

TACTILE_IO_API_QUERY auto ParseJsonMap(CPathStr path, ParseError* error = nullptr)
    -> Map*;

TACTILE_IO_API_QUERY auto ParseXmlMap(CPathStr path, ParseError* error = nullptr) -> Map*;

TACTILE_IO_API_QUERY auto ParseYamlMap(CPathStr path, ParseError* error = nullptr)
    -> Map*;

[[nodiscard]] inline auto ParseMap(const std::filesystem::path& path,
                                   ParseError* error = nullptr) -> MapPtr
{
  const auto ext = path.extension();
  if (ext == ".yaml" || ext == ".yml") {
    return MapPtr{ParseYamlMap(path.c_str(), error)};
  }
  else if (ext == ".json") {
    return MapPtr{ParseJsonMap(path.c_str(), error)};
  }
  else if (ext == ".xml" || ext == ".tmx") {
    return MapPtr{ParseXmlMap(path.c_str(), error)};
  }
  else {
    if (error) {
      *error = ParseError::MapUnsupportedExtension;
    }

    return nullptr;
  }
}

}  // namespace Tactile::IO