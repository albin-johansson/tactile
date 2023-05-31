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

#include "common/enum/object_type.hpp"
#include "common/primitives.hpp"
#include "common/type/ecs.hpp"
#include "model/model.hpp"

namespace tactile::sys {

/**
 * Creates a map object.
 *
 * \param model the associated model instance.
 * \param type the type of the new object.
 *
 * \return an object entity.
 */
[[nodiscard]] auto create_object(Model& model, ObjectType type) -> Entity;

/**
 * Creates a copy of an object.
 *
 * \details The new object will not feature a meta ID, nor will it share any entities
 * associated with the old object.
 *
 * \param model the associated model instance.
 * \param object_entity the object entity that will be duplicated.
 *
 * \return an object entity.
 */
[[nodiscard]] auto duplicate_object(Model& model, Entity object_entity) -> Entity;

}  // namespace tactile::sys
