#pragma once

#include <filesystem>  // path

#include <tactile-base/tactile_std.hpp>

#include "api.hpp"
#include "parse_error.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] TACTILE_IO_API auto ParseJsonMap(const std::filesystem::path& path,
                                               ParseError* error = nullptr)
    -> Maybe<MapData>;

[[nodiscard]] TACTILE_IO_API auto ParseXmlMap(const std::filesystem::path& path,
                                              ParseError* error = nullptr)
    -> Maybe<MapData>;

[[nodiscard]] TACTILE_IO_API auto ParseYamlMap(const std::filesystem::path& path,
                                               ParseError* error = nullptr)
    -> Maybe<MapData>;

}  // namespace Tactile::IO