#pragma once

#include <filesystem>  // path

#include <pugixml.hpp>  // xml_node

#include "parse_ir.hpp"

namespace Tactile::IO {

void AppendFancyTiles(pugi::xml_node node,
                      const Tileset& tileset,
                      const std::filesystem::path& dir);

}  // namespace Tactile::IO
