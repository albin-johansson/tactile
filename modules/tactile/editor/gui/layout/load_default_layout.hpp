#pragma once

#include <imgui.h>

namespace tactile {

/**
 * \brief Loads the default layout for the specified dock space.
 *
 * \ingroup gui
 *
 * \param id the ID of the dock space.
 * \param resetVisibility `true` if the docks should have their visibility reset;
 * `false` otherwise.
 */
void LoadDefaultLayout(ImGuiID id, bool resetVisibility);

}  // namespace tactile
