#pragma once

#include <entt/entt.hpp>

namespace tactile {

class Graphics;

/**
 * \brief Renders a map.
 *
 * \ingroup rendering
 *
 * \param graphics the graphics context that will be used.
 * \param registry the registry associated with the map that will rendered.
 */
void RenderMap(Graphics& graphics, const entt::registry& registry);

}  // namespace tactile
