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

#include "command_system.hpp"

#include <spdlog/spdlog.h>

#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/events/command_events.hpp"

namespace tactile {

void CommandSystem::undo(Registry& registry)
{
  spdlog::trace("[CommandSystem] Undoing last command");

  if (auto* command_stack = _get_active_command_stack(registry)) {
    command_stack->undo();
  }
}

void CommandSystem::redo(Registry& registry)
{
  spdlog::trace("[CommandSystem] Redoing last undone command");

  if (auto* command_stack = _get_active_command_stack(registry)) {
    command_stack->redo();
  }
}

void CommandSystem::set_command_capacity(Registry& registry, const usize capacity)
{
  spdlog::debug("[CommandSystem] Setting command capacity to {}", capacity);

  for (const auto [document_entity, document]: registry.each<Document>()) {
    auto& command_stack = registry.get<CommandStack>(document_entity);
    command_stack.set_capacity(capacity);
  }
}

void CommandSystem::on_set_command_capacity(Registry& registry,
                                            const SetCommandCapacityEvent& event)
{
  set_command_capacity(registry, event.capacity);
}

auto CommandSystem::_get_active_command_stack(Registry& registry) -> CommandStack*
{
  const auto document_entity = sys::get_active_document(registry);

  if (document_entity != kNullEntity) {
    return registry.try_get<CommandStack>(document_entity);
  }

  return nullptr;
}

auto CommandSystem::is_save_possible(const Registry& registry) const -> bool
{
  const auto document_entity = registry.get<DocumentContext>().active_document;

  if (document_entity != kNullEntity) {
    const auto& commands = registry.get<CommandStack>(document_entity);
    return !commands.is_clean();
  }

  return false;
}

auto CommandSystem::is_undo_possible(const Registry& registry) const -> bool
{
  const auto document_entity = registry.get<DocumentContext>().active_document;

  if (document_entity != kNullEntity) {
    const auto& commands = registry.get<CommandStack>(document_entity);
    return commands.can_undo();
  }

  return false;
}

auto CommandSystem::is_redo_possible(const Registry& registry) const -> bool
{
  const auto document_entity = registry.get<DocumentContext>().active_document;

  if (document_entity != kNullEntity) {
    const auto& commands = registry.get<CommandStack>(document_entity);
    return commands.can_redo();
  }

  return false;
}

}  // namespace tactile
