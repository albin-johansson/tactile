#pragma once

#include <entt.hpp>     // registry
#include <filesystem>   // path
#include <pugixml.hpp>  // xml_node

#include "core/components/tileset.hpp"

namespace Tactile::IO {

void AppendFancyTiles(pugi::xml_node node,
                      const entt::registry& registry,
                      const Tileset& tileset,
                      const std::filesystem::path& dir);

}  // namespace Tactile::IO
