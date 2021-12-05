#include "properties_dock.hpp"

#include <imgui.h>

#include "dialogs/add_property_dialog.hpp"
#include "dialogs/change_property_type_dialog.hpp"
#include "editor/gui/scoped.hpp"
#include "io/preferences.hpp"

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
    mPropertyTable.Update(registry, dispatcher);
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
