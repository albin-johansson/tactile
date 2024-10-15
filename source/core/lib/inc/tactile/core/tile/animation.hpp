// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstddef>   // size_t
#include <expected>  // expected

#include "tactile/base/debug/error_code.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile::core {

struct AnimationFrame;
class Registry;

/**
 * Updates the state of all animations in a registry.
 *
 * \param registry The associated registry.
 */
void update_animations(Registry& registry);

/**
 * Adds an animation frame to a given tile.
 *
 * \details
 * If the tile isn't already animated, a \c CAnimation component will
 * automatically be added to the tile. Furthermore, a frame index equal to the
 * current number of frames in the animation is interpreted as a request
 * to add the frame to the end of the animation.
 *
 * \note
 * If successful, the animation progress will be reset.
 *
 * \param registry    The associated registry.
 * \param tile_entity The target tile entity.
 * \param frame_index The index of the new frame.
 * \param frame       The new frame.
 *
 * \return
 * Nothing if the frame was added successfully; an error code otherwise.
 *
 * \pre The specified entity must be a valid tile.
 * \pre The new frame must feature a valid tile index.
 */
auto add_animation_frame(Registry& registry,
                         EntityID tile_entity,
                         std::size_t frame_index,
                         const AnimationFrame& frame) -> std::expected<void, ErrorCode>;

/**
 * Removes an animation frame from a given tile.
 *
 * \details
 * The animation will be removed from the tile if the specified frame is the
 * last one in the animation.
 *
 * \note
 * If successful, the animation progress will be reset.
 *
 * \param registry    The associated registry.
 * \param tile_entity The target tile entity.
 * \param frame_index The index of the frame to remove.
 *
 * \return
 * Nothing if the frame was removed successfully; an error code otherwise.
 *
 * \pre The specified entity must be a valid tile.
 * \pre The specified tile must be animated.
 */
auto remove_animation_frame(Registry& registry, EntityID tile_entity, std::size_t frame_index)
    -> std::expected<void, ErrorCode>;

}  // namespace tactile::core
