#pragma once

#include <vector>  // vector

#include <tactile_def.hpp>

#include <centurion.hpp>  // milliseconds
#include <entt/entt.hpp>  // entity

namespace Tactile {

/**
 * \brief Component that represents a single frame in an animation.
 *
 * \see Animation
 */
struct AnimationFrame final {
  TileID tile{};
  cen::milliseconds<uint32> duration{};
};

/**
 * \brief Component that represents an animation.
 *
 * \see AnimationFrame
 */
struct Animation final {
  usize index{};                                 ///< Index of the current frame.
  cen::milliseconds<uint32> last_update_time{};  ///< When a frame was last changed.
  std::vector<entt::entity> frames;              ///< The associated frames.
};

}  // namespace Tactile