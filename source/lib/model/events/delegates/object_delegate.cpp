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

#include "object_delegate.hpp"

#include "cmd/object/move_object.hpp"
#include "cmd/object/remove_object.hpp"
#include "cmd/object/rename_object.hpp"
#include "cmd/object/set_object_tag.hpp"
#include "cmd/object/set_object_visible.hpp"
#include "model/documents/command_system.hpp"
#include "model/documents/document_system.hpp"
#include "ui/widget_state.hpp"

namespace tactile {

void on_move_object(Registry& registry, const MoveObjectEvent& event)
{
  sys::try_execute<cmd::MoveObject>(registry, event.object, event.old_pos, event.new_pos);
}

void on_set_object_visible(Registry& registry, const SetObjectVisibleEvent& event)
{
  sys::try_execute<cmd::SetObjectVisible>(registry, event.object, event.visible);
}

void on_set_object_tag(Registry& registry, const SetObjectTagEvent& event)
{
  sys::try_execute<cmd::SetObjectTag>(registry, event.object, event.tag);
}

void on_set_object_name(Registry& registry, const SetObjectNameEvent& event)
{
  sys::try_execute<cmd::RenameObject>(registry, event.object, event.name);
}

void on_duplicate_object([[maybe_unused]] Registry& registry,
                         [[maybe_unused]] const DuplicateObjectEvent& event)
{
  // TODO implement command
}

void on_remove_object(Registry& registry, const RemoveObjectEvent& event)
{
  sys::try_execute<cmd::RemoveObject>(registry, event.object_layer, event.object);
}

void on_spawn_object_context_menu(Registry& registry, const SpawnObjectContextMenuEvent&)
{
  auto& widgets = registry.get<WidgetState>();
  widgets.editor_dock.central_map_viewport.should_open_object_context_menu = true;
}

}  // namespace tactile
