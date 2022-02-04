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
void OffsetViewport(entt::registry& registry, float dx, float dy);

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
void OffsetBoundViewport(entt::registry& registry,
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
void PanViewportLeft(entt::registry& registry);

/**
 * \brief Pans the viewport to the right.
 *
 * \param registry the document registry.
 */
void PanViewportRight(entt::registry& registry);

/**
 * \brief Pans the viewport upwards.
 *
 * \param registry the document registry.
 */
void PanViewportUp(entt::registry& registry);

/**
 * \brief Pans the viewport downwards.
 *
 * \param registry the document registry.
 */
void PanViewportDown(entt::registry& registry);

/**
 * \brief Resets the zoom (viewport tile size) to the default.
 *
 * \param registry the document registry.
 */
void ResetViewportZoom(entt::registry& registry);

/**
 * \brief Decreases the viewport zoom.
 *
 * \pre The viewport tile size must be large enough to support a decrease.
 *
 * \param registry the document registry.
 * \param mouseX the mouse x-coordinate.
 * \param mouseY the mouse y-coordinate
 *
 * \see `CanDecreaseViewportZoom()`
 */
void DecreaseViewportZoom(entt::registry& registry, float mouseX, float mouseY);

/**
 * \brief Increases the viewport zoom.
 *
 * \param registry the document registry.
 * \param mouseX the mouse x-coordinate.
 * \param mouseY the mouse y-coordinate
 */
void IncreaseViewportZoom(entt::registry& registry, float mouseX, float mouseY);

/**
 * \brief Indicates whether or not the viewport zoom can be decreased.
 *
 * \param registry the document registry.
 *
 * \return `true` if the viewport zoom can be decreased; `false` otherwise.
 *
 * \see `DecreaseViewportZoom()`
 */
[[nodiscard]] auto CanDecreaseViewportZoom(const entt::registry& registry) -> bool;

/**
 * \brief Returns the relationship between the viewport and logical tile sizes.
 *
 * \param registry the current registry.
 *
 * \return the ratio between the viewport tile size and the map tile size.
 */
[[nodiscard]] auto GetViewportScalingRatio(const entt::registry& registry)
    -> ViewportScalingRatio;

/// \} End of viewport system

}  // namespace tactile::sys
