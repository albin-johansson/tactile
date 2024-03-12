// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>  // derived_from
#include <utility>   // move

#include "model/cmd/command.hpp"
#include "tactile/base/container/deque.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

/// Manages a history of commands.
class CommandStack final {
 public:
  CommandStack();

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
