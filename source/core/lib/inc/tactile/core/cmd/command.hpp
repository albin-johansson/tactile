// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Represents editor actions that can be repeatedly executed and reverted.
 */
class ICommand
{
 public:
  TACTILE_INTERFACE_CLASS(ICommand);

  /**
   * Reverts the effects of the command.
   */
  virtual void undo() = 0;

  /**
   * Applies the effects of the command.
   */
  virtual void redo() = 0;

  /**
   * Frees any unused resources held by the command.
   *
   * \details
   * This function is called just before commands are removed from a command stack and
   * destroyed. The intention is that this function should be used to avoid unnecessary
   * resource leaks, especially entity identifiers.
   *
   * \warning
   * The command cannot be used after this function is called.
   */
  virtual void dispose()
  {}

  /**
   * Tries to merge with another command.
   *
   * \details
   * This function allows commands of the same type that target the same objects to be combined
   * to drastically reduce the amount of near identical consecutive commands. This is
   * especially noticeable for high-frequency commands, such as color picker updates. The
   * provided command is discarded if a merge is performed.
   *
   * \param cmd A command to try to merge into this command, never null.
   *
   * \return
   * True if the command was successfully merged; false otherwise.
   */
  [[nodiscard]]
  virtual auto merge_with([[maybe_unused]] const ICommand* cmd) -> bool
  {
    return false;
  }
};

}  // namespace tactile
