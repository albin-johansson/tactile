#pragma once

#include <centurion.hpp>  // frect
#include <entt.hpp>       // registry, entity

#include "common/ints.hpp"
#include "editor/gui/rendering/render_info.hpp"

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
 * \param info the current render info.
 * \param color the foreground color of the rectangle.
 */
void RenderRect(const entt::registry& registry,
                entt::entity entity,
                const RenderInfo& info,
                const ImVec2& position,
                uint32 color);

}  // namespace Tactile
