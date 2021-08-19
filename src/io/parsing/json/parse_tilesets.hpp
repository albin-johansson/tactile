#pragma once

#include <filesystem>  // path
#include <json.hpp>    // json
#include <vector>      // vector

#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTilesets(const nlohmann::json& json,
                                 std::vector<TilesetData>& tilesets,
                                 const std::filesystem::path& directory)
    -> ParseError;

}  // namespace Tactile::IO
