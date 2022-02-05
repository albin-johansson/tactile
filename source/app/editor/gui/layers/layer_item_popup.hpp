#pragma once

#include <entt/entt.hpp>  // registry, dispatcher

#include "tactile.hpp"

namespace tactile {

/**
 * \brief Updates the state of the layer item popup.
 *
 * \ingroup gui
 *
 * \param registry the currently active registry.
 * \param dispatcher the event dispatcher that will be used.
 * \param id the ID of the pressed layer.
 */
void UpdateLayerItemPopup(const entt::registry& registry,
                          entt::dispatcher& dispatcher,
                          layer_id id);

}  // namespace tactile
