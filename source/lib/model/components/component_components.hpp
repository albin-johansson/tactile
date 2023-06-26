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

#include "common/attribute.hpp"
#include "common/type/ecs.hpp"
#include "common/type/string_map.hpp"
#include "common/type/vector.hpp"

namespace tactile {

// TODO consider using UUID for component type reference, could make some systems simpler
struct AttachedComponent final {
  Entity definition {kNullEntity};  ///< The associated component definition.
  StringMap<Attribute> attributes;
};

struct ComponentDefinition final {
  String name;
  StringMap<Attribute> attributes;
};

struct ComponentSet final {
  Vector<Entity> definitions;  ///< Associated component definitions.
};

}  // namespace tactile
