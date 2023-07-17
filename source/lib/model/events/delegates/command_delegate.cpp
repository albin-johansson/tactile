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

#include "command_delegate.hpp"

#include <concepts>  // invocable

#include "cmd/command_stack.hpp"
#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/events/command_events.hpp"

namespace tactile {
namespace {

template <std::invocable<CommandStack&> T>
void _with_current_command_stack(Registry& registry, T&& callable)
{
  const auto document_entity = sys::get_active_document(registry);

  if (document_entity != kNullEntity) {
    auto& commands = registry.get<CommandStack>(document_entity);
    callable(commands);
  }
}

}  // namespace

void on_undo(Registry& registry, const UndoEvent&)
{
  _with_current_command_stack(registry, [](CommandStack& commands) { commands.undo(); });
}

void on_redo(Registry& registry, const RedoEvent&)
{
  _with_current_command_stack(registry, [](CommandStack& commands) { commands.redo(); });
}

void on_set_command_capacity(Registry& registry, const SetCommandCapacityEvent& event)
{
  for (const auto [document_entity, document]: registry.each<Document>()) {
    auto& commands = registry.get<CommandStack>(document_entity);
    commands.set_capacity(event.capacity);
  }
}

}  // namespace tactile
