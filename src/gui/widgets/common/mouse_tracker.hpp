#pragma once

#include <imgui.h>

#include <entt.hpp>  // dispatcher

namespace Tactile {

struct CanvasInfo;

[[nodiscard]] auto MouseTracker(const CanvasInfo& canvas, ImVec2 scrollOffset)
    -> ImVec2;

void UpdateViewportOffset(const CanvasInfo& canvas, entt::dispatcher& dispatcher);

}  // namespace Tactile
