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

#include "common/type/ecs.hpp"
#include "common/type/maybe.hpp"
#include "common/type/string.hpp"
#include "model/model_view.hpp"
#include "ui/dock/property/dialogs/new_property_dialog.hpp"
#include "ui/dock/property/dialogs/rename_property_dialog.hpp"
#include "ui/dock/property/dialogs/set_property_type_dialog.hpp"

namespace tactile {

struct PropertyItemContextMenuState final {
  bool show_add_dialog         : 1 {};
  bool show_rename_dialog      : 1 {};
  bool show_change_type_dialog : 1 {};
};

struct PropertyDockState final {
  Maybe<String> rename_target;
  Maybe<String> change_type_target;
  NewPropertyDialogState new_property_dialog;
  RenamePropertyDialogState rename_property_dialog;
  SetPropertyTypeDialogState set_property_type_dialog;
  PropertyItemContextMenuState context_state;
  bool has_focus : 1 {};
};

void push_property_dock_widget(ModelView& model, PropertyDockState& state);

[[nodiscard]] auto is_property_dock_focused() -> bool;

}  // namespace tactile::ui
