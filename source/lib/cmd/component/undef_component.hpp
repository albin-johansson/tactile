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

#include "cmd/command.hpp"
#include "common/attribute.hpp"
#include "common/type/ecs.hpp"
#include "common/type/hash_map.hpp"
#include "common/type/maybe.hpp"
#include "common/type/string.hpp"
#include "common/type/string_map.hpp"
#include "model/model.hpp"

namespace tactile::cmd {

/// A command for removing a component definition.
class UndefComponent final : public Command {
 public:
  UndefComponent(Model* model, Entity component_set_entity, String component_name);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Model* mModel;
  Entity mComponentSetEntity;
  String mComponentName;
  Maybe<StringMap<Attribute>> mPrevDefinitionValues;
  HashMap<Entity, StringMap<Attribute>> mRemovedComponentValues;
};

}  // namespace tactile::cmd