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

#include "core/commands/command_id.hpp"

namespace tactile {

class ICommand
{
 public:
  virtual ~ICommand() = default;

  /**
   * Reverts the effect of the command.
   *
   * This function should revert the effects of having previously called `redo()`.
   */
  virtual void undo() = 0;

  /**
   * Applies the effects of the command.
   *
   * This function is called whenever a command is pushed onto the command stack.
   */
  virtual void redo() = 0;

  [[nodiscard, deprecated]] virtual auto id() const -> CommandId = 0;

  /// Returns a short human-readable string that describes the command.
  [[nodiscard]] virtual auto get_name() const -> const char* = 0;

  /**
   * Attempts to merge the supplied command into this command.
   *
   * Override this function in order to enable reducing the amount of
   * commands on the command stack when it makes sense to chain a group of
   * commands of the same type together. For example, this is used to combine
   * consecutive 'Add row' commands. By default, this function just returns false.
   *
   * Note, this function is only called on the command on top of the command stack.
   *
   * \param cmd the command that will potentially be merged into *this* command, never
   * null.
   *
   * \return `true` if the supplied command was merged into *this* command;
   *         `false` otherwise.
   */
  [[nodiscard]] virtual auto merge_with([[maybe_unused]] const ICommand* cmd) -> bool
  {
    return false;
  }
};

}  // namespace tactile
