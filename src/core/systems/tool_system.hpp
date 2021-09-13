#pragma once

#include <entt.hpp>  // registry, dispatcher

#include "core/mouse_info.hpp"
#include "core/tool_type.hpp"

namespace Tactile::Sys {

void SelectTool(entt::registry& registry, ToolType tool);

void ToolOnPressed(entt::registry& registry,
                   entt::dispatcher& dispatcher,
                   const MouseInfo& mouse);

void ToolOnDragged(entt::registry& registry,
                   entt::dispatcher& dispatcher,
                   const MouseInfo& mouse);

void ToolOnReleased(entt::registry& registry,
                    entt::dispatcher& dispatcher,
                    const MouseInfo& mouse);

[[nodiscard]] auto IsStampEnabled(const entt::registry& registry) -> bool;

[[nodiscard]] auto IsEraserEnabled(const entt::registry& registry) -> bool;

[[nodiscard]] auto IsBucketEnabled(const entt::registry& registry) -> bool;

}  // namespace Tactile::Sys
