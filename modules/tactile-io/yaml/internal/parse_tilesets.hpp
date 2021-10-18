#pragma once

#include <filesystem>  // path
#include <vector>      // vector

#include "../../parse_error.hpp"
#include "../../parse_ir.hpp"
#include "../yaml_fwd.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTilesets(const YAML::Node& seq, const std::filesystem::path& dir)
    -> tl::expected<std::vector<TilesetData>, ParseError>;

}  // namespace Tactile::IO
