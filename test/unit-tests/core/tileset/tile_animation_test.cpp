// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

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
