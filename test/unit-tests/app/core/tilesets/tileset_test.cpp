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

#include "core/tilesets/tileset.hpp"

#include <gtest/gtest.h>

#include "core/tilesets/tileset_info.hpp"
#include "misc/panic.hpp"

using namespace tactile;

namespace {

constexpr const char* _default_texture_path = "foo/bar.png";
constexpr uint        _default_texture_id = 7;
constexpr Vector2i    _default_texture_size = {1024, 768};
constexpr Vector2i    _default_tile_size = {28, 42};

[[nodiscard]] auto _make_tileset() -> Tileset
{
  return Tileset{{
      .texture_path = _default_texture_path,
      .texture_id = _default_texture_id,
      .texture_size = _default_texture_size,
      .tile_size = _default_tile_size,
  }};
}

}  // namespace

TEST(Tileset, Defaults)
{
  const auto tileset = _make_tileset();
  ASSERT_EQ(_default_texture_path, tileset.texture_path());
  ASSERT_EQ(_default_texture_id, tileset.texture_id());
  ASSERT_EQ(_default_tile_size, tileset.tile_size());
  ASSERT_EQ(_default_texture_size, tileset.texture_size());
}

TEST(Tileset, SubscriptOperator)
{
  auto tileset = _make_tileset();

  ASSERT_THROW(tileset[-1], TactileError);
  ASSERT_THROW(tileset[tileset.tile_count()], TactileError);

  const auto& tile = tileset[42];
  ASSERT_EQ(42, tile.index());
}

TEST(Tileset, IndexOf)
{
  auto tileset = _make_tileset();

  ASSERT_THROW(tileset.index_of({-1, -1}), TactileError);
  ASSERT_THROW(tileset.index_of({tileset.row_count(), tileset.column_count()}),
               TactileError);

  ASSERT_EQ(0, tileset.index_of({0, 0}));
  ASSERT_EQ(tileset.tile_count() - 1,
            tileset.index_of({tileset.row_count() - 1, tileset.column_count() - 1}));
}

TEST(Tileset, TileCount)
{
  const auto tileset = _make_tileset();
  ASSERT_EQ(tileset.row_count() * tileset.column_count(), tileset.tile_count());
}
