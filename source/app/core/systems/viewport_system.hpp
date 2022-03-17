/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <entt/entt.hpp>

namespace tactile::sys {

/// \name Viewport system
/// \{

struct ViewportScalingRatio final
{
  float x{};
  float y{};
};

/**
 * \brief Offsets the current viewport.
 *
 * \param registry the document registry.
 * \param dx the x-axis offset.
 * \param dy the y-axis offset.
 */
void offset_viewport(entt::registry& registry, float dx, float dy);

/**
 * \brief Offsets a viewport that is bound to the size of an underlying texture.
 *
 * \pre The entity must feature a viewport component.
 * \pre The entity must feature a texture component.
 *
 * \param registry the source registry.
 * \param entity the entity that provides the viewport and texture components.
 * \param dx the x-axis offset.
 * \param dy the y-axis offset.
 * \param viewWidth the width of the viewport.
 * \param viewHeight the height of the viewport.
 */
void offset_bound_viewport(entt::registry& registry,
                           entt::entity entity,
                           float dx,
                           float dy,
                           float viewWidth,
                           float viewHeight);

/**
 * \brief Pans the viewport to the left.
 *
 * \param registry the document registry.
 */
void pan_viewport_left(entt::registry& registry);

/**
 * \brief Pans the viewport to the right.
 *
 * \param registry the document registry.
 */
void pan_viewport_right(entt::registry& registry);

/**
 * \brief Pans the viewport upwards.
 *
 * \param registry the document registry.
 */
void pan_viewport_up(entt::registry& registry);

/**
 * \brief Pans the viewport downwards.
 *
 * \param registry the document registry.
 */
void pan_viewport_down(entt::registry& registry);

/**
 * \brief Resets the zoom (viewport tile size) to the default.
 *
 * \param registry the document registry.
 */
void reset_viewport_zoom(entt::registry& registry);

/**
 * \brief Decreases the viewport zoom.
 *
 * \pre The viewport tile size must be large enough to support a decrease.
 *
 * \param registry the document registry.
 * \param mouseX the mouse x-coordinate.
 * \param mouseY the mouse y-coordinate
 *
 * \see `can_decrease_viewport_zoom()`
 */
void decrease_viewport_zoom(entt::registry& registry, float mouseX, float mouseY);

/**
 * \brief Increases the viewport zoom.
 *
 * \param registry the document registry.
 * \param mouseX the mouse x-coordinate.
 * \param mouseY the mouse y-coordinate
 */
void increase_viewport_zoom(entt::registry& registry, float mouseX, float mouseY);

/**
 * \brief Indicates whether or not the viewport zoom can be decreased.
 *
 * \param registry the document registry.
 *
 * \return `true` if the viewport zoom can be decreased; `false` otherwise.
 *
 * \see `decrease_viewport_zoom()`
 */
[[nodiscard]] auto can_decrease_viewport_zoom(const entt::registry& registry) -> bool;

/**
 * \brief Returns the relationship between the viewport and logical tile sizes.
 *
 * \param registry the current registry.
 *
 * \return the ratio between the viewport tile size and the map tile size.
 */
[[nodiscard]] auto get_viewport_scaling_ratio(const entt::registry& registry)
    -> ViewportScalingRatio;

/// \} End of viewport system

}  // namespace tactile::sys
