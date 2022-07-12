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

#include "core/tileset/tile_animation.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"

namespace tactile::test {

using Millis = TileAnimation::Millis;

TEST(TileAnimation, Defaults)
{
  const TileAnimation animation;
  ASSERT_EQ(0, animation.size());
  ASSERT_EQ(0, animation.capacity());
}

TEST(TileAnimation, ReserveFrames)
{
  TileAnimation animation;

  animation.reserve_frames(3);
  ASSERT_EQ(3, animation.capacity());
  ASSERT_EQ(0, animation.size());
}

TEST(TileAnimation, AddFrame)
{
  TileAnimation animation;

  animation.add_frame(42, Millis {14});
  ASSERT_EQ(1, animation.size());

  const auto& frame = animation.nth(0);
  ASSERT_EQ(42, frame.tile);
  ASSERT_EQ(Millis {14}, frame.duration);

  ASSERT_THROW(animation.nth(1), TactileError);
}

}  // namespace tactile::test
