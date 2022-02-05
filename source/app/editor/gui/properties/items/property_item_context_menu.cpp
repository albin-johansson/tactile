#include "property_item_context_menu.hpp"

#include <imgui.h>

#include "editor/events/property_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"

namespace tactile {

auto PropertyItemContextMenu(entt::dispatcher& dispatcher,
                             const std::string& name,
                             PropertyItemContextMenuState& state) -> bool
{
  if (auto popup = scoped::popup::for_item("##PropertyItemContext"); popup.is_open()) {
    state.show_rename_dialog = ImGui::MenuItem(TAC_ICON_EDIT " Rename Property...");
    state.show_change_type_dialog = ImGui::MenuItem(ICON_FA_SHAPES " Change Type...");

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Property")) {
      dispatcher.enqueue<remove_property_event>(name);
    }

    return true;
  }
  else {
    return false;
  }
}

}  // namespace tactile
