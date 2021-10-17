#pragma once

#include <filesystem>   // path
#include <pugixml.hpp>  // xml_node
#include <vector>       // vector

#include "parsers/parse_error.hpp"
#include "parsers/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTilesets(pugi::xml_node root,
                                 std::vector<TilesetData>& tilesets,
                                 const std::filesystem::path& directory) -> ParseError;

}  // namespace Tactile::IO
