#pragma once

#include <entt/entt.hpp>  // dispatcher
#include <imgui.h>

namespace Tactile {

struct CanvasInfo;

/// \addtogroup gui
/// \{

void UpdateViewportOffset(const CanvasInfo& canvas, entt::dispatcher& dispatcher);

/// \} End of group gui

}  // namespace Tactile
