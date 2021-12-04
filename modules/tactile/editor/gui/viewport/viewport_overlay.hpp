#pragma once

#include <entt/entt.hpp>  // registry

namespace Tactile {

struct ViewportCursorInfo;

void UpdateViewportOverlay(const entt::registry& registry,
                           const ViewportCursorInfo& cursor);

}  // namespace Tactile
