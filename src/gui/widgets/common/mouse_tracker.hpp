#pragma once

#include <imgui.h>

#include <entt.hpp>  // dispatcher

namespace Tactile {

struct CanvasInfo;

/// \addtogroup gui
/// \{

[[nodiscard]] auto MouseTracker(const CanvasInfo& canvas, ImVec2 scrollOffset)
    -> ImVec2;

void UpdateViewportOffset(const CanvasInfo& canvas, entt::dispatcher& dispatcher);

/// \} End of group gui

}  // namespace Tactile
