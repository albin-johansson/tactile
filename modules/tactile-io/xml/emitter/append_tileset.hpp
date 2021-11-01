#pragma once

#include <filesystem>  // path

#include <pugixml.hpp>  // xml_node

#include "ir.hpp"

namespace Tactile::IO {

void AppendTileset(pugi::xml_node mapNode,
                   const Tileset& tileset,
                   const std::filesystem::path& dir,
                   uint32 options);

}  // namespace Tactile::IO
