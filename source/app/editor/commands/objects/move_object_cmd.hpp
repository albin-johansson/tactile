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

#pragma once

#include <entt/entt.hpp>  // registry

#include "editor/commands/command_id.hpp"
#include "object_cmd.hpp"
#include "tactile.hpp"

namespace tactile {

/// \addtogroup commands
/// \{

class move_object_cmd final : public object_cmd
{
 public:
  move_object_cmd(registry_ref registry,
                  object_id id,
                  float oldX,
                  float oldY,
                  float newX,
                  float newY);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::move_object;
  }

 private:
  float mOldX;
  float mOldY;
  float mNewX;
  float mNewY;
};

/// \} End of group commands

}  // namespace tactile
