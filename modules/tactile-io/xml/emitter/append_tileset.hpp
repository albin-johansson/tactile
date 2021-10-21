#pragma once

#include <filesystem>  // path

#include <pugixml.hpp>  // xml_node

#include "parse_ir.hpp"

namespace Tactile::IO {

struct EmitterOptions;

void AppendTileset(pugi::xml_node mapNode,
                   const TilesetData& tileset,
                   const std::filesystem::path& dir,
                   const EmitterOptions& options);

}  // namespace Tactile::IO
