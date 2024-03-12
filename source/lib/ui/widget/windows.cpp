// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "windows.hpp"

#include <imgui.h>
#include <imgui_internal.h>

namespace tactile::ui {

void remove_tab_bar_from_next_window()
{
  ImGuiWindowClass wc {};
  wc.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
  ImGui::SetNextWindowClass(&wc);
}

}  // namespace tactile::ui
