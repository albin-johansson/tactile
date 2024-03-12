// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <imgui.h>

namespace tactile::ui {

/// Updates the root dock space, call this function once each frame.
void update_dock_space();

/// Loads the default layout for a dock space.
void load_default_layout(ImGuiID id, bool reset_visibility);

/// Resets all dock space widgets to the default layout.
void reset_layout();

}  // namespace tactile::ui
