#pragma once

#include <entt.hpp>     // registry, dispatcher
#include <string_view>  // string_view

#include "aliases/layer_id.hpp"

namespace Tactile {

void UpdateRenameLayerDialog(const entt::registry& registry,
                             entt::dispatcher& dispatcher);

void OpenRenameLayerDialog(LayerID id, std::string_view oldName);

}  // namespace Tactile
