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

#include "core/type/string.hpp"

namespace tactile {

/// The abstract base class for all command implementations.
///
/// <p>
/// This class is the core of a typical command pattern architecture. Where all user
/// actions that should be undoable are implemented as classed inheriting from the Command
/// class. All command classes must provide `redo()`, `undo()`, and `get_name()`
/// implementations. Additionally, command merging is supported through the `merge_with()`
/// function, which can be optionally overridden by commands. Command merging enables
/// repeated commands of a single type (targeting the same context) to be merged into a
/// single command, so that a single subsequent undo-call may revert the effects of
/// numerous commands.
///
/// <p>
/// A general aim is to get command implementations as lean as possible, calling into
/// other (non-command-aware) core APIs as much as possible, to simplify testing. Avoid
/// storing raw pointers in command instances, unless in the case of parent documents,
/// which cannot be referenced by smart pointers.
class Command {
 public:
  virtual ~Command() = default;

  /// Reverts the effect of the command.
  ///
  /// This function should revert the effects of having previously called `redo()`.
  virtual void undo() = 0;

  /// Applies the effects of the command.
  ///
  /// This function is called whenever a command is pushed onto the command stack.
  virtual void redo() = 0;

  /// Returns a short human-readable string that describes the command.
  /// The string returned by this function is user facing, so make sure it's translated!
  [[nodiscard]] virtual auto get_name() const -> String = 0;

  /// Attempts to combine a command with this command.
  ///
  /// <p>
  /// Override this function in order to enable reducing the amount of
  /// commands on the command stack when it makes sense to chain a group of
  /// commands of the same type together. For example, this is used to combine
  /// consecutive 'Add row' commands. By default, this function just returns false.
  ///
  /// <p>
  /// Note, this function is only called on the command on top of the command stack.
  ///
  /// \param cmd a command that will potentially be merged into this command (never null).
  ///
  /// \return true if the supplied command was merged into this command;
  ///         false otherwise.
  [[nodiscard]] virtual auto merge_with([[maybe_unused]] const Command* cmd) -> bool
  {
    return false;
  }
};

}  // namespace tactile
