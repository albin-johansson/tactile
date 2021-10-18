#pragma once

#include <filesystem>  // path
#include <vector>      // vector

#include <pugixml.hpp>  // xml_node

#include <tactile-io/parse_error.hpp>
#include <tactile-io/parse_ir.hpp>

namespace Tactile::IO {

[[nodiscard]] auto ParseTilesets(pugi::xml_node root,
                                 std::vector<TilesetData>& tilesets,
                                 const std::filesystem::path& directory) -> ParseError;

}  // namespace Tactile::IO
