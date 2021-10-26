#include "properties_dock.hpp"

#include <tactile-base/formatted_string.hpp>

#include <imgui.h>

#include "core/components/property_context.hpp"
#include "core/systems/property_system.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/widgets/alignment.hpp"
#include "editor/gui/widgets/common/button.hpp"
#include "editor/gui/widgets/common/centered_button.hpp"
#include "editor/gui/widgets/common/centered_text.hpp"
#include "editor/gui/widgets/common/window.hpp"
#include "editor/gui/widgets/properties/dialogs/add_property_dialog.hpp"
#include "editor/gui/widgets/properties/dialogs/change_property_type_dialog.hpp"
#include "editor/gui/widgets/properties/dialogs/rename_property_dialog.hpp"
#include "io/preferences.hpp"
#include "property_table.hpp"

namespace Tactile {
namespace {

constinit bool has_focus = false;

}  // namespace

void UpdatePropertiesDock(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  if (!Prefs::GetShowPropertiesDock()) {
    return;
  }

  constexpr auto flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
  bool isVisible = Prefs::GetShowPropertiesDock();

  Window dock{"Properties", flags, &isVisible};
  has_focus = dock.IsFocused();

  if (dock) {
    const auto& context = Sys::GetCurrentContext(registry);

    FormattedString contextName{"Context: {}", context.name};
    ImGui::TextUnformatted(contextName.GetData());

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

  UpdateAddPropertyDialog(registry, dispatcher);
  UpdateRenamePropertyDialog(registry, dispatcher);
  UpdateChangePropertyTypeDialog(dispatcher);

  Prefs::SetShowPropertiesDock(isVisible);
}

auto IsPropertyDockFocused() noexcept -> bool
{
  return has_focus;
}

}  // namespace Tactile
