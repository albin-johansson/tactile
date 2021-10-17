#pragma once

#include <string>  // string

namespace Tactile {

/// \addtogroup commands
/// \{

class ACommand
{
 public:
  ACommand() = default;

  /**
   * \brief Creates a command.
   *
   * \param text a short human-readable command name.
   */
  explicit ACommand(std::string text);

  virtual ~ACommand() = default;

  /**
   * \brief Reverts the effect of the command.
   *
   * \details This function should revert the effects of having previously
   * called `Redo()`.
   */
  virtual void Undo() = 0;

  /**
   * \brief Applies the effects of the command.
   *
   * \details This function is called whenever a command is pushed onto the
   * command stack.
   *
   * \see `CommandStack::Push`
   */
  virtual void Redo() = 0;

  /**
   * \brief Returns a identifier unique for the command type.
   *
   * \details This function is mainly designed to be used when overriding the
   * `MergeWith()` function, where it can be used to efficiently test if two
   * commands are of the same type, since the parameter type is `ACommand`.
   *
   * \return an identifier unique to the command class.
   *
   * \see `CommandId`
   */
  [[nodiscard]] virtual auto GetId() const -> int = 0;

  /**
   * \brief Attempts to merge the supplied command into *this* command.
   *
   * \details Override this function in order to enable reducing the amount of
   * commands on the command stack when it makes sense to chain a group of
   * commands of the same type together. For example, this is used to combine
   * consecutive "Add Row" commands.
   *
   * \details Use the `GetId()` function to efficiently filter out commands of
   * differing types.
   *
   * \note This function is only called on the command on top of the command
   * stack.
   *
   * \param cmd the command that will potentially be merged into *this* command.
   *
   * \return `true` if the supplied command was merged into *this* command;
   * `false` otherwise.
   */
  [[nodiscard]] virtual auto MergeWith([[maybe_unused]] const ACommand& cmd) -> bool
  {
    return false;
  }

  /**
   * \brief Returns a short human-readable string that describes the command.
   *
   * \return the name of the command.
   */
  [[nodiscard]] auto GetText() const -> const std::string&
  {
    return mText;
  }

 private:
  std::string mText;
};

/// \} End of group commands

}  // namespace Tactile
