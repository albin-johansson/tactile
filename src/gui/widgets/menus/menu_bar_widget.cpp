#include "menu_bar_widget.hpp"

#include <imgui.h>

#include "gui/widgets/menus/edit_menu.hpp"
#include "gui/widgets/menus/file_menu.hpp"
#include "gui/widgets/menus/help_menu.hpp"
#include "gui/widgets/menus/view_menu.hpp"

namespace Tactile {

void UpdateMenuBarWidget(const Model& model, entt::dispatcher& dispatcher)
{
  if (ImGui::BeginMainMenuBar())
  {
    UpdateFileMenu(model, dispatcher);
    UpdateEditMenu(model, dispatcher);
    UpdateViewMenu(model, dispatcher);
    UpdateHelpMenu();

    ImGui::EndMainMenuBar();
  }

  UpdateFileMenuWindows(dispatcher);
  UpdateEditMenuWindows(dispatcher);
  UpdateHelpMenuWindows();
}

}  // namespace Tactile
