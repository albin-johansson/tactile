#pragma once

#include <concepts>  // derived_from
#include <deque>     // deque
#include <memory>    // unique_ptr
#include <string>    // string
#include <utility>   // move

#include "command.hpp"
#include <tactile-base/tactile_std.hpp>

namespace Tactile {

/// \addtogroup commands
/// \{

class CommandStack final
{
 public:
  CommandStack();

  /// Clears the command stack of all commands.
  void Clear();

  /**
   * \brief Marks the current command stack state as "clean".
   *
   * \details The notion of a clean command stack is used to prevent unnecessary
   * saving of files, etc. For example, when a document is saved, it should be
   * marked as clean.
   */
  void MarkAsClean();

  /**
   * \brief Resets any current clean state.
   */
  void ResetClean();

  /**
   * \brief Undoes the current command.
   *
   * \pre There must be an undoable command.
   */
  void Undo();

  /**
   * \brief Redoes the current command.
   *
   * \pre There must be a redoable command.
   */
  void Redo();

  template <std::derived_from<ACommand> T, typename... Args>
  void PushWithoutRedo(Args&&... args)
  {
    if (GetSize() == GetCapacity()) {
      RemoveOldestCommand();
    }

    RemoveCommandsAfterCurrentIndex();
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
  void Push(Args&&... args)
  {
    if (GetSize() == GetCapacity()) {
      RemoveOldestCommand();
    }

    RemoveCommandsAfterCurrentIndex();

    auto cmd = std::make_unique<T>(std::forward<Args>(args)...);
    cmd->Redo();

    /* If the stack is empty, we simply push the command to the stack. However,
       if there are commands on the stack, we try to merge the command into the
       top of the stack and if that succeeds we discard the command. */
    if (mStack.empty() || !mStack.back()->MergeWith(*cmd)) {
      mIndex = mIndex ? *mIndex + 1 : 0;
      mStack.push_back(std::move(cmd));
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
  void SetCapacity(usize capacity);

  /**
   * \brief Indicates whether or not the current command stack state is clean.
   *
   * \return `true` if the current state is clean; `false` otherwise.
   */
  [[nodiscard]] auto IsClean() const -> bool;

  /**
   * \brief Indicates whether or not the current command is undoable.
   *
   * \return `true` if undo is available; `false` otherwise.
   */
  [[nodiscard]] auto CanUndo() const -> bool;

  /**
   * \brief Indicates whether or not the current command is redoable.
   *
   * \return `true` if redo is available; `false` otherwise.
   */
  [[nodiscard]] auto CanRedo() const -> bool;

  /**
   * \brief Returns the text associated with the current undoable command.
   *
   * \pre There must be a undoable command.
   *
   * \return the undoable command text.
   */
  [[nodiscard]] auto GetUndoText() const -> const std::string&;

  /**
   * \brief Returns the text associated with the current redoable command.
   *
   * \pre There must be a redoable command.
   *
   * \return the redoable command text.
   */
  [[nodiscard]] auto GetRedoText() const -> const std::string&;

  /// Returns the number of commands in the stack.
  [[nodiscard]] auto GetSize() const noexcept -> usize
  {
    return mStack.size();
  }

  /// Returns the current command index.
  [[nodiscard]] auto GetIndex() const noexcept -> Maybe<usize>
  {
    return mIndex;
  }

  /// Returns the clean index, if there is one.
  [[nodiscard]] auto GetCleanIndex() const noexcept -> Maybe<usize>
  {
    return mCleanIndex;
  }

  /**
   * \brief Returns the maximum amount of commands that the stack can hold.
   *
   * \details The capacity defaults to the value of `Prefs::GetCommandCapacity()`.
   *
   * \return the stack capacity.
   */
  [[nodiscard]] auto GetCapacity() const noexcept -> usize
  {
    return mCapacity;
  }

 private:
  std::deque<std::unique_ptr<ACommand>> mStack;
  Maybe<usize> mIndex{};
  Maybe<usize> mCleanIndex;
  usize mCapacity;

  void RemoveOldestCommand();

  void RemoveCommandsAfterCurrentIndex();
};

/// \} End of group commands

}  // namespace Tactile
