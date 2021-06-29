#include "load_default_layout.hpp"

#include "gui/widgets/layers/layer_widget.hpp"
#include "gui/widgets/properties/properties_widget.hpp"
#include "gui/widgets/tilesets/tileset_widget.hpp"
#include "imgui_internal.h"

namespace Tactile {

void LoadDefaultLayout(ImGuiID id)
{
  ImGui::DockBuilderRemoveNodeChildNodes(id);

  // clang-format off
  const auto root = id;

  const auto tools = ImGui::DockBuilderSplitNode(id, ImGuiDir_Up, 0.04f, nullptr, &id);
  const auto props = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.15f, nullptr, &id);

  auto tilesets = ImGui::DockBuilderSplitNode(id, ImGuiDir_Right, 0.20f, nullptr, &id);
  const auto layers = ImGui::DockBuilderSplitNode(tilesets, ImGuiDir_Down, 0.25f, nullptr, &tilesets);
  // clang-format on

  ImGui::DockBuilderDockWindow("Viewport", root);
  ImGui::DockBuilderDockWindow("Toolbar", tools);
  ImGui::DockBuilderDockWindow("Properties", props);
  ImGui::DockBuilderDockWindow("Tilesets", tilesets);
  ImGui::DockBuilderDockWindow("Layers", layers);

  ImGui::DockBuilderGetNode(tools)->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
  ImGui::DockBuilderGetNode(root)->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

  ImGui::DockBuilderFinish(id);

  SetLayerWidgetVisible(true);
  SetPropertiesWidgetVisible(true);
  SetTilesetWidgetVisible(true);
}

}  // namespace Tactile
