#include "properties_dock.hpp"

#include <utility>  // move

#include <imgui.h>

#include "dialogs/add_property_dialog.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"
#include "io/preferences.hpp"

namespace tactile {

void PropertiesDock::Update(const Model& model, entt::dispatcher& dispatcher)
{
  if (!prefs::GetShowPropertiesDock()) {
    return;
  }

  constexpr auto flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
  bool isVisible = prefs::GetShowPropertiesDock();

  scoped::Window dock{"Properties", flags, &isVisible};
  mHasFocus = dock.IsFocused();

  if (dock.IsOpen()) {
    const auto& registry = model.GetActiveRegistryRef();
    mPropertyTable.Update(registry, dispatcher);
  }

  mAddDialog.Update(model, dispatcher);
  mRenameDialog.Update(model, dispatcher);
  mChangeTypeDialog.Update(model, dispatcher);

  prefs::SetShowPropertiesDock(isVisible);
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
                                                  const attribute_type type)
{
  mChangeTypeDialog.Show(std::move(name), type);
}

}  // namespace tactile
