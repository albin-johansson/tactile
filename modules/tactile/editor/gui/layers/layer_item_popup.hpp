#pragma once

#include <tactile_def.hpp>

#include <entt/entt.hpp>  // registry, dispatcher

namespace Tactile {

class MapDocument;

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
                          LayerID id);

}  // namespace Tactile