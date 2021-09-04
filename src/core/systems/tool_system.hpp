#pragma once

#include <entt.hpp>  // registry

#include "core/mouse_info.hpp"
#include "core/mouse_tool_type.hpp"

namespace Tactile::Sys {

void SelectTool(entt::registry& registry, MouseToolType tool);

void ToolOnPressed(entt::registry& registry, const MouseInfo& mouse);

void ToolOnDragged(entt::registry& registry, const MouseInfo& mouse);

void ToolOnReleased(entt::registry& registry, const MouseInfo& mouse);

[[nodiscard]] auto IsStampEnabled(const entt::registry& registry) -> bool;

[[nodiscard]] auto IsEraserEnabled(const entt::registry& registry) -> bool;

[[nodiscard]] auto IsBucketEnabled(const entt::registry& registry) -> bool;

}  // namespace Tactile::Sys
