#pragma once

#include <entt/entt.hpp>

namespace Tactile {

class Graphics;

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
void RenderTileLayer(Graphics& graphics,
                     const entt::registry& registry,
                     entt::entity layerEntity,
                     float parentOpacity);

}  // namespace Tactile
