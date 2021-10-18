#pragma once

#include <filesystem>  // path

#include <tactile/io/maps/parse_error.hpp>
#include <tactile/io/maps/parse_ir.hpp>

namespace Tactile::IO {

[[nodiscard]] auto ParseXmlMap(const std::filesystem::path& path, MapData& data)
    -> ParseError;

}  // namespace Tactile::IO
