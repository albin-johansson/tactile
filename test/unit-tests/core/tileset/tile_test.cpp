// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/tileset/tile.hpp"

#include <utility>  // move

#include <gtest/gtest.h>

namespace tactile::test {

using Millis = TileAnimation::Millis;

TEST(Tile, Defaults)
{
  const Tile tile {7};
  ASSERT_EQ(7, tile.index());
  ASSERT_EQ(0, tile.object_count());
  ASSERT_FALSE(tile.is_animated());
}

TEST(Tile, AddObject)
{
  Tile tile {42};

  auto object = std::make_shared<Object>();
  tile.add_object(object);

  ASSERT_EQ(1, tile.object_count());
}

TEST(Tile, ClearAnimation)
{
  Tile tile {42};

  tile.set_animation(TileAnimation {});
  ASSERT_TRUE(tile.is_animated());

  tile.clear_animation();
  ASSERT_FALSE(tile.is_animated());
}

TEST(Tile, SetAnimation)
{
  Tile tile {42};

  {
    TileAnimation animation;
    animation.add_frame(7, Millis {42});
    animation.add_frame(8, Millis {37});
    tile.set_animation(std::move(animation));
  }

  ASSERT_TRUE(tile.is_animated());
  const auto& animation = tile.get_animation();

  ASSERT_EQ(2, animation.size());

  ASSERT_EQ(7, animation.nth(0).tile);
  ASSERT_EQ(Millis {42}, animation.nth(0).duration);

  ASSERT_EQ(8, animation.nth(1).tile);
  ASSERT_EQ(Millis {37}, animation.nth(1).duration);
}

}  // namespace tactile::test
