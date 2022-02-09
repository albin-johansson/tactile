#pragma once

#include <entt/entt.hpp>

#include "core/components/layer.hpp"
#include "tactile.hpp"

namespace tactile {

/**
 * \brief Shows a layer item that represents a layer in a map.
 *
 * \ingroup gui
 *
 * \param registry the document registry.
 * \param dispatcher the event dispatcher that will be used.
 * \param layerEntity the entity of the layer that will be shown.
 */
void show_layer_item(const entt::registry& registry,
                     entt::dispatcher& dispatcher,
                     entt::entity layerEntity);

}  // namespace tactile
