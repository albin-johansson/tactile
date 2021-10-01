#pragma once

#include <entt.hpp>  // registry, entity, dispatcher

#include "common/layer_id.hpp"
#include "core/components/layer.hpp"

namespace Tactile {

class MapDocument;

/**
 * \brief Shows a layer item that represents a layer in a map.
 *
 * \ingroup gui
 *
 * \param registry the currently active registry.
 * \param dispatcher the event dispatcher that will be used.
 * \param layerEntity the associated layer entity.
 * \param layer the associated layer.
 */
void LayerItem(const entt::registry& registry,
               entt::dispatcher& dispatcher,
               entt::entity layerEntity,
               const Layer& layer);

}  // namespace Tactile
