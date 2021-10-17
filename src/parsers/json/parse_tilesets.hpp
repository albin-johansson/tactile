#pragma once

#include <filesystem>  // path
#include <vector>      // vector

#include "common/expected.hpp"
#include "io/json.hpp"
#include "parsers/parse_error.hpp"
#include "parsers/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTilesets(const JSON& json, const std::filesystem::path& dir)
    -> Expected<std::vector<TilesetData>, ParseError>;

}  // namespace Tactile::IO
