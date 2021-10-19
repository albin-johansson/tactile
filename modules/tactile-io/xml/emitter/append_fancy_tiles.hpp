#pragma once

#include <filesystem>  // path
#include <vector>      // vector

#include <pugixml.hpp>  // xml_node

#include "../../parse_ir.hpp"

namespace Tactile::IO {

void AppendFancyTiles(pugi::xml_node node,
                      const std::vector<TileData>& tiles,
                      const std::filesystem::path& dir);

}  // namespace Tactile::IO
