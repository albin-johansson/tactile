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

#pragma once

#include <string>  // string

#include <entt/entt.hpp>

#include "dialogs/add_property_dialog.hpp"
#include "dialogs/change_property_type_dialog.hpp"
#include "dialogs/rename_property_dialog.hpp"
#include "editor/gui/common/dock_widget.hpp"
#include "property_table.hpp"
#include "tactile.hpp"

namespace tactile {

class DocumentModel;

class PropertyDock final : public ADockWidget
{
 public:
  PropertyDock();

  void show_add_property_dialog();

  void show_rename_property_dialog(const std::string& name);

  void show_change_property_type_dialog(std::string name, AttributeType type);

 protected:
  void on_update(const DocumentModel& model, entt::dispatcher& dispatcher) override;

  void set_visible(bool visible) override;

  [[nodiscard]] auto is_visible() const -> bool override;

 private:
  PropertyTable mPropertyTable;
  AddPropertyDialog mAddDialog;
  RenamePropertyDialog mRenameDialog;
  ChangePropertyTypeDialog mChangeTypeDialog;
};

}  // namespace tactile
