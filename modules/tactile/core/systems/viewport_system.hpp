#pragma once

#include <entt.hpp>  // registry

namespace Tactile::Sys {

/// \name Viewport system
/// \{

/**
 * \brief Offsets the current viewport.
 *
 * \param registry the associated registry.
 * \param dx the x-axis offset.
 * \param dy the y-axis offset.
 *
 * \since 0.2.0
 */
void OffsetViewport(entt::registry& registry, float dx, float dy);

/**
 * \brief Pans the viewport to the left.
 *
 * \param registry the associated registry.
 *
 * \since 0.2.0
 */
void PanViewportLeft(entt::registry& registry);

/**
 * \brief Pans the viewport to the right.
 *
 * \param registry the associated registry.
 *
 * \since 0.2.0
 */
void PanViewportRight(entt::registry& registry);

/**
 * \brief Pans the viewport upwards.
 *
 * \param registry the associated registry.
 *
 * \since 0.2.0
 */
void PanViewportUp(entt::registry& registry);

/**
 * \brief Pans the viewport downwards.
 *
 * \param registry the associated registry.
 *
 * \since 0.2.0
 */
void PanViewportDown(entt::registry& registry);

/**
 * \brief Resets the zoom (viewport tile size) to the default.
 *
 * \param registry the associated registry.
 *
 * \since 0.2.0
 */
void ResetViewportZoom(entt::registry& registry);

/**
 * \brief Decreases the viewport zoom.
 *
 * \pre The viewport tile size must be large enough to support a decrease.
 *
 * \param registry the associated registry.
 *
 * \see `CanDecreaseViewportZoom()`
 *
 * \since 0.2.0
 */
void DecreaseViewportZoom(entt::registry& registry);

/**
 * \brief Increases the viewport zoom.
 *
 * \param registry the associated registry.
 *
 * \since 0.2.0
 */
void IncreaseViewportZoom(entt::registry& registry);

/**
 * \brief Indicates whether or not the viewport zoom can be decreased.
 *
 * \param registry the associated registry.
 *
 * \return `true` if the viewport zoom can be decreased; `false` otherwise.
 *
 * \see `DecreaseViewportZoom()`
 *
 * \since 0.2.0
 */
[[nodiscard]] auto CanDecreaseViewportZoom(const entt::registry& registry) -> bool;

/// \} End of viewport system

}  // namespace Tactile::Sys
