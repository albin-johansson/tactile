#pragma once

#include <filesystem>  // path

#include <pugixml.hpp>  // xml_node

#include "ir.hpp"

namespace Tactile::IO {

struct EmitterOptions;

void AppendTileset(pugi::xml_node mapNode,
                   const Tileset& tileset,
                   const std::filesystem::path& dir,
                   const EmitterOptions& options);

}  // namespace Tactile::IO
