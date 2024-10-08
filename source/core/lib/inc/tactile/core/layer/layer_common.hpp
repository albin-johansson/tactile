// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/id.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

class Registry;

namespace ir {
struct Layer;
}  // namespace ir

/**
 * Creates a layer from an intermediate representation.
 *
 * \param registry The associated registry.
 * \param ir_layer The intermediate layer representation.
 *
 * \return
 * A layer entity identifier.
 */
[[nodiscard]]
auto make_layer(Registry& registry, const ir::Layer& ir_layer) -> EntityID;

/**
 * Destroys a layer entity.
 *
 * \pre The layer identifier must be valid.
 *
 * \param registry The associated registry.
 * \param layer_id The identifier of the layer to destroy.
 */
void destroy_layer(Registry& registry, EntityID layer_id);

/**
 * Creates a deep copy of a layer.
 *
 * \pre The layer identifier must be valid.
 *
 * \param[in]     registry        The associated registry.
 * \param[in]     source_layer_id The source layer identifier.
 * \param[in,out] next_layer_id   The next available persistent layer identifier.
 *
 * \return
 * A layer entity identifier.
 */
[[nodiscard]]
auto copy_layer(Registry& registry, EntityID source_layer_id, LayerID& next_layer_id)
    -> EntityID;

}  // namespace tactile
