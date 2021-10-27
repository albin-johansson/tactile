#include "load_default_layout.hpp"

#include <imgui_internal.h>

#include "io/preferences.hpp"

namespace Tactile {

void LoadDefaultLayout(ImGuiID id, const bool resetVisibility)
{
  ImGui::DockBuilderRemoveNodeChildNodes(id);

  const auto root = id;
  const auto left = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.04f, nullptr, &id);

  auto right = ImGui::DockBuilderSplitNode(id, ImGuiDir_Right, 0.20f, nullptr, &id);
  const auto rightBottom =
      ImGui::DockBuilderSplitNode(right, ImGuiDir_Down, 0.25f, nullptr, &right);

  const auto bottom = ImGui::DockBuilderSplitNode(id, ImGuiDir_Down, 0.25f, nullptr, &id);

  ImGui::DockBuilderDockWindow("Viewport", root);
  ImGui::DockBuilderDockWindow("Toolbar", left);
  ImGui::DockBuilderDockWindow("Tilesets", right);
  ImGui::DockBuilderDockWindow("Properties", right);
  ImGui::DockBuilderDockWindow("Layers", rightBottom);
  ImGui::DockBuilderDockWindow("Log", bottom);

  ImGui::DockBuilderFinish(id);

  if (resetVisibility) {
    Prefs::ResetDockVisibilities();
  }
}

}  // namespace Tactile
