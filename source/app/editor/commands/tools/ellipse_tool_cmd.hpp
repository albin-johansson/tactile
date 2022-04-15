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

#include "core/systems/object_system.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile.hpp"

namespace tactile {

/// \addtogroup commands
/// \{

class EllipseToolCmd final : public ACommand
{
 public:
  EllipseToolCmd(RegistryRef registry, float x, float y, float width, float height);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return CommandId::add_ellipse;
  }

 private:
  RegistryRef mRegistry;
  LayerID mLayerId{};
  float mX{};
  float mY{};
  float mWidth{};  /* X-axis diameter */
  float mHeight{}; /* Y-axis diameter */
  Maybe<ObjectID> mObjectId;
  Maybe<sys::RemoveObjectResult> mSnapshot;
};

/// \} End of group commands

}  // namespace tactile
