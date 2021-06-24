#pragma once

#include <concepts>  // derived_from
#include <deque>     // deque
#include <string>    // string
#include <utility>   // move

#include "aliases/ints.hpp"
#include "aliases/maybe.hpp"
#include "aliases/unique.hpp"
#include "command.hpp"

namespace tactile {

inline constexpr usize command_limit = 100;

class CommandStack final
{
 public:
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
    // TODO check limit

    RemoveCommandsAfterCurrentIndex();
    mIndex = mIndex ? *mIndex + 1 : 0;

    auto cmd = std::make_unique<T>(std::forward<Args>(args)...);
    cmd->Redo();

    mStack.push_back(std::move(cmd));
  }

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

 private:
  std::deque<Unique<ACommand>> mStack;
  Maybe<usize> mIndex{};
  Maybe<usize> mCleanIndex;

  void RemoveCommandsAfterCurrentIndex();
};

}  // namespace tactile
