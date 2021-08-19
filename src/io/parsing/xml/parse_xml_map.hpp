#pragma once

#include <filesystem>  // path

#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseXmlMap(const std::filesystem::path& path, MapData& data)
    -> ParseError;

}  // namespace Tactile::IO
