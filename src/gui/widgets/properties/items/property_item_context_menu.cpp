#include "property_item_context_menu.hpp"

#include <imgui.h>

#include "events/property_events.hpp"
#include "gui/icons.hpp"

namespace Tactile {

auto PropertyItemContextMenu(entt::dispatcher& dispatcher,
                             const std::string& name,
                             PropertyItemContextMenuState& state) -> bool
{
  const auto isOpen = ImGui::BeginPopupContextItem("##PropertyItemContext");
  if (isOpen) {
    state.show_rename_dialog = ImGui::MenuItem(TAC_ICON_EDIT " Rename Property...");
    state.show_change_type_dialog = ImGui::MenuItem(ICON_FA_SHAPES " Change Type...");

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Property")) {
      dispatcher.enqueue<RemovePropertyEvent>(name);
    }

    ImGui::EndPopup();
  }

  return isOpen;
}

}  // namespace Tactile
