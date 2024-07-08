// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/string.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/layer/object_type.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

class Registry;

/**
 * A component for layer objects.
 */
struct CObject final
{
  /** The associated identifier. */
  ObjectID id;

  /** The object position. */
  Float2 position;

  /** The object size. */
  Float2 size;

  /** The object type. */
  ObjectType type;

  /** Arbitrary user-provided tag. */
  String tag;

  /** Indicates whether the object is rendered. */
  bool is_visible;
};

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
 * Destroys an object entity.
 *
 * \pre The provided entity must represent an object.
 *
 * \param registry      The associated registry.
 * \param object_entity The object entity to destroy.
 */
void destroy_object(Registry& registry, EntityID object_entity);

/**
 * Creates a deep copy of an object.
 *
 * \pre The specified entity must be an object.
 *
 * \param registry      The associated registry.
 * \param object_entity The object that will be copied.
 *
 * \return
 * An object entity.
 */
[[nodiscard]]
auto copy_object(Registry& registry, EntityID object_entity) -> EntityID;

}  // namespace tactile
