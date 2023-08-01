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
#include "common/type/ptr.hpp"
#include "core/containers/deque.hpp"
#include "core/containers/string.hpp"
#include "core/functional/maybe.hpp"
#include "core/prelude.hpp"

namespace tactile {

/**
 * Manages a history of commands.
 *
 * \details Command stacks are laid out according to the following diagram. The diagram
 *          showcases one possible scenario.
 *
 * <ul>
 *   <li> New commands are always added to the back of the sequence.
 *   <li> The "clean" index simply refers to a specific state in the command sequence.
 *        Its sole purpose is to track whether any observable changes has been made since that state.
 *        This is particularly useful to track changes to documents.
 *   <li> The current index points to the command that was last executed. If a command is
 *        reverted (using the `undo` function), then this index is moved to the left but
 *        the command stack is otherwise untouched. Similarly, if a command is repeated
 *        after being reverted, this index is incremented and shifted to the right.
 * </ul>
 *
 * \code
 *                          clean
 *              front         |  back
 *              __v___________v___v__
 * <-- older -- | 0 | 1 | 2 | 3 | 4 | -- newer -->
 *              ----------^----------
 *                     current
 * \endcode
 */
class CommandStack final {
 public:
  TACTILE_DELETE_COPY(CommandStack);
  TACTILE_DEFAULT_MOVE(CommandStack);

  explicit CommandStack(usize capacity = 64);

  /// Clears the command stack of all commands.
  void clear();

  /**
   * Marks the current command stack state as "clean".
   *
   * \details The notion of a clean command stack is used to prevent unnecessary saving of
   * files, etc. For example, when a document is saved, it should be marked as clean. In
   * other words, a document with a clean command stack doesn't need to be saved again.
   */
  void mark_as_clean();

  /// Resets any current clean state.
  void reset_clean();

  /// Undoes the last command.
  void undo();

  /// Redoes the last command.
  void redo();

  /**
   * Pushes a command to command stack, but does not execute it.
   *
   * \tparam T a command type.
   * \param args arguments that will be forwarded to a command constructor.
   */
  template <std::derived_from<Command> T, typename... Args>
  void store(Args&&... args)
  {
    _store(std::make_unique<T>(std::forward<Args>(args)...));
  }

  /**
   * Pushes a command to the command stack and executes it.
   *
   * \note Any redoable commands will be removed when this function is called.
   *
   * \tparam T a command type.
   * \param args arguments that will be forwarded to a command constructor.
   */
  template <std::derived_from<Command> T, typename... Args>
  void push(Args&&... args)
  {
    if (size() == capacity()) {
      _remove_oldest_command();
    }

    _remove_commands_after_current_index();

    // Don't allocate the commands on the heap until we know for sure they should be kept.
    // Note, a lot of commands can get created and discarded in a short period of time.
    T cmd {std::forward<Args>(args)...};
    cmd.redo();

    // If the stack is empty, we simply push the command on the stack. However, if there
    // are commands on the stack, we try to merge the command into the top of the stack.
    // If that succeeds, we discard the temporary command. Otherwise, just add the command
    // to the stack as per usual.
    if (mStack.empty() || !mStack.back()->merge_with(&cmd)) {
      mCurrentIndex = _get_next_index();
      mStack.push_back(std::make_unique<T>(std::move(cmd)));
    }
    else {
      mCleanIndex.reset();
    }
  }

  /**
   * Sets the maximum amount of commands that the stack can hold.
   *
   * If the supplied capacity is smaller than the current capacity, then commands are
   * removed so that the size doesn't exceed the new capacity.
   *
   * \param capacity the maximum amount of commands.
   */
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
  [[nodiscard]] auto index() const noexcept -> Maybe<usize> { return mCurrentIndex; }

  /// Returns the clean index, if there is one.
  [[nodiscard]] auto clean_index() const noexcept -> Maybe<usize> { return mCleanIndex; }

  /// Returns the maximum amount of commands that the stack can hold.
  [[nodiscard]] auto capacity() const noexcept -> usize { return mCapacity; }

 private:
  Deque<Unique<Command>> mStack;
  Maybe<usize> mCurrentIndex;
  Maybe<usize> mCleanIndex;
  usize mCapacity;

  void _store(Unique<Command> cmd);

  void _remove_oldest_command();
  void _remove_commands_after_current_index();

  [[nodiscard]] auto _get_next_index() const -> usize;
};

}  // namespace tactile
