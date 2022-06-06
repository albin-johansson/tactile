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

#include <entt/fwd.hpp>

#include "core/common/math.hpp"

namespace tactile::sys {

/// \name Viewport system
/// \{

/**
 * \brief Offsets the current viewport.
 *
 * \param registry the document registry.
 * \param offset the offset in both axes.
 */
void offset_viewport(entt::registry& registry, const Vector2f& offset);

void offset_viewport(entt::registry& registry,
                     entt::entity entity,
                     const Vector2f& offset);

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
 * \param mousePos the mouse coordinate.
 */
void decrease_viewport_zoom(entt::registry& registry, const Vector2f& mousePos);

/**
 * \brief Increases the viewport zoom.
 *
 * \param registry the document registry.
 * \param mousePos the mouse coordinate.
 */
void increase_viewport_zoom(entt::registry& registry, const Vector2f& mousePos);

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
[[nodiscard]] auto get_viewport_scaling_ratio(const entt::registry& registry) -> Vector2f;

/// \} End of viewport system

}  // namespace tactile::sys
