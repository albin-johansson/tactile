// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>   // int32_t
#include <optional>  // optional

#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

class Registry;

/**
 * A component that provides common layer information.
 */
struct CLayer final
{
  /** The associated identifier used in save files. */
  std::optional<std::int32_t> persistent_id;

  /** The opacity of the layer when rendered. */
  float opacity;

  /** Indicates whether the layer is rendered. */
  bool visible;
};

/**
 * Indicates whether an entity is a layer.
 *
 * \details
 * Layer entities feature the following components. \n
 * - \c CMeta \n
 * - \c CLayer
 *
 * \note
 * This function only checks if the entity features the common layer components,
 * it doesn't check that the entity is one of the supported layer variants.
 *
 * \param registry The associated registry.
 * \param entity   The entity identifier to check.
 *
 * \return
 * True if the entity is a layer; false otherwise.
 */
[[nodiscard]]
auto is_layer(const Registry& registry, EntityID entity) -> bool;

/**
 * Creates an unspecialized layer entity.
 *
 * \warning
 * This function should only ever be used by other layer factory functions,
 * since the resulting layers are not immediately usable.
 *
 * \param registry The associated registry.
 *
 * \return
 * A layer entity.
 */
[[nodiscard]]
auto make_unspecialized_layer(Registry& registry) -> EntityID;

}  // namespace tactile
