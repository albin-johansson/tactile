#pragma once

struct ImVec2;

#include <tactile_def.hpp>

#include <entt.hpp>  // registry, entity

#include "editor/gui/rendering/render_info.hpp"

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
 * \param info the current render info.
 * \param position the translated position of the ellipse.
 * \param color the foreground color of the ellipse.
 */
void RenderEllipse(const entt::registry& registry,
                   entt::entity entity,
                   const RenderInfo& info,
                   const ImVec2& position,
                   uint32 color);

}  // namespace Tactile
