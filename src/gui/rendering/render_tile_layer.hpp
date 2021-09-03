#pragma once

#include <entt.hpp>  // registry, entity

namespace Tactile {

struct RenderInfo;

/**
 * \brief Renders a tile layer.
 *
 * \ingroup rendering
 *
 * \param registry the active registry.
 * \param layerEntity the entity that represents the tile layer.
 * \param info the rendering context information that will be used.
 * \param parentOpacity the opacity of the parent layer.
 */
void RenderTileLayer(const entt::registry& registry,
                     entt::entity layerEntity,
                     const RenderInfo& info,
                     float parentOpacity);

}  // namespace Tactile
