#pragma once

#include <entt/entt.hpp>

namespace tactile {

class graphics_ctx;

/**
 * \brief Renders a map.
 *
 * \ingroup rendering
 *
 * \param graphics the graphics context that will be used.
 * \param registry the registry associated with the map that will rendered.
 */
void RenderMap(graphics_ctx& graphics, const entt::registry& registry);

}  // namespace tactile
