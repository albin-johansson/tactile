#pragma once

#include <filesystem>  // path
#include <optional>    // optional

#include "api.hpp"
#include "parse_error.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] TACTILE_IO_API auto ParseYamlMap(const std::filesystem::path& path,
                                               ParseError* error = nullptr)
    -> std::optional<MapData>;

}  // namespace Tactile::IO