#pragma once

#include <entt/entt.hpp>  // registry

namespace tactile {

struct ViewportCursorInfo;

void UpdateViewportOverlay(const entt::registry& registry,
                           const ViewportCursorInfo& cursor);

}  // namespace tactile
