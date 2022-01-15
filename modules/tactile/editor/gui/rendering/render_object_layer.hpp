#pragma once

#include <centurion.hpp>
#include <entt/entt.hpp>

namespace Tactile {

class Graphics;

void RenderObject(Graphics& graphics,
                  const entt::registry& registry,
                  entt::entity objectEntity,
                  const cen::Color& color);

/**
 * \brief Renders an object layer.
 *
 * \ingroup rendering
 *
 * \param graphics the graphics context that will be used.
 * \param registry the associated registry.
 * \param layerEntity the object layer entity.
 * \param parentOpacity the opacity of the parent layer.
 */
void RenderObjectLayer(Graphics& graphics,
                       const entt::registry& registry,
                       entt::entity layerEntity,
                       float parentOpacity);

}  // namespace Tactile
