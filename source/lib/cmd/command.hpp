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

#include "common/type/string.hpp"

namespace tactile {

/**
 * Abstract base class for all command implementations.
 *
 * \details This class is the basis of a typical command pattern architecture. All
 *          user actions that should be undoable are encapsulated in classes inheriting
 *          from this class. Command classes must provide `redo()`, `undo()`, and
 *          `get_name()` functions. Additionally, command merging is supported through the
 *          `merge_with()` function, which can be optionally overridden by commands.
 *          Command merging enables repeated commands of a single type (targeting the same
 *          context) to be merged into a single command, so that a single subsequent
 *          undo-call may revert the effects of numerous commands.
 *
 * \note A general aim is to have command implementations be as lean as possible, calling
 *       into other (non-command-aware) core APIs to simplify testing.
 */
class Command {
 public:
  virtual ~Command() = default;

  /// Reverts the effects of calling `redo`.
  virtual void undo() = 0;

  /// Executes the command.
  virtual void redo() = 0;

  /**
   * Disposes any potential unused resources associated with the command.
   *
   * \details This function is called when the command is about to be removed from a
   *          command stack. This can be used to free resources that would otherwise go
   *          to waste when the command instance itself is disposed. However, it is
   *          crucial to only dispose associated resources guaranteed to only be
   *          referenced by the command instance itself.
   */
  virtual void dispose() {}

  /**
   * Returns a short, human-readable, string that describes the command.
   *
   * \return a string in the current display language.
   */
  [[nodiscard]] virtual auto get_name() const -> String = 0;

  /**
   * Tries to combine a command with this command.
   *
   * \details This function can be overriden to enable a reduction in the amount of
   *          consecutive commands on the command stack of the same type. By default,
   *          this function just returns false.
   *
   * \param cmd a command to try to merged into this command.
   *
   * \return true if the supplied command was merged into this command; false otherwise.
   */
  [[nodiscard]] virtual auto merge_with([[maybe_unused]] const Command* cmd) -> bool
  {
    return false;
  }
};

}  // namespace tactile
