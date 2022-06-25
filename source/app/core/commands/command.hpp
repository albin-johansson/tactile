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

#include <string>  // string

#include <entt/entity/registry.hpp>

#include "core/commands/command_id.hpp"
#include "core/common/functional.hpp"
#include "core/common/macros.hpp"

namespace tactile {

/// \addtogroup commands
/// \{

using RegistryRef = Ref<entt::registry>;

/// The abstract base class of all command implementations.
class ACommand
{
 public:
  TACTILE_DELETE_COPY(ACommand);
  TACTILE_DEFAULT_MOVE(ACommand);

  /**
   * Creates a command.
   *
   * \param text a short human-readable command name.
   */
  explicit ACommand(std::string text);

  virtual ~ACommand() = default;

  /**
   * Reverts the effect of the command.
   *
   * This function should revert the effects of having previously called `redo()`.
   */
  virtual void undo() = 0;

  /**
   * Applies the effects of the command.
   *
   * This function is called whenever a command is pushed onto the command stack.
   */
  virtual void redo() = 0;

  /**
   * Returns a identifier unique for the command type.
   *
   * This function is mainly designed to be used when overriding the
   * `merge_with()` function, where it can be used to efficiently test if two
   * commands are of the same type, since the parameter type is `ACommand`.
   *
   * \return an identifier unique for the command class.
   */
  [[nodiscard]] virtual auto id() const -> CommandId = 0;

  /**
   * Attempts to merge the supplied command into this command.
   *
   * Override this function in order to enable reducing the amount of
   * commands on the command stack when it makes sense to chain a group of
   * commands of the same type together. For example, this is used to combine
   * consecutive "Add Row" commands.
   *
   * Use the `id()` function to efficiently filter out commands of
   * differing types.
   *
   * Note, this function is only called on the command on top of the command
   * stack.
   *
   * \param cmd the command that will potentially be merged into *this* command.
   *
   * \return `true` if the supplied command was merged into *this* command;
   *         `false` otherwise.
   */
  [[nodiscard]] virtual auto merge_with([[maybe_unused]] const ACommand& cmd) -> bool
  {
    return false;
  }

  /// Returns a short human-readable string that describes the command.
  [[nodiscard]] auto text() const -> const std::string& { return mText; }

 private:
  std::string mText;
};

/// \} End of group commands

}  // namespace tactile
