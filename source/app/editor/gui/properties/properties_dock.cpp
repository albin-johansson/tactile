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
  scoped::window dock{"Properties", flags, &visible};
  mHasFocus = dock.has_focus();

  if (dock.is_open()) {
    const auto& registry = model.get_active_registry();
    mPropertyTable.Update(registry, dispatcher);
  }

  mAddDialog.update(model, dispatcher);
  mRenameDialog.update(model, dispatcher);
  mChangeTypeDialog.update(model, dispatcher);

  prefs.set_properties_dock_visible(visible);
}

void PropertiesDock::ShowAddPropertyDialog()
{
  mAddDialog.Open();
}

void PropertiesDock::ShowRenamePropertyDialog(const std::string& name)
{
  mRenameDialog.show(name);
}

void PropertiesDock::ShowChangePropertyTypeDialog(std::string name,
                                                  const attribute_type type)
{
  mChangeTypeDialog.Show(std::move(name), type);
}

}  // namespace tactile
