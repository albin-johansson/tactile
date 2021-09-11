#pragma once

#include <centurion.hpp>  // frect
#include <entt.hpp>       // registry, entity

#include "aliases/ints.hpp"

struct ImVec2;

namespace Tactile {

/**
 * \brief Renders a rectangle object.
 *
 * \pre The supplied object must be a rectangle.
 *
 * \ingroup rendering
 *
 * \param registry the associated registry.
 * \param entity the point entity.
 * \param bounds the current render bounds.
 * \param color the foreground color of the rectangle.
 * \param ratio the ratio between the viewport tile size and the logical tile size.
 */
void RenderRect(const entt::registry& registry,
                entt::entity entity,
                const ImVec2& position,
                const cen::frect& bounds,
                uint32 color,
                const ImVec2& ratio);

}  // namespace Tactile
