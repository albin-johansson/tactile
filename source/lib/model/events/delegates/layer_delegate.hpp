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

#include "core/ecs/registry.hpp"
#include "model/events/layer_events.hpp"

namespace tactile {

void on_show_rename_layer_dialog(Registry& registry,
                                 const ShowRenameLayerDialogEvent& event);

void on_create_layer(Registry& registry, const CreateLayerEvent& event);

void on_remove_layer(Registry& registry, const RemoveLayerEvent& event);

void on_rename_layer(Registry& registry, const RenameLayerEvent& event);

void on_duplicate_layer(Registry& registry, const DuplicateLayerEvent& event);

void on_select_layer(Registry& registry, const SelectLayerEvent& event);

void on_move_layer_up(Registry& registry, const MoveLayerUpEvent& event);

void on_move_layer_down(Registry& registry, const MoveLayerDownEvent& event);

void on_set_layer_opacity(Registry& registry, const SetLayerOpacityEvent& event);

void on_set_layer_visible(Registry& registry, const SetLayerVisibleEvent& event);

}  // namespace tactile
