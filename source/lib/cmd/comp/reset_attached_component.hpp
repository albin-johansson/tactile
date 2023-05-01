/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "common/type/ecs.hpp"
#include "common/type/maybe.hpp"
#include "common/type/string_map.hpp"
#include "core/attribute.hpp"
#include "core/component.hpp"
#include "cmd/command.hpp"

namespace tactile::cmd {

/// A command for resetting the attributes of an attached component.
class ResetAttachedComponent final : public Command {
 public:
  ResetAttachedComponent(Entity component_entity);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Entity mComponentEntity {kNullEntity};
  Maybe<StringMap<Attribute>> mPreviousValues;
};

}  // namespace tactile::cmd