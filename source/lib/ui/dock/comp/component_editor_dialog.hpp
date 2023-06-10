/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "common/type/dispatcher.hpp"
#include "common/type/ecs.hpp"
#include "model/model.hpp"
#include "ui/dock/comp/dialogs/new_comp_dialog.hpp"
#include "ui/dock/comp/dialogs/new_component_attribute_dialog.hpp"
#include "ui/dock/comp/dialogs/rename_component_attribute_dialog.hpp"
#include "ui/dock/comp/dialogs/rename_component_dialog.hpp"

namespace tactile::ui {

struct ComponentEditorDialogState final {
  Entity active_definition {kNullEntity};
  NewCompDialogState new_comp_dialog;
  RenameCompDialogState rename_comp_dialog;
  NewCompAttrDialogState new_comp_attr_dialog;
  RenameCompAttrDialogState rename_comp_attr_dialog;
  bool should_open {};
};

void push_component_editor_dialog(const Model& model,
                                  ComponentEditorDialogState& state,
                                  Dispatcher& dispatcher);

}  // namespace tactile::ui
