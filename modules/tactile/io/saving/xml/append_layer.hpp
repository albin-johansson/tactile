#pragma once

#include <filesystem>  // path

#include <entt.hpp>     // registry, entity
#include <pugixml.hpp>  // xml_node

#include <tactile-base/tactile_std.hpp>

namespace Tactile::IO {

void AppendLayer(pugi::xml_node mapNode,
                 const entt::registry& registry,
                 entt::entity layerEntity,
                 const std::filesystem::path& dir);

}  // namespace Tactile::IO
