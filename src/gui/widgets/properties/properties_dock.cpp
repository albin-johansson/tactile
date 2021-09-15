#include "properties_dock.hpp"

#include <imgui.h>

#include "aliases/ints.hpp"
#include "core/components/property_context.hpp"
#include "core/systems/property_system.hpp"
#include "dialogs/add_property_dialog.hpp"
#include "dialogs/change_property_type_dialog.hpp"
#include "dialogs/rename_property_dialog.hpp"
#include "events/property_events.hpp"
#include "gui/icons.hpp"
#include "gui/widgets/alignment.hpp"
#include "gui/widgets/common/button.hpp"
#include "gui/widgets/common/centered_text.hpp"
#include "gui/widgets/common/help_marker.hpp"
#include "io/preferences.hpp"
#include "properties_content_widget.hpp"

namespace Tactile {
namespace {

constinit bool has_focus = false;

}  // namespace

void UpdatePropertiesDock(const entt::registry& registry,
                          entt::dispatcher& dispatcher)
{
  // TODO context menu for entire dock

  if (!Prefs::GetShowPropertiesDock())
  {
    return;
  }

  constexpr auto flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
  bool isVisible = Prefs::GetShowPropertiesDock();
  if (ImGui::Begin("Properties", &isVisible, flags))
  {
    has_focus = ImGui::IsWindowFocused();
    const auto& context = Sys::GetCurrentContext(registry);

    if (Button(TAC_ICON_ADD, "Add property."))
    {
      OpenAddPropertyDialog();
    }

    ImGui::SameLine();
    ImGui::Text("Context: %s", context.name.c_str());
    ImGui::SameLine();
    HelpMarker("The owner of the shown properties.");
    ImGui::Separator();

    if (context.properties.empty())
    {
      PrepareVerticalAlignmentCenter(1);
      CenteredText("No available properties!");
    }
    else
    {
      UpdatePropertiesContentWidget(registry, dispatcher);
    }
  }
  else
  {
    has_focus = false;
  }

  UpdateAddPropertyDialog(registry, dispatcher);
  UpdateRenamePropertyDialog(registry, dispatcher);
  UpdateChangePropertyTypeDialog(registry, dispatcher);

  Prefs::SetShowPropertiesDock(isVisible);
  ImGui::End();
}

auto IsPropertyDockFocused() noexcept -> bool
{
  return has_focus;
}

}  // namespace Tactile
