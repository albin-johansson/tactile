// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/editor_dock_widget.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "tactile/core/model/model.hpp"
#include "tactile/core/ui/common/styling.hpp"
#include "tactile/core/ui/common/windows.hpp"

namespace tactile::core {

void EditorDockWidget::update(const Model&, EventDispatcher&)
{
  ImGuiWindowClass window_class {};
  window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
  ImGui::SetNextWindowClass(&window_class);

  const ScopedStyle window_padding {ImGuiStyleVar_WindowPadding, ImVec2 {4, 4}};
  const ScopedStyle window_border_size {ImGuiStyleVar_WindowBorderSize, 0.0f};

  ScopedWindowOptions window_options {};
  window_options.flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav;

  if (const ScopedWindow window {"##EditorDock", window_options}; window.is_open()) {
  }
}

}  // namespace tactile::core
