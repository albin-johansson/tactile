// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/tileset/tile.hpp"

#include <gtest/gtest.h>

using namespace tactile;
using int_literals::operator""_uz;

TEST(Tile, Defaults)
{
  const Rectangle region {.position = {0, 0}, .size = {10, 10}};
  const Tile tile {region};
  EXPECT_EQ(tile.get_region(), region);
  EXPECT_FALSE(tile.is_animated());
}

/// \tests tactile::Tile::set_animation
/// \tests tactile::Tile::get_animation
/// \tests tactile::Tile::is_animated
TEST(Tile, SetAnimation)
{
  const Rectangle region {.position = {0, 0}, .size = {10, 10}};
  Tile tile {region};

  TileAnimation animation;
  animation.append_frame(TileAnimationFrame {TileIndex {10}, Milliseconds {123}});
  animation.append_frame(TileAnimationFrame {TileIndex {20}, Milliseconds {456}});

  tile.set_animation(animation);
  ASSERT_TRUE(tile.is_animated());

  EXPECT_EQ(tile.get_animation().frame_count(), animation.frame_count());
  EXPECT_EQ(tile.get_animation().get_frame(0_uz), animation.get_frame(0_uz));
  EXPECT_EQ(tile.get_animation().get_frame(1_uz), animation.get_frame(1_uz));

  tile.set_animation(kNothing);
  EXPECT_FALSE(tile.is_animated());

  const auto& const_tile = tile;
  EXPECT_ANY_THROW((void) tile.get_animation());
  EXPECT_ANY_THROW((void) const_tile.get_animation());
}

/// \tests tactile::Tile::get_region
TEST(Tile, GetRegion)
{
  const Rectangle region {.position = {43, 83}, .size = {134, 74}};
  const Tile tile {region};
  EXPECT_EQ(tile.get_region(), region);
}
