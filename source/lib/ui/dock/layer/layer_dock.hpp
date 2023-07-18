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

#include "model/model_view.hpp"
#include "ui/dock/layer/add_layer_context_menu.hpp"
#include "ui/dock/layer/dialogs/rename_layer_dialog.hpp"

namespace tactile {

struct LayerDockState final {
  ui::AddLayerContextMenu add_layer_context_menu;
  ui::RenameLayerDialogState rename_layer_dialog;
  bool has_focus {};
};

void push_layer_dock_widget(ModelView& model, LayerDockState& state);

[[nodiscard]] auto is_layer_dock_enabled(const Registry& registry) -> bool;

}  // namespace tactile::ui
