// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>  // derived_from
#include <memory>    // make_unique
#include <utility>   // move, forward

#include "tactile/base/container/deque.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/cmd/command.hpp"

namespace tactile {

/**
 * Manages a history of commands.
 *
 * \details
 * Command stacks are laid out according to the following diagram.
 *
 * \code
 *                       clean index
 *              front         |  back
 *              __v___________v___v__
 * <-- older -- | 0 | 1 | 2 | 3 | 4 | -- newer -->
 *              ----------^----------
 *                 current index
 * \endcode
 *
 * - New commands are always added to the back of the sequence. \n
 * - The "clean" index simply refers to a specific state in the command
 *   sequence. Its sole purpose is to track whether any observable changes has
 *   been made since that state. This is particularly useful to track changes to
 *   documents. \n
 * - The current index points to the command that was last executed. If a
 *   command is reverted (using the `undo` function), then this index is moved
 *   to the left but the command stack is otherwise untouched. Similarly, if a
 *   command is repeated after being reverted, this index is incremented and
 *   shifted to the right.
 */
class CommandStack final
{
 public:
  TACTILE_DELETE_COPY(CommandStack);
  TACTILE_DEFAULT_MOVE(CommandStack);

  /**
   * Creates an empty command stack with the specified capacity.
   *
   * \param capacity The maximum amount of stored commands.
   */
  explicit CommandStack(usize capacity);

  ~CommandStack() noexcept = default;

  /**
   * Marks the current command stack state as "clean".
   *
   * \details
   * The notion of a clean command stack is used to prevent unnecessary
   * saving of files. This function should be called whenever a document is
   * saved.
   */
  void mark_as_clean();

  /**
   * Resets the current clean state index.
   */
  void reset_clean_index();

  /**
   * Reverts the most recent command.
   */
  void undo();

  /**
   * Executes the most recently reverted command.
   */
  void redo();

  /**
   * Constructs a command and places it on the stack.
   *
   * \details
   * Use this function for commands that do not need to be executed when added.
   * For example, an interactive tool command will likely not need to be
   * executed when pushed since the effects would have already been applied when
   * used.
   *
   * \tparam T A command type.
   *
   * \param args Arguments forwarded to a command constructor.
   */
  template <std::derived_from<ICommand> T, typename... Args>
  void store(Args&&... args)
  {
    _store(make_unique<T>(std::forward<Args>(args)...));
  }

  /**
   * Constructs a command, executes it, and places it on the stack (or merges
   * it).
   *
   * \tparam T A command type.
   *
   * \param args Arguments forwarded to a command constructor.
   */
  template <std::derived_from<ICommand> T, typename... Args>
  void push(Args&&... args)
  {
    if (size() == capacity()) {
      _remove_oldest_command();
    }

    _remove_commands_after_current_index();

    // Minor optimization: we don't allocate the commands on the heap until we
    // know for sure they should be kept (a lot of commands can get created in
    // a short duration).
    T cmd {std::forward<Args>(args)...};
    cmd.redo();

    // If the stack is empty, we simply push the command on the stack. However,
    // if there are commands on the stack, we try to merge the command into the
    // top of the stack. If that succeeds, we discard the temporary command.
    // Otherwise, just add the command to the stack as per usual.
    if (mCommands.empty() || !mCommands.back()->merge_with(&cmd)) {
      mCommands.push_back(std::make_unique<T>(std::move(cmd)));
      _increase_current_index();
    }
    else {
      mCleanIndex.reset();
    }
  }

  /**
   * Sets the maximum amount of commands that the stack can hold.
   *
   * \details
   * If the supplied capacity is smaller than the current capacity, then
   * commands are removed so that the size doesn't exceed the new capacity.
   *
   * \param capacity The maximum amount of stored commands.
   */
  void set_capacity(usize capacity);

  /**
   * Indicates whether the current command stack state is clean.
   */
  [[nodiscard]]
  auto is_clean() const -> bool;

  /**
   * Indicates whether there is an undoable command.
   */
  [[nodiscard]]
  auto can_undo() const -> bool;

  /**
   * Indicates whether there is a redoable command.
   */
  [[nodiscard]]
  auto can_redo() const -> bool;

  /**
   * Returns the number of commands on the stack.
   */
  [[nodiscard]]
  auto size() const -> usize;

  /**
   * Returns the maximum amount of commands that the stack can hold.
   */
  [[nodiscard]]
  auto capacity() const -> usize;

  /**
   * Returns the current command index, if there is one.
   */
  [[nodiscard]]
  auto index() const -> Optional<usize>;

  /**
   * Returns the clean index, if there is one.
   */
  [[nodiscard]]
  auto clean_index() const -> Optional<usize>;

 private:
  Deque<Unique<ICommand>> mCommands {};
  Optional<usize> mCurrentIndex {};
  Optional<usize> mCleanIndex {};
  usize mCapacity {};

  // Pushes a command onto the stack, but does not execute it.
  void _store(Unique<ICommand> cmd);

  // Removes the oldest command from the stack, i.e., the one at the bottom.
  void _remove_oldest_command();

  // Removes all commands to the right of the current index (newer ones).
  void _remove_commands_after_current_index();

  void _reset_or_decrease_clean_index();

  // Shifts the current index to the left (to an older command).
  void _reset_or_decrease_current_index();

  // Shifts the current index to the right (to a newer command).
  void _increase_current_index();

  [[nodiscard]]
  auto _get_next_command_index() const -> usize;
};

}  // namespace tactile
