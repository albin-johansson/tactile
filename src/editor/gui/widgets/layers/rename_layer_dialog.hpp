#pragma once

#include <string>  // string

#include <entt.hpp>  // registry, dispatcher

#include <tactile-base/tactile_std.hpp>

namespace Tactile {

void UpdateRenameLayerDialog(const entt::registry& registry,
                             entt::dispatcher& dispatcher);

void OpenRenameLayerDialog(LayerID id, std::string oldName);

}  // namespace Tactile
