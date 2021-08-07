#pragma once

#include <entt.hpp>  // dispatcher

#include "aliases/layer_id.hpp"

namespace Tactile {

class MapDocument;

/**
 * \brief Updates the state of the layer item popup.
 *
 * \ingroup gui
 *
 * \param document the currently active map document.
 * \param dispatcher the event dispatcher that will be used.
 * \param id the ID of the pressed layer.
 */
void UpdateLayerItemPopup(const MapDocument& document,
                          entt::dispatcher& dispatcher,
                          layer_id id);

}  // namespace Tactile
