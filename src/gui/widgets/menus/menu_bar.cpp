#include "menu_bar.hpp"

#include <imgui.h>

#include "edit_menu.hpp"
#include "file_menu.hpp"
#include "help_menu.hpp"
#include "map_menu.hpp"
#include "view_menu.hpp"

namespace Tactile {

void UpdateMenuBar(const Model& model, entt::dispatcher& dispatcher)
{
  if (ImGui::BeginMainMenuBar()) {
    UpdateFileMenu(model, dispatcher);
    UpdateEditMenu(model, dispatcher);
    UpdateViewMenu(model, dispatcher);
    UpdateMapMenu(model, dispatcher);
    UpdateHelpMenu();

    ImGui::EndMainMenuBar();
  }

  UpdateFileMenuWindows(dispatcher);
  UpdateEditMenuWindows(dispatcher);
  UpdateMapMenuWindows(dispatcher);
  UpdateHelpMenuWindows();
}

}  // namespace Tactile
