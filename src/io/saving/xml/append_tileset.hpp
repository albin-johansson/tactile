#pragma once

#include <filesystem>   // path
#include <pugixml.hpp>  // xml_node

#include "core/tileset/tileset.hpp"

namespace Tactile::IO {

void AppendTileset(const Tileset& tileset,
                   pugi::xml_node& mapNode,
                   const std::filesystem::path& dir);

}  // namespace Tactile::IO
