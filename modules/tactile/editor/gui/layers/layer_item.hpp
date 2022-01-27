#pragma once

#include <entt/entt.hpp>  // registry, entity, dispatcher
#include <tactile_def.hpp>

#include "core/components/layer.hpp"

namespace tactile {

class Icons;

/**
 * \brief Shows a layer item that represents a layer in a map.
 *
 * \ingroup gui
 *
 * \param registry the currently active registry.
 * \param icons the loaded icons.
 * \param dispatcher the event dispatcher that will be used.
 * \param layerEntity the associated layer entity.
 * \param layer the associated layer.
 */
void LayerItem(const entt::registry& registry,
               const Icons& icons,
               entt::dispatcher& dispatcher,
               entt::entity layerEntity,
               const comp::layer& layer);

}  // namespace tactile
