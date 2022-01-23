#pragma once

#include <vector>  // vector

#include <centurion.hpp>  // milliseconds
#include <entt/entt.hpp>  // entity
#include <tactile_def.hpp>

namespace tactile {

/**
 * \brief Component that represents a single frame in an animation.
 *
 * \see Animation
 */
struct AnimationFrame final
{
  TileID tile{};
  cen::u32ms duration{};
};

/**
 * \brief Component that represents an animation.
 *
 * \see AnimationFrame
 */
struct Animation final
{
  usize index{};                     ///< Index of the current frame.
  cen::u32ms last_update_time{};     ///< When a frame was last changed.
  std::vector<entt::entity> frames;  ///< The associated frames.
};

}  // namespace tactile