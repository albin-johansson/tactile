#include "load_default_layout.hpp"

#include <imgui_internal.h>

#include "io/preferences.hpp"

namespace Tactile {

void LoadDefaultLayout(ImGuiID id, const bool resetVisibility)
{
  ImGui::DockBuilderRemoveNodeChildNodes(id);

  // clang-format off
  const auto root = id;
  const auto top = ImGui::DockBuilderSplitNode(id, ImGuiDir_Up, 0.04f, nullptr, &id);

  auto right = ImGui::DockBuilderSplitNode(id, ImGuiDir_Right, 0.20f, nullptr, &id);
  const auto rightBottom = ImGui::DockBuilderSplitNode(right, ImGuiDir_Down, 0.25f, nullptr, &right);
  // clang-format on

  ImGui::DockBuilderDockWindow("Viewport", root);
  ImGui::DockBuilderDockWindow("Toolbar", top);
  ImGui::DockBuilderDockWindow("Tilesets", right);
  ImGui::DockBuilderDockWindow("Properties", right);
  ImGui::DockBuilderDockWindow("Layers", rightBottom);

  ImGui::DockBuilderFinish(id);

  if (resetVisibility)
  {
    Prefs::SetShowLayerDock(true);
    Prefs::SetShowPropertiesDock(true);
    Prefs::SetShowTilesetDock(true);
  }
}

}  // namespace Tactile
