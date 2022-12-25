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

#include "core/tile/tileset.hpp"

#include <gtest/gtest.h>

#include "core/tile/tile.hpp"
#include "core/tile/tileset_info.hpp"
#include "core/debug/panic.hpp"
#include "io/load_texture.hpp"

namespace tactile::test {
namespace {

constexpr const char* texture_path = "resources/terrain.png";
constexpr Int2 expected_texture_size = {1024, 1024};
constexpr Int2 expected_tile_size = {32, 32};

[[nodiscard]] auto make_tileset() -> Tileset
{
  return Tileset {TilesetInfo {
      .texture = io::load_texture(texture_path),
      .tile_size = expected_tile_size,
  }};
}

}  // namespace

TEST(Tileset, Defaults)
{
  const auto tileset = make_tileset();
  ASSERT_EQ(expected_tile_size, tileset.tile_size());
  ASSERT_EQ(expected_texture_size, tileset.texture().get_size());
  ASSERT_EQ(texture_path, tileset.texture().get_path());
}

TEST(Tileset, SubscriptOperator)
{
  auto tileset = make_tileset();

  ASSERT_THROW(tileset[-1], TactileError);
  ASSERT_THROW(tileset[tileset.tile_count()], TactileError);

  const auto& tile = tileset[42];
  ASSERT_EQ(42, tile.get_index());
}

TEST(Tileset, IndexOf)
{
  auto tileset = make_tileset();

  ASSERT_THROW((void) tileset.index_of({-1, -1}), TactileError);
  ASSERT_THROW((void) tileset.index_of({tileset.row_count(), tileset.column_count()}),
               TactileError);

  ASSERT_EQ(0, tileset.index_of({0, 0}));
  ASSERT_EQ(tileset.tile_count() - 1,
            tileset.index_of({tileset.row_count() - 1, tileset.column_count() - 1}));
}

TEST(Tileset, TileCount)
{
  const auto tileset = make_tileset();
  ASSERT_EQ(tileset.row_count() * tileset.column_count(), tileset.tile_count());
}

}  // namespace tactile::test
