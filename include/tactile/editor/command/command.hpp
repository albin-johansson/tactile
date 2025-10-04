// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include "tactile/core/macros.hpp"

namespace tactile::editor {

/// Interface for editor actions that can be repeatedly executed and reverted.
class ICommand
{
 public:
  TACTILE_INTERFACE(ICommand);

  /// Executes the command.
  virtual void redo() = 0;

  /// Reverts the effects of the redo function.
  virtual void undo() = 0;

  /// Attempts to merge another command into this command, combining the effects.
  virtual auto merge([[maybe_unused]] const ICommand& other) -> bool
  {
    return false;
  }
};

}  // namespace tactile::editor
