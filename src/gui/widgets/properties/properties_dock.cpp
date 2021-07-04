#include "properties_dock.hpp"

#include <imgui.h>

#include "aliases/ints.hpp"
#include "core/events/properties/set_property_value_event.hpp"
#include "core/model.hpp"
#include "gui/icons.hpp"
#include "gui/widgets/alignment.hpp"
#include "gui/widgets/button_ex.hpp"
#include "gui/widgets/centered_text.hpp"
#include "gui/widgets/help_marker.hpp"
#include "gui/widgets/properties/dialogs/add_property_dialog.hpp"
#include "gui/widgets/properties/dialogs/change_property_type_dialog.hpp"
#include "gui/widgets/properties/dialogs/rename_property_dialog.hpp"
#include "gui/widgets/properties/properties_content_widget.hpp"
#include "io/preferences.hpp"

namespace Tactile {

void UpdatePropertiesDock(const Model& model, entt::dispatcher& dispatcher)
{
  if (!Prefs::GetShowPropertiesDock() || !model.GetActiveMapId())
  {
    return;
  }

  constexpr auto flags =
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
  bool isVisible = Prefs::GetShowPropertiesDock();
  if (ImGui::Begin("Properties", &isVisible, flags))
  {
    const auto* document = model.GetActiveDocument();

    if (ButtonEx(TAC_ICON_ADD, "Add property."))
    {
      OpenAddPropertyDialog();
    }

    ImGui::SameLine();
    ImGui::Text("Context: %s", document->GetContextName().data());
    ImGui::SameLine();
    HelpMarker("The owner of the shown properties.");
    ImGui::Separator();

    if (document->GetPropertyCount() != 0)
    {
      UpdatePropertiesContentWidget(*document, dispatcher);
    }
    else
    {
      PrepareVerticalAlignmentCenter(1);
      CenteredText("No available properties!");
    }
  }

  if (const auto* document = model.GetActiveDocument())
  {
    UpdateAddPropertyDialog(*document, dispatcher);
    UpdateRenamePropertyDialog(*document, dispatcher);
    UpdateChangePropertyTypeDialog(*document, dispatcher);
  }

  Prefs::SetShowPropertiesDock(isVisible);
  ImGui::End();
}

}  // namespace Tactile
