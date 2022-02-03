#include "properties_dock.hpp"

#include <utility>  // move

#include <imgui.h>

#include "dialogs/add_property_dialog.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"
#include "io/persistence/preferences.hpp"

namespace tactile {

void PropertiesDock::Update(const Model& model, entt::dispatcher& dispatcher)
{
  auto& prefs = get_preferences();
  bool visible = prefs.is_properties_dock_visible();

  if (!visible) {
    return;
  }

  constexpr auto flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
  scoped::Window dock{"Properties", flags, &visible};
  mHasFocus = dock.IsFocused();

  if (dock.IsOpen()) {
    const auto& registry = model.GetActiveRegistryRef();
    mPropertyTable.Update(registry, dispatcher);
  }

  mAddDialog.Update(model, dispatcher);
  mRenameDialog.Update(model, dispatcher);
  mChangeTypeDialog.Update(model, dispatcher);

  prefs.set_properties_dock_visible(visible);
}

void PropertiesDock::ShowAddPropertyDialog()
{
  mAddDialog.Open();
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
