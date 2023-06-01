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

#include "model/event/layer_events.hpp"
#include "model/model.hpp"

namespace tactile {

void on_create_layer(Model& model, const CreateLayerEvent& event);

void on_remove_layer(Model& model, const RemoveLayerEvent& event);

void on_rename_layer(Model& model, const RenameLayerEvent& event);

void on_duplicate_layer(Model& model, const DuplicateLayerEvent& event);

void on_select_layer(Model& model, const SelectLayerEvent& event);

void on_move_layer_up(Model& model, const MoveLayerUpEvent& event);

void on_move_layer_down(Model& model, const MoveLayerDownEvent& event);

void on_set_layer_opacity(Model& model, const SetLayerOpacityEvent& event);

void on_set_layer_visible(Model& model, const SetLayerVisibleEvent& event);

}  // namespace tactile
