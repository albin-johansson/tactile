// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/tileset/attached_tileset.hpp"

#include <gtest/gtest.h>

#include "tactile/core/debug/error.hpp"
#include "testutil/tileset_helpers.hpp"

using namespace tactile;

/// \tests tactile::AttachedTileset::AttachedTileset
TEST(AttachedTileset, ConstructorWithNullTileset)
{
  EXPECT_THROW(AttachedTileset(nullptr, TileID {1}), RuntimeError);
}

/// \tests tactile::AttachedTileset::to_index
TEST(AttachedTileset, ToIndex)
{
  const auto tileset_info = test::make_dummy_tileset_info();
  const auto tileset = make_shared<Tileset>(tileset_info);

  const AttachedTileset attached_tileset {tileset, TileID {42}};
  const auto first_id = attached_tileset.get_first_tile_id();
  const auto last_id = attached_tileset.get_last_tile_id();

  EXPECT_EQ(attached_tileset.to_index(first_id), TileIndex {0});
  EXPECT_EQ(attached_tileset.to_index(last_id), TileIndex {tileset->tile_count() - 1});
}

/// \tests tactile::AttachedTileset::has_tile
TEST(AttachedTileset, HasTile)
{
  const auto tileset_info = test::make_dummy_tileset_info();
  const auto tileset = make_shared<Tileset>(tileset_info);

  const AttachedTileset attached_tileset {tileset, TileID {1}};
  const auto first_id = attached_tileset.get_first_tile_id();
  const auto last_id = attached_tileset.get_last_tile_id();

  EXPECT_TRUE(attached_tileset.has_tile(first_id));
  EXPECT_TRUE(attached_tileset.has_tile(last_id));

  EXPECT_FALSE(attached_tileset.has_tile(TileID {first_id.value - 1}));
  EXPECT_FALSE(attached_tileset.has_tile(TileID {last_id.value + 1}));
}

/// \tests tactile::AttachedTileset::get_first_tile_id
/// \tests tactile::AttachedTileset::get_last_tile_id
TEST(AttachedTileset, TileIdentifiers)
{
  const auto tileset_info = test::make_dummy_tileset_info();
  const auto tileset = make_shared<Tileset>(tileset_info);
  const AttachedTileset attached_tileset {tileset, TileID {27}};

  EXPECT_EQ(attached_tileset.get_first_tile_id(), TileID {27});
  EXPECT_EQ(attached_tileset.get_last_tile_id(), TileID {27 + tileset->tile_count() - 1});
}
