#pragma once

#include <filesystem>  // path

#include "../json_common.hpp"
#include "parse_error.hpp"
#include "tactile_io.hpp"

namespace tactile::IO {

[[nodiscard]] auto ParseTilesets(const JSON& json,
                                 Map& map,
                                 const std::filesystem::path& dir) -> ParseError;

}  // namespace tactile::IO
