#pragma once

#include <entt.hpp>     // registry, entity
#include <filesystem>   // path
#include <pugixml.hpp>  // xml_node

#include "aliases/layer_id.hpp"

namespace Tactile::IO {

void AppendLayer(pugi::xml_node mapNode,
                 const entt::registry& registry,
                 entt::entity layerEntity,
                 const std::filesystem::path& dir);

}  // namespace Tactile::IO
