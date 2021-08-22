#pragma once

#include <filesystem>  // path
#include <vector>      // vector

#include "aliases/json.hpp"
#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTilesets(const JSON& json,
                                 std::vector<TilesetData>& tilesets,
                                 const std::filesystem::path& directory)
    -> ParseError;

}  // namespace Tactile::IO
