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

#include "core/tilesets/tileset_bundle.hpp"

#include <gtest/gtest.h>

#include "core/tilesets/tileset_info.hpp"
#include "misc/panic.hpp"

using namespace tactile;
using namespace core;

namespace {

[[nodiscard]] auto _make_tileset() -> Shared<Tileset>
{
  return std::make_shared<Tileset>(TilesetInfo{.texture_path = "foo.png",
                                               .texture_id = 42,
                                               .texture_size = {1024, 1024},
                                               .tile_size = {32, 32}});
}

}  // namespace

TEST(TilesetBundle, Defaults)
{
  const TilesetBundle bundle;
  ASSERT_EQ(1, bundle.next_tile_id());
  ASSERT_EQ(0, bundle.size());
  ASSERT_TRUE(bundle.empty());
  ASSERT_FALSE(bundle.active_tileset_id().has_value());
}

TEST(TilesetBundle, AttachTilesetWithExplicitFirstTileId)
{
  const TileID firstTile = 123;

  TilesetBundle bundle;

  auto tileset = _make_tileset();
  bundle.attach_tileset(tileset, firstTile, false);

  ASSERT_EQ(1, bundle.size());
  ASSERT_EQ(1, bundle.next_tile_id());
  ASSERT_FALSE(bundle.empty());

  ASSERT_TRUE(bundle.is_valid_tile(firstTile));
  ASSERT_TRUE(bundle.is_valid_tile(firstTile + 42));
  ASSERT_TRUE(bundle.is_valid_tile(firstTile + tileset->tile_count()));

  ASSERT_FALSE(bundle.is_valid_tile(firstTile - 1));
  ASSERT_FALSE(bundle.is_valid_tile(firstTile + tileset->tile_count() + 1));

  const auto& ref = bundle.get_ref(tileset->get_uuid());
  ASSERT_EQ(tileset->tile_count(), ref.last_tile - ref.first_tile);
  ASSERT_EQ(firstTile, ref.first_tile);
  ASSERT_EQ(firstTile + tileset->tile_count(), ref.last_tile);
}

TEST(TilesetBundle, AttachTileset)
{
  TilesetBundle bundle;

  const auto firstTile = bundle.next_tile_id();

  auto tileset = _make_tileset();
  bundle.attach_tileset(tileset, false);

  ASSERT_EQ(1, bundle.size());
  ASSERT_EQ(firstTile + tileset->tile_count() + 1, bundle.next_tile_id());
  ASSERT_FALSE(bundle.empty());

  ASSERT_TRUE(bundle.is_valid_tile(firstTile));
  ASSERT_TRUE(bundle.is_valid_tile(firstTile + 39));
  ASSERT_TRUE(bundle.is_valid_tile(firstTile + tileset->tile_count()));

  ASSERT_FALSE(bundle.is_valid_tile(firstTile - 1));
  ASSERT_FALSE(bundle.is_valid_tile(firstTile + tileset->tile_count() + 1));

  const auto& ref = bundle.get_ref(tileset->get_uuid());
  ASSERT_EQ(tileset->tile_count(), ref.last_tile - ref.first_tile);
  ASSERT_EQ(firstTile, ref.first_tile);
  ASSERT_EQ(firstTile + tileset->tile_count(), ref.last_tile);
}

TEST(TilesetBundle, DetachTileset)
{
  TilesetBundle bundle;

  auto       tileset = _make_tileset();
  const auto tilesetId = tileset->get_uuid();

  bundle.attach_tileset(tileset, false);

  ASSERT_EQ(1, bundle.size());
  ASSERT_TRUE(bundle.has_tileset(tilesetId));

  ASSERT_TRUE(bundle.is_valid_tile(1));
  ASSERT_TRUE(bundle.is_valid_tile(42));

  bundle.detach_tileset(tilesetId);

  ASSERT_EQ(0, bundle.size());
  ASSERT_FALSE(bundle.has_tileset(tilesetId));

  ASSERT_FALSE(bundle.is_valid_tile(1));
  ASSERT_FALSE(bundle.is_valid_tile(42));

  ASSERT_THROW(bundle.detach_tileset(tilesetId), TactileError);
}

TEST(TilesetBundle, SelectTileset)
{
  TilesetBundle bundle;

  const auto a = _make_tileset();
  const auto b = _make_tileset();

  bundle.attach_tileset(a, false);
  bundle.attach_tileset(b, false);

  ASSERT_FALSE(bundle.active_tileset_id().has_value());

  bundle.select_tileset(a->get_uuid());
  ASSERT_EQ(a->get_uuid(), bundle.active_tileset_id());

  bundle.select_tileset(b->get_uuid());
  ASSERT_EQ(b->get_uuid(), bundle.active_tileset_id());

  bundle.detach_tileset(b->get_uuid());
  ASSERT_EQ(a->get_uuid(), bundle.active_tileset_id());

  bundle.detach_tileset(a->get_uuid());
  ASSERT_FALSE(bundle.active_tileset_id());
}

TEST(TilesetBundle, HasTileset)
{
  TilesetBundle bundle;

  const auto tileset = _make_tileset();
  ASSERT_FALSE(bundle.has_tileset(tileset->get_uuid()));

  bundle.attach_tileset(tileset, true);
  ASSERT_TRUE(bundle.has_tileset(tileset->get_uuid()));
}

TEST(TilesetBundle, GetRef)
{
  TilesetBundle bundle;

  const auto a = _make_tileset();
  const auto b = _make_tileset();

  bundle.attach_tileset(a, false);
  bundle.attach_tileset(b, true);

  const auto& aa = bundle.get_ref(a->get_uuid());
  const auto& bb = bundle.get_ref(b->get_uuid());

  ASSERT_EQ(1, aa.first_tile);
  ASSERT_EQ(aa.last_tile + 1, bb.first_tile);

  ASSERT_EQ(a, aa.tileset);
  ASSERT_EQ(b, bb.tileset);

  ASSERT_FALSE(aa.selection.has_value());
  ASSERT_FALSE(bb.selection.has_value());

  ASSERT_FALSE(aa.embedded);
  ASSERT_TRUE(bb.embedded);
}