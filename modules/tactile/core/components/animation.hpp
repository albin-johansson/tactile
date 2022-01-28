#pragma once

#include <vector>  // vector

#include <centurion.hpp>
#include <entt/entt.hpp>

#include "tactile_def.hpp"

namespace tactile::comp {

/**
 * \brief Represents a single frame in an animation.
 */
struct animation_frame final
{
  TileID tile{};          ///< Tile that will be rendered during the frame.
  cen::u32ms duration{};  ///< Duration of the frame.
};

/**
 * \brief Represents an animation.
 */
struct animation final
{
  usize index{};                     ///< Index of the current frame.
  cen::u32ms last_update_time{};     ///< When a frame was last changed.
  std::vector<entt::entity> frames;  ///< The associated frames.
};

}  // namespace tactile::comp