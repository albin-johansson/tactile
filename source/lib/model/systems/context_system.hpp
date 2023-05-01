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
#include "common/type/string.hpp"
#include "core/context.hpp"
#include "model/model.hpp"

namespace tactile::sys {

/// Indicates whether an entity is a valid context entity.
[[nodiscard]] auto is_context_entity(const Model& model, Entity entity) -> bool;

/// Renames an existing property in a context.
///
/// \pre `new_name` must not refer to a name of another property of the context.
///
/// \param model the associated model.
/// \param context_entity the target context entity.
/// \param current_name the current name of the property.
/// \param new_name the new name of the property.
void rename_property(Model& model,
                     Entity context_entity,
                     StringView current_name,
                     String new_name);

/// Creates a copy of the context component attached to a context entity.
[[nodiscard]] auto copy_context_component_from(Model& model, Entity context_entity)
    -> Context;

}  // namespace tactile::sys
