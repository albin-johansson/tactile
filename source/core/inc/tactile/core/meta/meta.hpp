// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/hash_map.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/container/string_map.hpp"
#include "tactile/base/meta/attribute.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

class Registry;

namespace ir {
struct Metadata;
}  // namespace ir

/**
 * Represents an attribute bundle.
 *
 * \note
 * Attribute bundles are known as "components" in the UI, we don't use that
 * terminology in code to avoid confusion with ECS components.
 */
using AttributeBundle = StringMap<Attribute>;

/**
 * A component that provides common metadata.
 */
struct CMeta final
{
  /** The name associated with the context. */
  String name;

  /** The attached properties. */
  StringMap<Attribute> properties;

  /** The attached attribute bundles. */
  HashMap<UUID, AttributeBundle> components;
};

/**
 * Indicates whether an entity is a meta context.
 *
 * \param registry The associated registry.
 * \param id       The target entity identifier.
 *
 * \return
 * True if the entity is a meta context; false otherwise.
 */
[[nodiscard]]
auto is_meta(const Registry& registry, EntityID id) -> bool;

/**
 * Converts IR metadata to the internal representation and adds it to a context.
 *
 * \pre The entity identifier must reference a meta context.
 *
 * \param registry    The associated registry.
 * \param meta_id     The meta context entity identifier.
 * \param ir_metadata The source metadata representation.
 */
void convert_ir_metadata(Registry& registry,
                         EntityID meta_id,
                         const ir::Metadata& ir_metadata);

}  // namespace tactile
