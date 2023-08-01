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

#include "core/attributes/attribute.hpp"
#include "core/ecs/entity.hpp"
#include "core/containers/string.hpp"
#include "io/ir/map/map_ir.hpp"
#include "model/registry.hpp"

namespace tactile {

class ComponentBuilder final {
 public:
  using Self = ComponentBuilder;

  explicit ComponentBuilder(Registry& registry, Entity component_set_entity);

  auto with_name(String name) -> Self&;

  auto with_attribute(String name, Attribute value) -> Self&;

  auto build() -> Entity;

 private:
  Registry* mRegistry;
  Entity mComponentSetEntity;
  String mComponentName;
  AttributeMap mAttributes;  // TODO move definition of this alias
};

}  // namespace tactile
