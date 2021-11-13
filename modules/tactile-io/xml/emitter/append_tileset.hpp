#pragma once

#include <filesystem>  // path

#include <pugixml.hpp>  // xml_node

#include "tactile_io.hpp"

namespace Tactile::IO {

void AppendTileset(pugi::xml_node mapNode,
                   const Tileset& tileset,
                   const std::filesystem::path& dir,
                   EmitterOptions options);

}  // namespace Tactile::IO
