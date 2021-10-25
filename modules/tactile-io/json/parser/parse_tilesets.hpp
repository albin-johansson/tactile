#pragma once

#include <filesystem>  // path

#include "../json_common.hpp"
#include "ir.hpp"
#include "parse_error.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTilesets(const JSON& json,
                                 Map& map,
                                 const std::filesystem::path& dir) -> ParseError;

}  // namespace Tactile::IO
