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
#include "model/events/object_events.hpp"

namespace tactile {

void on_move_object(Registry& registry, const MoveObjectEvent& event);

void on_set_object_visible(Registry& registry, const SetObjectVisibleEvent& event);

void on_set_object_tag(Registry& registry, const SetObjectTagEvent& event);

void on_set_object_name(Registry& registry, const SetObjectNameEvent& event);

void on_duplicate_object(Registry& registry, const DuplicateObjectEvent& event);

void on_remove_object(Registry& registry, const RemoveObjectEvent& event);

void on_spawn_object_context_menu(Registry& registry,
                                  const SpawnObjectContextMenuEvent& event);

}  // namespace tactile
