#pragma once

#include <entt.hpp>     // registry, entity
#include <filesystem>   // path
#include <pugixml.hpp>  // xml_node

namespace Tactile::IO {

void AppendFancyTiles(pugi::xml_node node,
                      const entt::registry& registry,
                      entt::entity tilesetEntity,
                      const std::filesystem::path& dir);

}  // namespace Tactile::IO
