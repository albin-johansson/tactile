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

#include "core/commands/command.hpp"
#include "core/commands/command_id.hpp"
#include "core/common/identifiers.hpp"
#include "core/common/maybe.hpp"
#include "core/systems/layers/object_system.hpp"

namespace tactile {

/// \addtogroup commands
/// \{

class RectangleToolCmd final : public ACommand
{
 public:
  RectangleToolCmd(RegistryRef registry, float x, float y, float width, float height);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return CommandId::add_rectangle;
  }

 private:
  RegistryRef mRegistry;
  LayerID mLayerId{};
  float mX{};
  float mY{};
  float mWidth{};
  float mHeight{};
  Maybe<ObjectID> mObjectId;
  Maybe<sys::RemoveObjectResult> mSnapshot;
};

/// \} End of group commands

}  // namespace tactile