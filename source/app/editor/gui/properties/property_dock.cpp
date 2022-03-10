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
#include "dialogs/change_property_type_dialog.hpp"
#include "dialogs/rename_property_dialog.hpp"
#include "editor/model.hpp"
#include "io/persistence/preferences.hpp"
#include "property_table.hpp"

namespace tactile {

struct PropertyDock::Data final
{
  PropertyTable property_table;
  AddPropertyDialog add_dialog;
  RenamePropertyDialog rename_dialog;
  ChangePropertyTypeDialog change_type_dialog;
};

PropertyDock::PropertyDock()
    : ADockWidget{"Properties",
                  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar}
    , mData{std::make_unique<Data>()}
{
  set_close_button_enabled(true);
}

PropertyDock::~PropertyDock() noexcept = default;

void PropertyDock::on_update(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  const auto& registry = model.get_active_registry();
  mData->property_table.update(registry, dispatcher);

  mData->add_dialog.update(model, dispatcher);
  mData->rename_dialog.update(model, dispatcher);
  mData->change_type_dialog.update(model, dispatcher);
}

void PropertyDock::show_add_property_dialog()
{
  mData->add_dialog.open();
}

void PropertyDock::show_rename_property_dialog(const std::string& name)
{
  mData->rename_dialog.show(name);
}

void PropertyDock::show_change_property_type_dialog(std::string name,
                                                    const AttributeType type)
{
  mData->change_type_dialog.show(std::move(name), type);
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
