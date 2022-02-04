#pragma once

#include <entt/entt.hpp>

namespace tactile {

class graphics_ctx;

/**
 * \brief Renders a tile layer.
 *
 * \ingroup rendering
 *
 * \param graphics the graphics context that will be used.
 * \param registry the current registry.
 * \param layerEntity the entity that represents the tile layer.
 * \param parentOpacity the opacity of the parent layer.
 */
void RenderTileLayer(graphics_ctx& graphics,
                     const entt::registry& registry,
                     entt::entity layerEntity,
                     float parentOpacity);

}  // namespace tactile
