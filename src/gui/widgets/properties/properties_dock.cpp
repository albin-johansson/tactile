#include "properties_dock.hpp"

#include <imgui.h>

#include <rune/core/formatted_string.hpp>  // formatted_string

#include "core/components/property_context.hpp"
#include "core/systems/property_system.hpp"
#include "dialogs/add_property_dialog.hpp"
#include "dialogs/change_property_type_dialog.hpp"
#include "dialogs/rename_property_dialog.hpp"
#include "gui/icons.hpp"
#include "gui/widgets/alignment.hpp"
#include "gui/widgets/common/button.hpp"
#include "gui/widgets/common/centered_button.hpp"
#include "gui/widgets/common/centered_text.hpp"
#include "io/preferences.hpp"
#include "property_table.hpp"

namespace Tactile {
namespace {

constinit bool has_focus = false;

}  // namespace

void UpdatePropertiesDock(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  // TODO context menu for entire dock

  if (!Prefs::GetShowPropertiesDock()) {
    return;
  }

  constexpr auto flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
  bool isVisible = Prefs::GetShowPropertiesDock();
  if (ImGui::Begin("Properties", &isVisible, flags)) {
    has_focus = ImGui::IsWindowFocused();
    const auto& context = Sys::GetCurrentContext(registry);

    rune::formatted_string<128> contextName{"Context: {}", context.name};
    ImGui::TextUnformatted(contextName.data());

    if (context.properties.empty()) {
      PrepareVerticalAlignmentCenter(2.5f);
      CenteredText("Current context has no properties!");
      ImGui::Spacing();
      if (CenteredButton(TAC_ICON_PROPERTIES " Add property...")) {
        OpenAddPropertyDialog();
      }
    }
    else {
      ImGui::Separator();
      UpdatePropertyTable(registry, dispatcher);
    }
  }
  else {
    has_focus = false;
  }

  UpdateAddPropertyDialog(registry, dispatcher);
  UpdateRenamePropertyDialog(registry, dispatcher);
  UpdateChangePropertyTypeDialog(dispatcher);

  Prefs::SetShowPropertiesDock(isVisible);
  ImGui::End();
}

auto IsPropertyDockFocused() noexcept -> bool
{
  return has_focus;
}

}  // namespace Tactile
