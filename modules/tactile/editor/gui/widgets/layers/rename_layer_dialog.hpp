#pragma once

#include <string>  // string

#include <tactile-base/tactile_std.hpp>

#include <entt.hpp>  // registry, dispatcher

namespace Tactile {

void UpdateRenameLayerDialog(const entt::registry& registry,
                             entt::dispatcher& dispatcher);

void OpenRenameLayerDialog(LayerID id, std::string oldName);

}  // namespace Tactile
