#pragma once

#include <entt/entt.hpp>  // registry, entity

namespace Tactile {

struct RenderInfo;

/**
 * \brief Renders an object layer.
 *
 * \ingroup rendering
 *
 * \param registry the associated registry.
 * \param layerEntity the object layer entity.
 * \param info the rendering context information that will be used.
 * \param parentOpacity the opacity of the parent layer.
 */
void RenderObjectLayer(const entt::registry& registry,
                       entt::entity layerEntity,
                       const RenderInfo& info,
                       float parentOpacity);

}  // namespace Tactile
