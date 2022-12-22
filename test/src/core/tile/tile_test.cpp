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

#include "core/tile/tile.hpp"

#include <utility>  // move

#include <gtest/gtest.h>

using namespace std::chrono_literals;

namespace tactile::test {

TEST(Tile, Defaults)
{
  const Tile tile {7};
  ASSERT_EQ(7, tile.get_index());
  ASSERT_EQ(0u, tile.object_count());
  ASSERT_EQ(Int4 {}, tile.get_source());
  ASSERT_FALSE(tile.is_animated());
}

TEST(Tile, AddObject)
{
  Tile tile {42};

  auto object = std::make_shared<Object>();
  tile.add_object(object);

  ASSERT_EQ(1u, tile.object_count());
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
    animation.add_frame(7, 42ms);
    animation.add_frame(8, 37ms);
    tile.set_animation(std::move(animation));
  }

  ASSERT_TRUE(tile.is_animated());
  const auto& animation = tile.get_animation();

  ASSERT_EQ(2u, animation.size());

  ASSERT_EQ(7, animation[0].tile);
  ASSERT_EQ(42ms, animation[0].duration);

  ASSERT_EQ(8, animation[1].tile);
  ASSERT_EQ(37ms, animation[1].duration);
}

TEST(Tile, SetSource)
{
  Tile tile {100};

  const Int4 source {27, 82, 45, 32};
  tile.set_source(source);

  ASSERT_EQ(source, tile.get_source());
}

}  // namespace tactile::test
