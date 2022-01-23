#pragma once

#include <entt/entt.hpp>

struct ImVec2;

namespace tactile {

/// \addtogroup gui
/// \{

void UpdateViewportOffset(const ImVec2& viewportSize, entt::dispatcher& dispatcher);

/// \} End of group gui

}  // namespace tactile
