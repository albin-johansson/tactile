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

#include <concepts>  // derived_from
#include <deque>     // deque
#include <string>    // string
#include <utility>   // move

#include "core/common/ints.hpp"
#include "core/common/maybe.hpp"
#include "core/common/memory.hpp"
#include "editor/commands/command.hpp"

namespace tactile {

/**
 * \brief Manages a history of commands.
 *
 * \ingroup commands
 */
class CommandStack final
{
 public:
  CommandStack();

  /**
   * \brief Clears the command stack of all commands.
   */
  void clear();

  /**
   * \brief Marks the current command stack state as "clean".
   *
   * \details The notion of a clean command stack is used to prevent unnecessary
   * saving of files, etc. For example, when a document is saved, it should be
   * marked as clean.
   */
  void mark_as_clean();

  /**
   * \brief Resets any current clean state.
   */
  void reset_clean();

  /**
   * \brief Undoes the current command.
   *
   * \pre There must be an undoable command.
   */
  void undo();

  /**
   * \brief Redoes the current command.
   *
   * \pre There must be a redoable command.
   */
  void redo();

  template <std::derived_from<ACommand> T, typename... Args>
  void push_without_redo(Args&&... args)
  {
    if (size() == capacity()) {
      remove_oldest_command();
    }

    remove_commands_after_current_index();
    auto cmd = std::make_unique<T>(std::forward<Args>(args)...);
    mIndex = mIndex ? *mIndex + 1 : 0;
    mStack.push_back(std::move(cmd));
  }

  /**
   * \brief Pushes a command to the command stack and executes it.
   *
   * \details Any redoable commands will be removed when this function is
   * called.
   *
   * \tparam T the command type.
   *
   * \tparam Args the types of the arguments that will be forwarded.
   *
   * \param args the arguments that will be forwarded to the command
   * constructor.
   */
  template <std::derived_from<ACommand> T, typename... Args>
  void push(Args&&... args)
  {
    if (size() == capacity()) {
      remove_oldest_command();
    }

    remove_commands_after_current_index();

    T cmd{std::forward<Args>(args)...};
    cmd.redo();

    /* If the stack is empty, we simply push the command to the stack. However,
       if there are commands on the stack, we try to merge the command into the
       top of the stack and if that succeeds we discard the command. */
    if (mStack.empty() || !mStack.back()->merge_with(cmd)) {
      mIndex = mIndex ? *mIndex + 1 : 0;
      mStack.push_back(std::make_unique<T>(std::move(cmd)));
    }
    else if (!mStack.empty()) {
      /* If we get here, the command was merged into the top-most command */
      mCleanIndex.reset();
    }
  }

  /**
   * \brief Sets the maximum amount of commands that the stack can hold.
   *
   * \details If the supplied capacity is smaller than the current capacity, then
   * commands are removed so that the size doesn't exceed the new capacity.
   *
   * \param capacity the maximum amount of commands.
   */
  void set_capacity(usize capacity);

  /**
   * \brief Indicates whether or not the current command stack state is clean.
   *
   * \return `true` if the current state is clean; `false` otherwise.
   */
  [[nodiscard]] auto is_clean() const -> bool;

  /**
   * \brief Indicates whether or not the current command is undoable.
   *
   * \return `true` if undo is available; `false` otherwise.
   */
  [[nodiscard]] auto can_undo() const -> bool;

  /**
   * \brief Indicates whether or not the current command is redoable.
   *
   * \return `true` if redo is available; `false` otherwise.
   */
  [[nodiscard]] auto can_redo() const -> bool;

  /**
   * \brief Returns the text associated with the current undoable command.
   *
   * \pre There must be a undoable command.
   *
   * \return the undoable command text.
   */
  [[nodiscard]] auto get_undo_text() const -> const std::string&;

  /**
   * \brief Returns the text associated with the current redoable command.
   *
   * \pre There must be a redoable command.
   *
   * \return the redoable command text.
   */
  [[nodiscard]] auto get_redo_text() const -> const std::string&;

  /**
   * \brief Returns the number of commands on the stack.
   *
   * \return the amount of stack commands.
   */
  [[nodiscard]] auto size() const noexcept -> usize { return mStack.size(); }

  /**
   * \brief Returns the current command index.
   *
   * \return the index of the current command;
   *         an empty optional is returned is there is no such command.
   */
  [[nodiscard]] auto index() const noexcept -> Maybe<usize> { return mIndex; }

  /**
   * \brief Returns the clean index, if there is one.
   *
   * \return the index of the command considered to represent the clean state;
   *         an empty optional is returned is there is no clean index.
   */
  [[nodiscard]] auto clean_index() const noexcept -> Maybe<usize> { return mCleanIndex; }

  /**
   * \brief Returns the maximum amount of commands that the stack can hold.
   *
   * \return the stack capacity.
   */
  [[nodiscard]] auto capacity() const noexcept -> usize { return mCapacity; }

 private:
  std::deque<Unique<ACommand>> mStack;
  Maybe<usize> mIndex;
  Maybe<usize> mCleanIndex;
  usize mCapacity;

  void remove_oldest_command();

  void remove_commands_after_current_index();
};

}  // namespace tactile
