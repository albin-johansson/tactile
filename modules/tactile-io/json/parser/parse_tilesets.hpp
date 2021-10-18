#pragma once

#include <filesystem>  // path
#include <vector>      // vector

#include <expected.hpp>  // expected

#include "../json_common.hpp"
#include "parse_error.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTilesets(const JSON& json, const std::filesystem::path& dir)
    -> tl::expected<std::vector<TilesetData>, ParseError>;

}  // namespace Tactile::IO
