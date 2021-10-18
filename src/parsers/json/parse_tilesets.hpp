#pragma once

#include <filesystem>  // path
#include <vector>      // vector

#include <tactile-io/parse_error.hpp>
#include <tactile-io/parse_ir.hpp>

#include "common/expected.hpp"
#include "io/json.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTilesets(const JSON& json, const std::filesystem::path& dir)
    -> Expected<std::vector<TilesetData>, ParseError>;

}  // namespace Tactile::IO
