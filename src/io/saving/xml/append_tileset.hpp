#pragma once

#include <entt.hpp>     // registry
#include <filesystem>   // path
#include <pugixml.hpp>  // xml_node

namespace Tactile::IO {

void AppendTileset(pugi::xml_node mapNode,
                   const entt::registry& registry,
                   entt::entity tilesetEntity,
                   const std::filesystem::path& dir);

}  // namespace Tactile::IO
