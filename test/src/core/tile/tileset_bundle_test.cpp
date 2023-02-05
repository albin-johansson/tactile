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

#include "core/tile/tileset_bundle.hpp"

#include <doctest/doctest.h>

#include "common/debug/panic.hpp"
#include "core/tile/tileset_info.hpp"
#include "io/load_texture.hpp"

namespace tactile::test {
namespace {

[[nodiscard]] auto make_tileset() -> Shared<Tileset>
{
  return std::make_shared<Tileset>(
      TilesetInfo {.texture = load_texture("resources/terrain.png"),
                   .tile_size = {32, 32}});
}

}  // namespace

TEST_SUITE("TilesetBundle")
{
  TEST_CASE("Defaults")
  {
    const TilesetBundle bundle;
    REQUIRE(1 == bundle.get_next_tile_id());
    REQUIRE(0u == bundle.size());
    REQUIRE(bundle.empty());
    REQUIRE(!bundle.get_active_tileset_id().has_value());
  }

  TEST_CASE("attach_tileset (with explicit first tile ID)")
  {
    const TileID first_tile = 123;

    TilesetBundle bundle;

    auto tileset = make_tileset();
    bundle.attach_tileset(tileset, first_tile, false);

    REQUIRE(1u == bundle.size());
    REQUIRE(1 == bundle.get_next_tile_id());
    REQUIRE(!bundle.empty());

    REQUIRE(bundle.is_valid_tile(first_tile));
    REQUIRE(bundle.is_valid_tile(first_tile + 42));
    REQUIRE(bundle.is_valid_tile(first_tile + tileset->tile_count()));

    REQUIRE(!bundle.is_valid_tile(first_tile - 1));
    REQUIRE(!bundle.is_valid_tile(first_tile + tileset->tile_count() + 1));

    const auto& ref = bundle.get_tileset_ref(tileset->get_uuid());
    REQUIRE(tileset->tile_count() == ref.get_last_tile() - ref.get_first_tile());
    REQUIRE(first_tile == ref.get_first_tile());
    REQUIRE(first_tile + tileset->tile_count() == ref.get_last_tile());
  }

  TEST_CASE("attach_tileset")
  {
    TilesetBundle bundle;

    const auto first_tile = bundle.get_next_tile_id();

    auto tileset = make_tileset();
    bundle.attach_tileset(tileset, false);

    REQUIRE(1u == bundle.size());
    REQUIRE(first_tile + tileset->tile_count() + 1 == bundle.get_next_tile_id());
    REQUIRE(!bundle.empty());

    REQUIRE(bundle.is_valid_tile(first_tile));
    REQUIRE(bundle.is_valid_tile(first_tile + 39));
    REQUIRE(bundle.is_valid_tile(first_tile + tileset->tile_count()));

    REQUIRE(!bundle.is_valid_tile(first_tile - 1));
    REQUIRE(!bundle.is_valid_tile(first_tile + tileset->tile_count() + 1));

    const auto& ref = bundle.get_tileset_ref(tileset->get_uuid());
    REQUIRE(tileset->tile_count() == ref.get_last_tile() - ref.get_first_tile());
    REQUIRE(first_tile == ref.get_first_tile());
    REQUIRE(first_tile + tileset->tile_count() == ref.get_last_tile());
  }

  TEST_CASE("detach_tileset")
  {
    TilesetBundle bundle;

    auto tileset = make_tileset();
    const auto tileset_id = tileset->get_uuid();

    bundle.attach_tileset(tileset, false);

    REQUIRE(1u == bundle.size());
    REQUIRE(bundle.has_tileset(tileset_id));

    REQUIRE(bundle.is_valid_tile(1));
    REQUIRE(bundle.is_valid_tile(42));

    bundle.detach_tileset(tileset_id);

    REQUIRE(0u == bundle.size());
    REQUIRE(!bundle.has_tileset(tileset_id));

    REQUIRE(!bundle.is_valid_tile(1));
    REQUIRE(!bundle.is_valid_tile(42));

    REQUIRE_THROWS_AS(bundle.detach_tileset(tileset_id), TactileError);
  }

  TEST_CASE("select_tileset")
  {
    TilesetBundle bundle;

    const auto a = make_tileset();
    const auto b = make_tileset();

    bundle.attach_tileset(a, false);
    bundle.attach_tileset(b, false);

    REQUIRE(!bundle.get_active_tileset_id().has_value());

    bundle.select_tileset(a->get_uuid());
    REQUIRE(a->get_uuid() == bundle.get_active_tileset_id());

    bundle.select_tileset(b->get_uuid());
    REQUIRE(b->get_uuid() == bundle.get_active_tileset_id());

    bundle.detach_tileset(b->get_uuid());
    REQUIRE(a->get_uuid() == bundle.get_active_tileset_id());

    bundle.detach_tileset(a->get_uuid());
    REQUIRE(!bundle.get_active_tileset_id());
  }

  TEST_CASE("has_tileset")
  {
    TilesetBundle bundle;

    const auto tileset = make_tileset();
    REQUIRE(!bundle.has_tileset(tileset->get_uuid()));

    bundle.attach_tileset(tileset, true);
    REQUIRE(bundle.has_tileset(tileset->get_uuid()));
  }

  TEST_CASE("get_tileset_ref")
  {
    TilesetBundle bundle;

    const auto a = make_tileset();
    const auto b = make_tileset();

    bundle.attach_tileset(a, false);
    bundle.attach_tileset(b, true);

    const auto& aa = bundle.get_tileset_ref(a->get_uuid());
    const auto& bb = bundle.get_tileset_ref(b->get_uuid());

    REQUIRE(1 == aa.get_first_tile());
    REQUIRE(aa.get_last_tile() + 1 == bb.get_first_tile());

    REQUIRE(a->get_uuid() == aa.get_tileset().get_uuid());
    REQUIRE(b->get_uuid() == bb.get_tileset().get_uuid());

    REQUIRE(!aa.get_selection().has_value());
    REQUIRE(!bb.get_selection().has_value());

    REQUIRE(!aa.is_embedded());
    REQUIRE(bb.is_embedded());
  }

  TEST_CASE("to_local_index")
  {
    TilesetBundle bundle;

    const auto a = make_tileset();
    const auto b = make_tileset();

    bundle.attach_tileset(a, false);
    bundle.attach_tileset(b, false);

    auto& ref_a = bundle.get_tileset_ref(a->get_uuid());
    auto& ref_b = bundle.get_tileset_ref(b->get_uuid());

    REQUIRE(1 == ref_a.get_first_tile());
    REQUIRE(1 + a->tile_count() == ref_a.get_last_tile());

    REQUIRE(ref_a.get_last_tile() + 1 == ref_b.get_first_tile());
    REQUIRE(ref_b.get_first_tile() + b->tile_count() == ref_b.get_last_tile());

    REQUIRE(0 == bundle.to_tile_index(ref_a.get_first_tile()));
    REQUIRE(a->tile_count() == bundle.to_tile_index(ref_a.get_last_tile()));

    REQUIRE(0 == bundle.to_tile_index(ref_b.get_first_tile()));
    REQUIRE(b->tile_count() == bundle.to_tile_index(ref_b.get_last_tile()));

    REQUIRE_THROWS_AS(bundle.to_tile_index(ref_a.get_first_tile() - 1), TactileError);
    REQUIRE_THROWS_AS(bundle.to_tile_index(ref_b.get_last_tile() + 1), TactileError);
  }
}

}  // namespace tactile::test
