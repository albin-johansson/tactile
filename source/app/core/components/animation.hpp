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

#include <vector>  // vector

#include <centurion/common.hpp>
#include <entt/entity/entity.hpp>

#include "core/common/identifiers.hpp"
#include "core/common/ints.hpp"

namespace tactile::comp {

/**
 * \brief Represents a single frame in an animation.
 *
 * \ingroup components
 */
struct AnimationFrame final
{
  TileID tile{};          ///< Tile that will be rendered during the frame.
  cen::u64ms duration{};  ///< Duration of the frame.
};

/**
 * \brief Represents an animation.
 *
 * \ingroup components
 */
struct Animation final
{
  usize index{};                     ///< Index of the current frame.
  cen::u64ms last_update_time{};     ///< When a frame was last changed.
  std::vector<entt::entity> frames;  ///< The associated frames.
};

}  // namespace tactile::comp