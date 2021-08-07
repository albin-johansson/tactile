#pragma once

#include <entt.hpp>  // dispatcher

#include "aliases/layer_id.hpp"
#include "core/map/layers/layer.hpp"

namespace Tactile {

class MapDocument;

/**
 * \brief Shows a layer item that represents a layer in a map.
 *
 * \ingroup gui
 *
 * \param document the currently active map document.
 * \param dispatcher the event dispatcher that will be used.
 * \param id the ID of the layer associated with the item.
 * \param layer the associated layer.
 */
void LayerItem(const MapDocument& document,
               entt::dispatcher& dispatcher,
               layer_id id,
               const SharedLayer& layer);

}  // namespace Tactile
