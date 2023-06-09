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

#include "model/event/object_events.hpp"
#include "model/model.hpp"

namespace tactile {

void on_move_object(Model& model, const MoveObjectEvent& event);

void on_set_object_visible(Model& model, const SetObjectVisibleEvent& event);

void on_set_object_tag(Model& model, const SetObjectTagEvent& event);

void on_set_object_name(Model& model, const SetObjectNameEvent& event);

void on_duplicate_object(Model& model, const DuplicateObjectEvent& event);

void on_remove_object(Model& model, const RemoveObjectEvent& event);

void on_spawn_object_context_menu(Model& model, const SpawnObjectContextMenuEvent& event);

}  // namespace tactile
