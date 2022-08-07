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
