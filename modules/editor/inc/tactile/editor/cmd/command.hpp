// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/prelude.hpp"
#include "tactile/editor/api.hpp"

namespace tactile::editor {

class TACTILE_EDITOR_API ICommand {
 public:
  TACTILE_INTERFACE_CLASS(ICommand);

  /** Reverts the effects of the command. */
  virtual void undo() = 0;

  /** Applies the effects of the command. */
  virtual void redo() = 0;

  [[nodiscard]] virtual auto get_id() const -> uint32 = 0;

  /**
   * \brief Tries to merge with another command.
   *
   * \details This function allows commands of the same type that target the same objects
   *          to be combined in order to drastically reduce the amount of near identical
   *          consecutive commands. This is especially noticeable for high-frequency
   *          commands, such as color picker updates. The provided command is discarded
   *          if a merge is performed.
   *
   * \param cmd a command to try to merge into this command, never null.
   *
   * \return true if the command was merged; false otherwise.
   */
  [[nodiscard]] virtual auto merge_with([[maybe_unused]] const ICommand* cmd) -> bool
  {
    return false;
  }
};

}  // namespace tactile::editor
