// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

class Registry;

/// \addtogroup Viewport
/// \{

/**
 * A component that represents a viewport over a tile grid.
 */
struct CViewport final
{
  /** The position of the top-left corner, relative to the content origin. */
  Float2 pos;

  /** The size of the viewport. */
  Float2 size;

  /** The content scale factor. */
  float scale;
};

/**
 * A component that provides constraints for a viewport.
 */
struct CViewportLimits final
{
  /** The minimum X and Y coordinates of the viewport. */
  Float2 min_pos;

  /** The maximum X and Y coordinates of the viewport. */
  Float2 max_pos;
};

/**
 * Indicates whether an entity is a viewport.
 *
 * \details
 * Viewport entities feature the following components. \n
 * - \c CViewport. \n
 * - \c CViewportLimits (optional).
 *
 * \param registry The associated registry.
 * \param entity   The entity to check.
 *
 * \return
 * True if the entity is a viewport; false otherwise.
 */
[[nodiscard]]
auto is_viewport(const Registry& registry, EntityID entity) -> bool;

/**
 * Constrains the possible positions of a viewport.
 *
 * \details
 * This function adds a \c CViewportLimits component to the given viewport
 * entity. Remember to use the \c translate_viewport function to enforce the
 * provided limits.
 *
 * \pre The specified entity must be a viewport.
 *
 * \param registry        The associated registry.
 * \param viewport_entity The target viewport entity.
 * \param min_pos         The minimum possible position.
 * \param max_pos         The maximum possible position.
 */
void set_viewport_limits(Registry& registry,
                         EntityID viewport_entity,
                         const Float2& min_pos,
                         const Float2& max_pos);

/**
 * Translates the offset of a viewport.
 *
 * \details
 * This function will take viewport limits into account if the viewport entity
 * features a \c CViewportLimitsComponent.
 *
 * \pre The specified entity must be a viewport.
 *
 * \param registry        The associated registry.
 * \param viewport_entity The target viewport entity.
 * \param delta           The offset delta.
 */
void translate_viewport(Registry& registry,
                        EntityID viewport_entity,
                        const Float2& delta);

/**
 * Increases the zoom of a viewport.
 *
 * \details
 * This function will take viewport limits into account if the viewport entity
 * features a \c ViewportLimitsComponent.
 *
 * \pre The specified entity must be a viewport.
 *
 * \param registry          The associated registry.
 * \param viewport_entity   The target viewport entity.
 * \param anchor_screen_pos The target of the zoom operation.
 */
void increase_viewport_zoom(Registry& registry,
                            EntityID viewport_entity,
                            const Float2& anchor_screen_pos);

/**
 * Decreases the zoom of a viewport.
 *
 * \details
 * This function will take viewport limits into account if the viewport entity
 * features a \c ViewportLimitsComponent.
 *
 * \pre The specified entity must be a viewport.
 *
 * \param registry          The associated registry.
 * \param viewport_entity   The target viewport entity.
 * \param anchor_screen_pos The target of the zoom operation.
 */
void decrease_viewport_zoom(Registry& registry,
                            EntityID viewport_entity,
                            const Float2& anchor_screen_pos);

/**
 * Resets the zoom of a viewport.
 *
 * \pre The specified entity must be a viewport.
 *
 * \param registry        The associated registry.
 * \param viewport_entity The target viewport entity.
 */
void reset_viewport_zoom(Registry& registry, EntityID viewport_entity);

/**
 * Aligns a viewport to be centered over the associated content.
 *
 * \pre The specified entity must be a viewport.
 *
 * \param registry        The associated registry.
 * \param viewport_entity The target viewport entity.
 * \param content_size    The size of the underlying content.
 */
void center_viewport_over_content(Registry& registry,
                                  EntityID viewport_entity,
                                  const Float2& content_size);

/**
 * Converts a position using screen coordinates to one with world coordinates.
 *
 * \param viewport   The associated viewport.
 * \param screen_pos The position to convert.
 *
 * \return
 * A position using world coordinates.
 */
[[nodiscard]]
auto to_world_pos(const CViewport& viewport,
                  const Float2& screen_pos) noexcept -> Float2;

/// \}

}  // namespace tactile
