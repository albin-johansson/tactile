// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/tileset/tile_animation.hpp"

#include <thread>  // this_thread

#include <gtest/gtest.h>

using namespace tactile;
using tactile::int_literals::operator""_uz;

TEST(TileAnimation, Defaults)
{
  const TileAnimation animation;
  EXPECT_EQ(animation.get_current_frame_index(), 0_uz);
  EXPECT_EQ(animation.frame_count(), 0_uz);
}

/// \tests tactile::TileAnimation::update
/// \tests tactile::TileAnimation::get_current_frame_index
TEST(TileAnimation, Update)
{
  const TileAnimationFrame frame1 {.tile_index = 10, .duration = Milliseconds {1}};
  const TileAnimationFrame frame2 {.tile_index = 20, .duration = Milliseconds {1}};
  const TileAnimationFrame frame3 {.tile_index = 30, .duration = Milliseconds {1}};

  TileAnimation animation;
  animation.append_frame(frame1);
  animation.append_frame(frame2);
  animation.append_frame(frame3);

  EXPECT_EQ(animation.get_current_frame_index(), 0_uz);

  std::this_thread::sleep_for(Milliseconds {10});
  animation.update();
  EXPECT_EQ(animation.get_current_frame_index(), 1_uz);

  std::this_thread::sleep_for(Milliseconds {10});
  animation.update();
  EXPECT_EQ(animation.get_current_frame_index(), 2_uz);

  std::this_thread::sleep_for(Milliseconds {10});
  animation.update();
  EXPECT_EQ(animation.get_current_frame_index(), 0_uz);
}

/// \tests tactile::TileAnimation::append_frame
/// \tests tactile::TileAnimation::frame_count
TEST(TileAnimation, AppendFrame)
{
  const TileAnimationFrame frame1 {.tile_index = 194, .duration = Milliseconds {1'000}};
  const TileAnimationFrame frame2 {.tile_index = 943, .duration = Milliseconds {1'000}};

  TileAnimation animation;

  animation.append_frame(frame1);
  ASSERT_EQ(animation.frame_count(), 1_uz);
  EXPECT_EQ(animation.get_frame(0_uz), frame1);

  animation.append_frame(frame2);
  ASSERT_EQ(animation.frame_count(), 2_uz);
  EXPECT_EQ(animation.get_frame(0_uz), frame1);
  EXPECT_EQ(animation.get_frame(1_uz), frame2);
}

/// \tests tactile::TileAnimation::insert_frame
/// \tests tactile::TileAnimation::frame_count
TEST(TileAnimation, InsertFrame)
{
  const TileAnimationFrame frame1 {.tile_index = 123, .duration = Milliseconds {792}};
  const TileAnimationFrame frame2 {.tile_index = 456, .duration = Milliseconds {214}};
  const TileAnimationFrame frame3 {.tile_index = 789, .duration = Milliseconds {468}};
  const TileAnimationFrame frame4 {.tile_index = 382, .duration = Milliseconds {159}};

  TileAnimation animation;
  ASSERT_EQ(animation.frame_count(), 0_uz);

  animation.insert_frame(0_uz, frame1);
  ASSERT_EQ(animation.frame_count(), 1_uz);
  EXPECT_EQ(animation.get_frame(0_uz), frame1);

  // Insert at beginning.
  animation.insert_frame(0_uz, frame2);
  ASSERT_EQ(animation.frame_count(), 2_uz);
  EXPECT_EQ(animation.get_frame(0_uz), frame2);
  EXPECT_EQ(animation.get_frame(1_uz), frame1);

  // Insert at end.
  animation.insert_frame(2_uz, frame3);
  ASSERT_EQ(animation.frame_count(), 3_uz);
  EXPECT_EQ(animation.get_frame(0_uz), frame2);
  EXPECT_EQ(animation.get_frame(1_uz), frame1);
  EXPECT_EQ(animation.get_frame(2_uz), frame3);

  // Insert into middle.
  animation.insert_frame(1_uz, frame4);
  ASSERT_EQ(animation.frame_count(), 4_uz);
  EXPECT_EQ(animation.get_frame(0_uz), frame2);
  EXPECT_EQ(animation.get_frame(1_uz), frame4);
  EXPECT_EQ(animation.get_frame(2_uz), frame1);
  EXPECT_EQ(animation.get_frame(3_uz), frame3);

  EXPECT_NO_THROW(animation.insert_frame(animation.frame_count() + 1_uz, frame1));

  ASSERT_EQ(animation.frame_count(), 4_uz);
  EXPECT_EQ(animation.get_frame(0_uz), frame2);
  EXPECT_EQ(animation.get_frame(1_uz), frame4);
  EXPECT_EQ(animation.get_frame(2_uz), frame1);
  EXPECT_EQ(animation.get_frame(3_uz), frame3);
}

/// \tests tactile::TileAnimation::remove_frame
/// \tests tactile::TileAnimation::frame_count
TEST(TileAnimation, RemoveFrame)
{
  const TileAnimationFrame frame1 {.tile_index = 1, .duration = Milliseconds {10}};
  const TileAnimationFrame frame2 {.tile_index = 2, .duration = Milliseconds {20}};
  const TileAnimationFrame frame3 {.tile_index = 3, .duration = Milliseconds {30}};
  const TileAnimationFrame frame4 {.tile_index = 4, .duration = Milliseconds {40}};

  TileAnimation animation;
  animation.append_frame(frame1);
  animation.append_frame(frame2);
  animation.append_frame(frame3);
  animation.append_frame(frame4);
  ASSERT_EQ(animation.frame_count(), 4_uz);

  animation.remove_frame(1_uz);
  ASSERT_EQ(animation.frame_count(), 3_uz);
  EXPECT_EQ(animation.get_frame(0_uz), frame1);
  EXPECT_EQ(animation.get_frame(1_uz), frame3);
  EXPECT_EQ(animation.get_frame(2_uz), frame4);

  animation.remove_frame(2_uz);
  ASSERT_EQ(animation.frame_count(), 2_uz);
  EXPECT_EQ(animation.get_frame(0_uz), frame1);
  EXPECT_EQ(animation.get_frame(1_uz), frame3);

  animation.remove_frame(1_uz);
  ASSERT_EQ(animation.frame_count(), 1_uz);
  EXPECT_EQ(animation.get_frame(0_uz), frame1);

  animation.remove_frame(0_uz);
  ASSERT_EQ(animation.frame_count(), 0_uz);

  EXPECT_NO_THROW(animation.remove_frame(0));
}

/// \tests tactile::TileAnimation::get_frame
TEST(TileAnimation, GetFrame)
{
  const TileAnimationFrame frame1 {.tile_index = 100, .duration = Milliseconds::zero()};
  const TileAnimationFrame frame2 {.tile_index = 200, .duration = Milliseconds::zero()};

  TileAnimation animation;
  animation.append_frame(frame1);
  animation.append_frame(frame2);

  EXPECT_EQ(animation.get_frame(0_uz), frame1);
  EXPECT_EQ(animation.get_frame(1_uz), frame2);
  EXPECT_ANY_THROW((void) animation.get_frame(2_uz));
}