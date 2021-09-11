#pragma once

struct ImVec2;

#include <entt.hpp>  // registry, entity

#include "aliases/ints.hpp"

namespace Tactile {

/**
 * \brief Renders an ellipse object.
 *
 * \pre The supplied object must be an ellipse.
 *
 * \ingroup rendering
 *
 * \param registry the associated registry.
 * \param entity the point entity.
 * \param position the translated position of the ellipse.
 * \param ratio the ratio between the viewport tile size and the logical tile size.
 * \param color the foreground color of the ellipse.
 */
void RenderEllipse(const entt::registry& registry,
                   entt::entity entity,
                   const ImVec2& position,
                   const ImVec2& ratio,
                   uint32 color);

}  // namespace Tactile
