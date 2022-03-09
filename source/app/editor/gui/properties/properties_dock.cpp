/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "properties_dock.hpp"

#include <utility>  // move

#include <imgui.h>

#include "dialogs/add_property_dialog.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"
#include "io/persistence/preferences.hpp"

namespace tactile {

void PropertiesDock::Update(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  auto& prefs = get_preferences();
  bool visible = prefs.is_properties_dock_visible();

  if (!visible) {
    return;
  }

  constexpr auto flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
  scoped::Window dock{"Properties", flags, &visible};
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
                                                  const AttributeType type)
{
  mChangeTypeDialog.Show(std::move(name), type);
}

}  // namespace tactile
