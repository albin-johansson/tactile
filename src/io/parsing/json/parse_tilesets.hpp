#pragma once

#include <filesystem>  // path
#include <vector>      // vector

#include "common/expected.hpp"
#include "common/json.hpp"
#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTilesets(const JSON& json, const std::filesystem::path& dir)
    -> Expected<std::vector<TilesetData>, ParseError>;

}  // namespace Tactile::IO
