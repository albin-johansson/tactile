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
#include "core/ecs/entity.hpp"
#include "core/ecs/registry.hpp"
#include "tactile/core/layers/object_type.hpp"
#include "tactile/core/math/vector.hpp"

namespace tactile::cmd {

class AddObject final : public Command {
 public:
  AddObject(Registry* registry,
            Entity map_entity,
            Entity object_layer_entity,
            ObjectType type,
            Float2 position,
            Float2 size = {0, 0});

  void undo() override;

  void redo() override;

  void dispose() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Registry* mRegistry;
  Entity mMapEntity;
  Entity mObjectLayerEntity;
  ObjectType mObjectType;
  Float2 mPosition;
  Float2 mSize;
  Entity mObjectEntity {kNullEntity};
  bool mDidAddObject : 1 {false};
};

}  // namespace tactile::cmd
