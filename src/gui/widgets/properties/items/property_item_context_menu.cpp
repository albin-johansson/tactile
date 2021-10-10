#include "property_item_context_menu.hpp"

#include <imgui.h>

#include "events/property_events.hpp"
#include "gui/icons.hpp"

namespace Tactile {

void PropertyItemContextMenu(entt::dispatcher& dispatcher,
                             const std::string& name,
                             PropertyItemContextMenuState& state)
{
  if (ImGui::BeginPopupContextItem("##PropertyItemContext")) {
    state.show_add_dialog = ImGui::MenuItem(TAC_ICON_ADD " Add New Property...");
    ImGui::Separator();

    state.show_rename_dialog = ImGui::MenuItem(TAC_ICON_EDIT " Rename Property...");
    state.show_change_type_dialog = ImGui::MenuItem(ICON_FA_SHAPES " Change Type...");

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Property")) {
      dispatcher.enqueue<RemovePropertyEvent>(name);
    }

    ImGui::EndPopup();
  }
}

}  // namespace Tactile
