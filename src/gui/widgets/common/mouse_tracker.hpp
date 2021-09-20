#pragma once

#include <imgui.h>

#include <entt.hpp>  // dispatcher

namespace Tactile {

struct CanvasInfo;

/// \addtogroup gui
/// \{

void UpdateViewportOffset(const CanvasInfo& canvas, entt::dispatcher& dispatcher);

/// \} End of group gui

}  // namespace Tactile
