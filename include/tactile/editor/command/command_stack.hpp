// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include <concepts>
#include <utility>

#include "tactile/core/container/deque.hpp"
#include "tactile/core/container/option.hpp"
#include "tactile/core/container/smart_ptr.hpp"
#include "tactile/core/primitives.hpp"
#include "tactile/editor/command/command.hpp"

namespace tactile::editor {

/// Manages a fixed-size history of editor commands.
class CommandStack final
{
 public:
  /// Creates a command stack with the given capacity.
  explicit CommandStack(isize capacity = 100);

  /// Marks the current state of the command stack as clean.
  void mark_as_clean();

  /// Reverts the cursor command.
  void undo();

  /// Executes the command after the cursor.
  void redo();

  /// Stores a command in the stack without executing it.
  void store(Unique<ICommand> command);

  /// Indicates whether the command stack is clean.
  auto is_clean() const -> bool;

  /// Indicates whether undo can be called.
  auto can_undo() const -> bool;

  /// Indicates whether redo can be called.
  auto can_redo() const -> bool;

  /// Returns the number of commands in the stack.
  auto size() const -> isize;

  /// Returns the maximum number of commands in the stack.
  auto capacity() const -> isize;

  /// Returns a pointer to the cursor command, or null if there is none.
  auto get_cursor_command() const -> const ICommand*;

  /// Executes a command and adds it to the stack.
  template <std::derived_from<ICommand> T, typename... Args>
    requires std::constructible_from<T, Args...>
  void execute(Args&&... args)
  {
    if (size() == capacity()) {
      remove_oldest_command();
    }

    remove_commands_after_cursor();

    T command {std::forward<Args>(args)...};
    command.redo();

    if (m_commands.empty() || !m_commands.back()->merge(command)) {
      // Only allocate the command on the heap if we know it's really needed.
      m_commands.push_back(make_unique<T>(std::move(command)));
      ++m_cursor_command_index;
    }
    else {
      // We get here if the command got merged, which is an observable change.
      reset_clean_state();
    }
  }

 private:
  /// The maximum number of commands in the stack.
  isize m_capacity {};

  /// The stored commands (the oldest command is at index 0).
  Deque<Unique<ICommand>> m_commands {};

  /// The index of the command representing the current observable state.
  isize m_cursor_command_index {-1};

  /// The index of the command where the stack is considered "clean" (i.e., unchanged).
  isize m_clean_command_index {-1};

  /// Indicates whether there is a clean state.
  bool m_has_clean_state {false};

  void remove_oldest_command();

  void remove_commands_after_cursor();

  void reset_clean_state();

  auto get_undo_command_index() const -> Option<usize>;

  auto get_redo_command_index() const -> Option<usize>;
};

}  // namespace tactile::editor
