#pragma once

#include <filesystem>  // path

#include <tactile-base/tactile_std.hpp>

#include "api.hpp"
#include "parse_error.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

TACTILE_IO_API_QUERY auto ParseJsonMap(const std::filesystem::path& path,
                                       ParseError* error = nullptr) -> MapPtr;

[[nodiscard]] TACTILE_IO_API auto ParseXmlMap(const std::filesystem::path& path,
                                              ParseError* error = nullptr)
    -> Maybe<MapData>;

TACTILE_IO_API_QUERY auto ParseYamlMap(const std::filesystem::path& path,
                                       ParseError* error = nullptr) -> MapPtr;

}  // namespace Tactile::IO