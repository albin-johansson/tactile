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

#include "core/common/identifiers.hpp"
#include "core/common/maybe.hpp"
#include "editor/commands/command_id.hpp"
#include "editor/commands/objects/object_command.hpp"

namespace tactile {

class SetObjectVisibilityCmd final : public AObjectCommand
{
 public:
  SetObjectVisibilityCmd(RegistryRef registry, ObjectID id, bool visible);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return CommandId::set_object_visibility;
  }

 private:
  bool mVisible;
  Maybe<bool> mPreviousVisibility;
};

}  // namespace tactile
