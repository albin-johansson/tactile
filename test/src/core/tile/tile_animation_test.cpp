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

#include "core/tile/tile_animation.hpp"

#include <gtest/gtest.h>

#include "common/type/chrono.hpp"
#include "core/debug/panic.hpp"

using namespace std::chrono_literals;

namespace tactile::test {

TEST(TileAnimation, Defaults)
{
  const TileAnimation animation;
  ASSERT_EQ(0u, animation.size());
  ASSERT_EQ(0u, animation.capacity());
}

TEST(TileAnimation, ReserveFrames)
{
  TileAnimation animation;

  animation.reserve_frames(3);
  ASSERT_EQ(3u, animation.capacity());
  ASSERT_EQ(0u, animation.size());
}

TEST(TileAnimation, AddFrame)
{
  TileAnimation animation;

  animation.add_frame(42, 14ms);
  ASSERT_EQ(1u, animation.size());

  const auto& frame = animation[0];
  ASSERT_EQ(42, frame.tile);
  ASSERT_EQ(14ms, frame.duration);

  ASSERT_THROW(animation[1], TactileError);
}

TEST(TileAnimation, InsertFrame)
{
  TileAnimation animation;

  ASSERT_EQ(success, animation.insert_frame(0, 5, 100ms));
  ASSERT_EQ(1u, animation.size());

  ASSERT_EQ(success, animation.insert_frame(0, 20, 100ms));
  ASSERT_EQ(2u, animation.size());

  ASSERT_EQ(success, animation.insert_frame(animation.size(), 30, 100ms));
  ASSERT_EQ(3u, animation.size());

  ASSERT_EQ(failure, animation.insert_frame(animation.size() + 1, 40, 100ms));
  ASSERT_EQ(3u, animation.size());
}

TEST(TileAnimation, RemoveFrame)
{
  TileAnimation animation;
  ASSERT_EQ(failure, animation.remove_frame(0));

  animation.add_frame(12, 42ms);
  ASSERT_EQ(1u, animation.size());

  ASSERT_EQ(failure, animation.remove_frame(1));
  ASSERT_EQ(1u, animation.size());

  ASSERT_EQ(success, animation.remove_frame(0));
  ASSERT_EQ(failure, animation.remove_frame(0));
  ASSERT_EQ(0u, animation.size());
}

}  // namespace tactile::test
