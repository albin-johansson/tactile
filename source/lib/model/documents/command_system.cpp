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

#include "model/documents/document_components.hpp"

namespace tactile::sys {

auto is_save_possible(const Registry& registry) -> bool
{
  const auto document_entity = registry.get<DocumentContext>().active_document;

  if (document_entity != kNullEntity) {
    const auto& commands = registry.get<CommandStack>(document_entity);
    return !commands.is_clean();
  }

  return false;
}

auto is_undo_possible(const Registry& registry) -> bool
{
  const auto document_entity = registry.get<DocumentContext>().active_document;

  if (document_entity != kNullEntity) {
    const auto& commands = registry.get<CommandStack>(document_entity);
    return commands.can_undo();
  }

  return false;
}

auto is_redo_possible(const Registry& registry) -> bool
{
  const auto document_entity = registry.get<DocumentContext>().active_document;

  if (document_entity != kNullEntity) {
    const auto& commands = registry.get<CommandStack>(document_entity);
    return commands.can_redo();
  }

  return false;
}

}  // namespace tactile::sys
