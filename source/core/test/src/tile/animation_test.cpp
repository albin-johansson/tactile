// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/tile/animation.hpp"

#include <gtest/gtest.h>

#include "tactile/core/entity/registry.hpp"
#include "tactile/core/tile/tile.hpp"

namespace tactile {

/**
 * \trace tactile::update_animations
 */
TEST(Animation, UpdateAnimations)
{
  Registry registry {};

  auto add_frame = [&registry](EntityID tile_entity,
                               const usize frame_index,
                               const TileIndex tile_index) {
    const AnimationFrame frame {tile_index, Milliseconds::zero()};
    return add_animation_frame(registry, tile_entity, frame_index, frame);
  };

  const auto tile1_entity = make_tile(registry, TileIndex {1});
  const auto tile2_entity = make_tile(registry, TileIndex {2});
  const auto tile3_entity = make_tile(registry, TileIndex {3});

  EXPECT_EQ(add_frame(tile1_entity, 0, TileIndex {10}), kOK);
  EXPECT_EQ(add_frame(tile1_entity, 1, TileIndex {11}), kOK);
  EXPECT_EQ(add_frame(tile1_entity, 2, TileIndex {12}), kOK);
  EXPECT_EQ(add_frame(tile2_entity, 0, TileIndex {20}), kOK);
  EXPECT_EQ(add_frame(tile2_entity, 1, TileIndex {21}), kOK);
  EXPECT_EQ(add_frame(tile3_entity, 0, TileIndex {30}), kOK);

  EXPECT_EQ(registry.get<CAnimation>(tile1_entity).frame_index, 0);
  EXPECT_EQ(registry.get<CAnimation>(tile2_entity).frame_index, 0);
  EXPECT_EQ(registry.get<CAnimation>(tile3_entity).frame_index, 0);

  update_animations(registry);

  EXPECT_EQ(registry.get<CAnimation>(tile1_entity).frame_index, 1);
  EXPECT_EQ(registry.get<CAnimation>(tile2_entity).frame_index, 1);
  EXPECT_EQ(registry.get<CAnimation>(tile3_entity).frame_index, 0);

  update_animations(registry);

  EXPECT_EQ(registry.get<CAnimation>(tile1_entity).frame_index, 2);
  EXPECT_EQ(registry.get<CAnimation>(tile2_entity).frame_index, 0);
  EXPECT_EQ(registry.get<CAnimation>(tile3_entity).frame_index, 0);

  update_animations(registry);

  EXPECT_EQ(registry.get<CAnimation>(tile1_entity).frame_index, 0);
  EXPECT_EQ(registry.get<CAnimation>(tile2_entity).frame_index, 1);
  EXPECT_EQ(registry.get<CAnimation>(tile3_entity).frame_index, 0);
}

/**
 * \trace tactile::add_animation_frame
 */
TEST(Animation, AddAnimationFrame)
{
  Registry registry {};
  const auto tile_entity = make_tile(registry, TileIndex {100});

  const AnimationFrame frame1 {TileIndex {110}, Milliseconds {123}};
  const AnimationFrame frame2 {TileIndex {120}, Milliseconds {321}};
  const AnimationFrame frame3 {TileIndex {130}, Milliseconds {987}};
  const AnimationFrame frame4 {TileIndex {140}, Milliseconds {593}};
  const AnimationFrame frame5 {TileIndex {150}, Milliseconds {739}};

  ASSERT_FALSE(registry.has<CAnimation>(tile_entity));

  // Bad frame index
  EXPECT_NE(add_animation_frame(registry, tile_entity, 1, frame1), kOK);
  EXPECT_FALSE(registry.has<CAnimation>(tile_entity));

  // Create animation
  EXPECT_EQ(add_animation_frame(registry, tile_entity, 0, frame1), kOK);
  EXPECT_TRUE(registry.has<CAnimation>(tile_entity));

  // Prepend
  EXPECT_EQ(add_animation_frame(registry, tile_entity, 0, frame2), kOK);

  // Insert
  EXPECT_EQ(add_animation_frame(registry, tile_entity, 1, frame3), kOK);

  // Append
  EXPECT_EQ(add_animation_frame(registry, tile_entity, 3, frame4), kOK);

  // Bad index
  EXPECT_NE(add_animation_frame(registry, tile_entity, 5, frame5), kOK);

  const auto& animation = registry.get<CAnimation>(tile_entity);

  // [ frame2, frame3, frame1, frame4 ]
  ASSERT_EQ(animation.frames.size(), 4);

  EXPECT_EQ(animation.frames[0].tile_index, frame2.tile_index);
  EXPECT_EQ(animation.frames[0].duration, frame2.duration);

  EXPECT_EQ(animation.frames[1].tile_index, frame3.tile_index);
  EXPECT_EQ(animation.frames[1].duration, frame3.duration);

  EXPECT_EQ(animation.frames[2].tile_index, frame1.tile_index);
  EXPECT_EQ(animation.frames[2].duration, frame1.duration);

  EXPECT_EQ(animation.frames[3].tile_index, frame4.tile_index);
  EXPECT_EQ(animation.frames[3].duration, frame4.duration);
}

/**
 * \trace tactile::remove_animation_frame
 */
TEST(Animation, RemoveAnimationFrame)
{
  Registry registry {};
  const auto tile_entity = make_tile(registry, TileIndex {10});

  const AnimationFrame frame1 {TileIndex {1}, Milliseconds {10}};
  const AnimationFrame frame2 {TileIndex {2}, Milliseconds {20}};
  const AnimationFrame frame3 {TileIndex {3}, Milliseconds {30}};

  ASSERT_EQ(add_animation_frame(registry, tile_entity, 0, frame1), kOK);
  ASSERT_EQ(add_animation_frame(registry, tile_entity, 1, frame2), kOK);
  ASSERT_EQ(add_animation_frame(registry, tile_entity, 2, frame3), kOK);

  const auto& animation = registry.get<CAnimation>(tile_entity);

  // [ frame1, frame2, frame3 ]
  ASSERT_EQ(animation.frames.size(), 3);
  EXPECT_EQ(animation.frames[0].tile_index, frame1.tile_index);
  EXPECT_EQ(animation.frames[0].duration, frame1.duration);
  EXPECT_EQ(animation.frames[1].tile_index, frame2.tile_index);
  EXPECT_EQ(animation.frames[1].duration, frame2.duration);
  EXPECT_EQ(animation.frames[2].tile_index, frame3.tile_index);
  EXPECT_EQ(animation.frames[2].duration, frame3.duration);

  // => [ frame1, frame2 ]
  EXPECT_EQ(remove_animation_frame(registry, tile_entity, 2), kOK);
  ASSERT_EQ(animation.frames.size(), 2);
  EXPECT_EQ(animation.frames[0].tile_index, frame1.tile_index);
  EXPECT_EQ(animation.frames[0].duration, frame1.duration);
  EXPECT_EQ(animation.frames[1].tile_index, frame2.tile_index);
  EXPECT_EQ(animation.frames[1].duration, frame2.duration);

  // => [ frame2 ]
  EXPECT_EQ(remove_animation_frame(registry, tile_entity, 0), kOK);
  ASSERT_EQ(animation.frames.size(), 1);
  EXPECT_EQ(animation.frames[0].tile_index, frame2.tile_index);
  EXPECT_EQ(animation.frames[0].duration, frame2.duration);

  // => [ ]
  EXPECT_TRUE(registry.has<CAnimation>(tile_entity));
  EXPECT_EQ(remove_animation_frame(registry, tile_entity, 0), kOK);
  EXPECT_FALSE(registry.has<CAnimation>(tile_entity));
}

}  // namespace tactile
