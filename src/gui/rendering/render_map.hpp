#pragma once

#include <entt.hpp>  // registry

namespace Tactile {

struct RenderInfo;

/**
 * \brief Renders a map.
 *
 * \ingroup rendering
 *
 * \param registry the registry associated with the map that will rendered.
 * \param info the rendering context information that will be used.
 */
void RenderMap(const entt::registry& registry, const RenderInfo& info);

}  // namespace Tactile
