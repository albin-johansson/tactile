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

#include "common/type/uuid.hpp"
#include "core/attributes/attribute.hpp"
#include "tactile/core/containers/string_map.hpp"
#include "tactile/core/containers/vector.hpp"
#include "core/ecs/entity.hpp"

namespace tactile {

struct AttachedComponent final {
  Entity definition {kNullEntity};  ///< The associated component definition.
  StringMap<Attribute> attributes;
};

struct Component final {
  UUID uuid {make_uuid()};
  String name;
  StringMap<Attribute> attributes;
};

struct ComponentSet final {
  Vector<Entity> definitions;  ///< Associated component definitions.
};

}  // namespace tactile
