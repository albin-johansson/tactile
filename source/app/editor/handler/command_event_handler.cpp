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

#include "command_event_handler.hpp"

#include <entt/signal/dispatcher.hpp>

#include "model/cmd/command_stack.hpp"
#include "model/event/command_events.hpp"
#include "model/model.hpp"
#include "ui/app_context.hpp"

namespace tactile {
namespace {

void on_undo()
{
  if (auto* document = get_model().active_document()) {
    auto& commands = document->get_history();
    commands.undo();
  }
}

void on_redo()
{
  if (auto* document = get_model().active_document()) {
    auto& commands = document->get_history();
    commands.redo();
  }
}

void on_set_command_capacity(const SetCommandCapacityEvent event)
{
  get_model().set_command_capacity(event.capacity);
}

}  // namespace

void install_command_event_handler()
{
  auto& dispatcher = get_dispatcher();
  dispatcher.sink<UndoEvent>().connect<&on_undo>();
  dispatcher.sink<RedoEvent>().connect<&on_redo>();
  dispatcher.sink<SetCommandCapacityEvent>().connect<&on_set_command_capacity>();
}

}  // namespace tactile
