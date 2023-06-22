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

#include <utility>  // forward

#include "cmd/command_stack.hpp"
#include "model/documents/document_system.hpp"
#include "model/model.hpp"

namespace tactile::sys {

/// Indicates whether the 'save' action is currently available.
[[nodiscard]] auto is_save_possible(const Model& model) -> bool;

/// Indicates whether the 'undo' action is currently available.
[[nodiscard]] auto is_undo_possible(const Model& model) -> bool;

/// Indicates whether the 'redo' action is currently available.
[[nodiscard]] auto is_redo_possible(const Model& model) -> bool;

/**
 * Tries to push a command to the currently active document, if there is one.
 *
 * \warning The provided model must outlive the created command, since all commands store
 *          a raw pointer to their associated model instance.
 *
 * \tparam T a command type.
 *
 * \param model the associated model.
 * \param args  pack of command constructor arguments.
 */
template <typename T, typename... Args>
void try_execute(Model& model, Args&&... args)
{
  const auto document_entity = get_active_document(model);
  if (document_entity != kNullEntity) {
    auto& command_stack = model.get<CommandStack>(document_entity);
    command_stack.push<T>(&model, std::forward<Args>(args)...);
  }
}

}  // namespace tactile::sys
