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

#include "cmd/command_stack.hpp"
#include "common/macros.hpp"
#include "common/primitives.hpp"
#include "common/type/ecs.hpp"
#include "common/type/hash_map.hpp"
#include "model/system.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_STRUCT(SetCommandCapacityEvent)

class CommandSystem final : public System {
 public:
  CommandSystem(Registry& registry, Dispatcher& dispatcher);

  /// Subscribes to UndoEvent, RedoEvent, and SetCommandCapacityEvent.
  void install() override;

  void undo();

  void redo();

  void set_command_capacity(usize capacity);

 private:
  void _on_set_command_capacity(const SetCommandCapacityEvent& event);

  [[nodiscard]] auto _get_active_command_stack() -> CommandStack*;
};

}  // namespace tactile
