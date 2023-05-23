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

#include <concepts>  // derived_from
#include <utility>   // move, forward

#include "cmd/command.hpp"
#include "common/macros.hpp"
#include "common/primitives.hpp"
#include "common/type/deque.hpp"
#include "common/type/maybe.hpp"
#include "common/type/ptr.hpp"
#include "common/type/string.hpp"

namespace tactile {

/// Manages a history of commands.
class CommandStack final {
 public:
  explicit CommandStack(usize capacity = 64);

  TACTILE_DELETE_COPY(CommandStack);
  TACTILE_DEFAULT_MOVE(CommandStack);

  /// Clears the command stack of all commands.
  void clear();

  /// Marks the current command stack state as "clean".
  ///
  /// The notion of a clean command stack is used to prevent unnecessary saving of files,
  /// etc. For example, when a document is saved, it should be marked as clean.
  void mark_as_clean();

  /// Resets any current clean state.
  void reset_clean();

  /// Undoes the last command.
  void undo();

  /// Redoes the last command.
  void redo();

  template <std::derived_from<Command> T, typename... Args>
  void store(Args&&... args)
  {
    if (size() == capacity()) {
      remove_oldest_command();
    }

    remove_commands_after_current_index();
    auto cmd = std::make_unique<T>(std::forward<Args>(args)...);
    mIndex = mIndex ? *mIndex + 1 : 0;
    mStack.push_back(std::move(cmd));
  }

  /// Pushes a command to the command stack and executes it.
  ///
  /// Any redoable commands will be removed when this function is called.
  ///
  /// \param args the arguments that will be forwarded to a command constructor.
  template <std::derived_from<Command> T, typename... Args>
  void push(Args&&... args)
  {
    if (size() == capacity()) {
      remove_oldest_command();
    }

    remove_commands_after_current_index();

    // Minor optimization: we don't allocate the commands on the heap until we now for
    // sure they should be kept (a lot of commands can get created in a short duration).
    T cmd {std::forward<Args>(args)...};
    cmd.redo();

    // If the stack is empty, we simply push the command on the stack. However, if there
    // are commands on the stack, we try to merge the command into the top of the stack.
    // If that succeeds, we discard the temporary command. Otherwise, just add the command
    // to the stack as per usual.
    if (mStack.empty() || !mStack.back()->merge_with(&cmd)) {
      mIndex = mIndex ? (*mIndex + 1) : 0;
      mStack.push_back(std::make_unique<T>(std::move(cmd)));
    }
    else {
      mCleanIndex.reset();
    }
  }

  /// Sets the maximum amount of commands that the stack can hold.
  ///
  /// If the supplied capacity is smaller than the current capacity, then commands are
  /// removed so that the size doesn't exceed the new capacity.
  ///
  /// \param capacity the maximum amount of commands.
  void set_capacity(usize capacity);

  /// Indicates whether or not the current command stack state is clean.
  [[nodiscard]] auto is_clean() const -> bool;

  /// Indicates whether or not the current command is undoable.
  [[nodiscard]] auto can_undo() const -> bool;

  /// Indicates whether or not the current command is redoable.
  [[nodiscard]] auto can_redo() const -> bool;

  /// Returns the text associated with the current undoable command.
  [[nodiscard]] auto get_undo_text() const -> String;

  /// Returns the text associated with the current redoable command.
  [[nodiscard]] auto get_redo_text() const -> String;

  /// Returns the number of commands on the stack.
  [[nodiscard]] auto size() const noexcept -> usize { return mStack.size(); }

  /// Returns the current command index, if there is one.
  [[nodiscard]] auto index() const noexcept -> Maybe<usize> { return mIndex; }

  /// Returns the clean index, if there is one.
  [[nodiscard]] auto clean_index() const noexcept -> Maybe<usize> { return mCleanIndex; }

  /// Returns the maximum amount of commands that the stack can hold.
  [[nodiscard]] auto capacity() const noexcept -> usize { return mCapacity; }

 private:
  Deque<Unique<Command>> mStack;
  Maybe<usize> mIndex;
  Maybe<usize> mCleanIndex;
  usize mCapacity;

  void remove_oldest_command();

  void remove_commands_after_current_index();
};

}  // namespace tactile
