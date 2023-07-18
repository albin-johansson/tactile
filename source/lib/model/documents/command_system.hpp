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
#include "common/macros.hpp"
#include "common/primitives.hpp"
#include "common/type/ecs.hpp"
#include "common/type/hash_map.hpp"
#include "model/documents/document_system.hpp"
#include "model/registry.hpp"
#include "model/system.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_STRUCT(SetCommandCapacityEvent)

class CommandSystem final : public System {
 public:
  void undo(Registry& registry);

  void redo(Registry& registry);

  void set_command_capacity(Registry& registry, usize capacity);

  void on_set_command_capacity(Registry& registry, const SetCommandCapacityEvent& event);

  [[nodiscard]] auto is_save_possible(const Registry& registry) -> bool;

  [[nodiscard]] auto is_undo_possible(const Registry& registry) -> bool;

  [[nodiscard]] auto is_redo_possible(const Registry& registry) -> bool;

 private:
  [[nodiscard]] auto _get_active_command_stack(Registry& registry) -> CommandStack*;
};

namespace sys {

/**
 * Tries to push a command to the currently active document, if there is one.
 *
 * \warning The provided model must outlive the created command, since all commands store
 *          a raw pointer to their associated model instance.
 *
 * \tparam T a command type.
 *
 * \param model the associated registry.
 * \param args  pack of command constructor arguments.
 */
template <typename T, typename... Args>
void try_execute(Registry& registry, Args&&... args)
{
  const auto document_entity = get_active_document(registry);
  if (document_entity != kNullEntity) {
    auto& command_stack = registry.get<CommandStack>(document_entity);
    command_stack.push<T>(&registry, std::forward<Args>(args)...);
  }
}

}  // namespace sys
}  // namespace tactile
