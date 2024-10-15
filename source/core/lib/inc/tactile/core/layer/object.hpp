// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <string>  // string

#include "tactile/base/id.hpp"
#include "tactile/base/io/save/ir.hpp"
#include "tactile/base/layer/object_type.hpp"
#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile::core {

class Registry;

/**
 * Indicates whether an entity represents an object.
 *
 * \details
 * Object entities feature the following components: \n
 * - \c CMeta \n
 * - \c CObject
 *
 * \param registry The associated registry.
 * \param entity   The entity to check.
 *
 * \return
 * True if the entity is an object; false otherwise.
 */
[[nodiscard]]
auto is_object(const Registry& registry, EntityID entity) -> bool;

/**
 * Creates an object.
 *
 * \param registry The associated registry.
 * \param id       The object identifier.
 * \param type     The object type.
 *
 * \return
 * An object entity.
 */
[[nodiscard]]
auto make_object(Registry& registry, ObjectID id, ObjectType type) -> EntityID;

/**
 * Creates an object from an intermediate representation.
 *
 * \param registry  The associated registry.
 * \param ir_object The intermediate object representation.
 *
 * \return
 * An object entity.
 */
[[nodiscard]]
auto make_object(Registry& registry, const ir::Object& ir_object) -> EntityID;

/**
 * Destroys an object entity.
 *
 * \param registry      The associated registry.
 * \param object_entity The target object.
 *
 * \pre The specified entity must be a valid object.
 */
void destroy_object(Registry& registry, EntityID object_entity);

/**
 * Creates a deep copy of an object.
 *
 * \param registry      The associated registry.
 * \param object_entity The target object.
 *
 * \return
 * An object entity.
 *
 * \pre The specified entity must be a valid object.
 */
[[nodiscard]]
auto copy_object(Registry& registry, EntityID object_entity) -> EntityID;

}  // namespace tactile::core
