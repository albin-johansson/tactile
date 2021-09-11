#pragma once

#include <entt.hpp>  // registry, dispatcher
#include <string>    // string

#include "aliases/layer_id.hpp"

namespace Tactile {

void UpdateRenameLayerDialog(const entt::registry& registry,
                             entt::dispatcher& dispatcher);

void OpenRenameLayerDialog(LayerID id, std::string oldName);

}  // namespace Tactile
