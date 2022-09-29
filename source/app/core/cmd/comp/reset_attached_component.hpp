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

#include "core/cmd/command.hpp"
#include "core/common/maybe.hpp"
#include "core/common/uuid.hpp"
#include "core/comp/component.hpp"
#include "core/type/ptr.hpp"

namespace tactile {
class Context;
class ComponentIndex;
}  // namespace tactile

namespace tactile::cmd {

/// A command for resetting the attributes of an attached component.
class ResetAttachedComponent final : public Command {
 public:
  ResetAttachedComponent(Shared<ComponentIndex> index,
                         Shared<Context> context,
                         const UUID& component_id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> std::string override;

 private:
  Shared<ComponentIndex> mIndex;
  Shared<Context> mContext;
  UUID mComponentId {};
  Maybe<Component> mComponent;
};

}  // namespace tactile::cmd