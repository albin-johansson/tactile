#pragma once

#include <entt.hpp>  // registry

namespace Tactile {

struct CanvasInfo;
struct ViewportCursorInfo;

void ViewportOverlay(const entt::registry& registry,
                     const CanvasInfo& canvas,
                     const ViewportCursorInfo& cursor);

}  // namespace Tactile
