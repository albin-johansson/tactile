#pragma once

#include <filesystem>  // path

#include "parsers/parse_error.hpp"
#include "parsers/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseXmlMap(const std::filesystem::path& path, MapData& data)
    -> ParseError;

}  // namespace Tactile::IO
