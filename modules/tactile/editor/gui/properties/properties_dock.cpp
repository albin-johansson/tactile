#include "properties_dock.hpp"

#include <utility>  // move

#include <imgui.h>

#include "assert.hpp"
#include "dialogs/add_property_dialog.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"
#include "io/preferences.hpp"

namespace Tactile {

void PropertiesDock::Update(const Model& model, entt::dispatcher& dispatcher)
{
  if (!Prefs::GetShowPropertiesDock()) {
    return;
  }

  constexpr auto flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
  bool isVisible = Prefs::GetShowPropertiesDock();

  Scoped::Window dock{"Properties", flags, &isVisible};
  mHasFocus = dock.IsFocused();

  if (dock.IsOpen()) {
    const auto* registry = model.GetActiveRegistry();
    TACTILE_ASSERT(registry);

    mPropertyTable.Update(*registry, dispatcher);
  }

  mAddDialog.Update(model, dispatcher);
  mRenameDialog.Update(model, dispatcher);
  mChangeTypeDialog.Update(model, dispatcher);

  Prefs::SetShowPropertiesDock(isVisible);
}

void PropertiesDock::ShowAddPropertyDialog()
{
  mAddDialog.Show();
}

void PropertiesDock::ShowRenamePropertyDialog(const std::string& name)
{
  mRenameDialog.Show(name);
}

void PropertiesDock::ShowChangePropertyTypeDialog(std::string name,
                                                  const PropertyType type)
{
  mChangeTypeDialog.Show(std::move(name), type);
}

}  // namespace Tactile
