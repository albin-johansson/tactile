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

#include <doctest/doctest.h>

#include "core/debug/panic.hpp"
#include "core/tile/tile.hpp"
#include "core/tile/tileset_info.hpp"
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

TEST_SUITE("Tileset")
{
  TEST_CASE("Defaults")
  {
    const auto tileset = make_tileset();
    REQUIRE(expected_tile_size == tileset.tile_size());
    REQUIRE(expected_texture_size == tileset.texture().get_size());
    REQUIRE(texture_path == tileset.texture().get_path());
  }

  TEST_CASE("operator[]")
  {
    auto tileset = make_tileset();

    REQUIRE_THROWS_AS(tileset[-1], TactileError);
    REQUIRE_THROWS_AS(tileset[tileset.tile_count()], TactileError);

    const auto& tile = tileset[42];
    REQUIRE(42 == tile.get_index());
  }

  TEST_CASE("SelectTile")
  {
    auto tileset = make_tileset();

    REQUIRE(!tileset.get_selected_tile().has_value());

    tileset.select_tile(42);
    REQUIRE(42 == tileset.get_selected_tile());

    REQUIRE_THROWS_AS(tileset.select_tile(-1), TactileError);
    REQUIRE_THROWS_AS(tileset.select_tile(tileset.tile_count()), TactileError);
    REQUIRE_NOTHROW(tileset.select_tile(tileset.tile_count() - 1));
  }

  TEST_CASE("index_of")
  {
    auto tileset = make_tileset();

    REQUIRE_THROWS_AS((void) tileset.index_of({-1, -1}), TactileError);
    REQUIRE_THROWS_AS(
        (void) tileset.index_of({tileset.row_count(), tileset.column_count()}),
        TactileError);

    REQUIRE(0 == tileset.index_of({0, 0}));
    REQUIRE(tileset.tile_count() - 1 ==
            tileset.index_of({tileset.row_count() - 1, tileset.column_count() - 1}));
  }

  TEST_CASE("tile_count")
  {
    const auto tileset = make_tileset();
    REQUIRE(tileset.row_count() * tileset.column_count() == tileset.tile_count());
  }
}

}  // namespace tactile::test
