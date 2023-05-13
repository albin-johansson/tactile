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
#include "common/type/hash_map.hpp"
#include "common/type/string.hpp"
#include "common/type/string_map.hpp"
#include "core/context.hpp"
#include "model/model.hpp"

namespace tactile::sys {

/// Creates a copy of the context component attached to a context entity.
[[nodiscard]] auto copy_context(Model& model, const Context& src_context) -> Context;

/// Copies the attributes of all components of a specific component type.
[[nodiscard]] auto copy_component_values(const Model& model,
                                         ComponentDefinitionEntity definition_entity)
    -> HashMap<ContextEntity, StringMap<Attribute>>;

/// Copies a specific attribute in all components of a specific component type.
[[nodiscard]] auto copy_single_attribute_in_components(const Model& model,
                                                       Entity definition_entity,
                                                       StringView attribute_name)
    -> HashMap<ComponentEntity, Attribute>;

}  // namespace tactile::sys
