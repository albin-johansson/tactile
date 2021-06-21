#include "show_gui.hpp"

#include "imgui.h"
#include "show_layers.hpp"
#include "show_map_viewport.hpp"
#include "show_menu_bar.hpp"
#include "show_properties.hpp"
#include "show_tilesets.hpp"
#include "show_tool_bar.hpp"

namespace tactile {

void ShowGui()
{
  ShowMenuBar();

  ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

  ShowToolBar();
  ShowMapViewport();
  ShowLayers();
  ShowProperties();
  ShowTilesets();

  ImGui::ShowDemoWindow();
}

}  // namespace tactile
