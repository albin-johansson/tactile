#pragma once

#include "imgui.h"

namespace tactile {

/// Updates the root dock space, call this function once each frame.
void UpdateDockSpace();

/// Resets all dock space widgets to the default layout.
void ResetLayout();

}  // namespace tactile
