#pragma once

#include <tactile-base/tactile_std.hpp>

#include <centurion.hpp>  // frect
#include <entt.hpp>       // registry, entity

#include "editor/gui/rendering/render_info.hpp"

struct ImVec2;

namespace Tactile {

/**
 * \brief Renders a point object.
 *
 * \pre The supplied object must be a point.
 *
 * \ingroup rendering
 *
 * \param registry the associated registry.
 * \param entity the point entity.
 * \param info the current render info.
 * \param position the translated position of the point.
 * \param color the foreground color of the rectangle.
 */
void RenderPoint(const entt::registry& registry,
                 entt::entity entity,
                 const RenderInfo& info,
                 const ImVec2& position,
                 uint32 color);

}  // namespace Tactile
