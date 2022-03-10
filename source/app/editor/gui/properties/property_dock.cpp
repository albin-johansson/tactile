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

#include "property_dock.hpp"

#include <utility>  // move

#include <imgui.h>

#include "dialogs/add_property_dialog.hpp"
#include "editor/model.hpp"
#include "io/persistence/preferences.hpp"

namespace tactile {

PropertyDock::PropertyDock()
    : ADockWidget{"Properties",
                  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar}
{
  set_close_button_enabled(true);
}

void PropertyDock::on_update(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  const auto& registry = model.get_active_registry();
  mPropertyTable.update(registry, dispatcher);

  mAddDialog.update(model, dispatcher);
  mRenameDialog.update(model, dispatcher);
  mChangeTypeDialog.update(model, dispatcher);
}

void PropertyDock::show_add_property_dialog()
{
  mAddDialog.open();
}

void PropertyDock::show_rename_property_dialog(const std::string& name)
{
  mRenameDialog.show(name);
}

void PropertyDock::show_change_property_type_dialog(std::string name,
                                                    const AttributeType type)
{
  mChangeTypeDialog.show(std::move(name), type);
}

void PropertyDock::set_visible(const bool visible)
{
  get_preferences().set_property_dock_visible(visible);
}

auto PropertyDock::is_visible() const -> bool
{
  return get_preferences().is_property_dock_visible();
}

}  // namespace tactile
