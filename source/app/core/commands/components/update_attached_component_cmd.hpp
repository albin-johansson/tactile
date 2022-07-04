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

#include "core/attribute.hpp"
#include "core/commands/command.hpp"
#include "core/common/maybe.hpp"
#include "core/common/memory.hpp"
#include "core/common/uuid.hpp"
#include "core/fwd.hpp"

namespace tactile {

/// A command for updating the attribute of a component attached to a context.
class UpdateAttachedComponentCmd final : public ICommand
{
 public:
  UpdateAttachedComponentCmd(Shared<core::IContext> context,
                             const UUID&            componentId,
                             std::string            attribute,
                             Attribute              value);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto merge_with(const ICommand& cmd) -> bool override;

  [[nodiscard]] auto get_name() const -> const char* override;

  [[nodiscard]] auto id() const noexcept -> CommandId override
  {
    return CommandId::UpdateAttachedComponent;
  }

 private:
  Shared<core::IContext> mContext;
  UUID                   mComponentId{};
  std::string            mAttributeName;
  Attribute              mUpdatedValue;
  Maybe<Attribute>       mPreviousValue;
};

}  // namespace tactile