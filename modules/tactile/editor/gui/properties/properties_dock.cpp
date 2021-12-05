#include "properties_dock.hpp"

#include <imgui.h>

#include "editor/gui/properties/dialogs/add_property_dialog.hpp"
#include "editor/gui/properties/dialogs/change_property_type_dialog.hpp"
#include "editor/gui/scoped.hpp"
#include "io/preferences.hpp"
#include "property_table.hpp"

namespace Tactile {

void PropertiesDock::Update(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  if (!Prefs::GetShowPropertiesDock()) {
    return;
  }

  constexpr auto flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
  bool isVisible = Prefs::GetShowPropertiesDock();

  Scoped::Window dock{"Properties", flags, &isVisible};
  mHasFocus = dock.IsFocused();

  if (dock.IsOpen()) {
    UpdatePropertyTable(registry, dispatcher);
  }

  UpdateAddPropertyDialog(registry, dispatcher);
  mRenameDialog.Update(registry, dispatcher);
  UpdateChangePropertyTypeDialog(dispatcher);

  Prefs::SetShowPropertiesDock(isVisible);
}

void PropertiesDock::ShowRenamePropertyDialog(const std::string& name)
{
  mRenameDialog.Show(name);
}

}  // namespace Tactile
