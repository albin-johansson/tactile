#pragma once

#include <entt.hpp>     // registry, entity
#include <filesystem>   // path
#include <pugixml.hpp>  // xml_node

namespace Tactile::IO {

void AppendProperties(const entt::registry& registry,
                      entt::entity entity,
                      pugi::xml_node node,
                      const std::filesystem::path& dir);

}  // namespace Tactile::IO
