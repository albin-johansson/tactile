// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/tile/tile.hpp"

#include <gtest/gtest.h>

#include "tactile/core/entity/registry.hpp"
#include "tactile/core/layer/object.hpp"
#include "tactile/core/meta/meta.hpp"

namespace tactile {

/**
 * \trace tactile::make_tile
 * \trace tactile::is_tile
 */
TEST(Tile, MakeTile)
{
  Registry registry {};
  const auto tile_entity = make_tile(registry, TileIndex {42});

  EXPECT_TRUE(is_tile(registry, tile_entity));
  ASSERT_TRUE(registry.has<CMeta>(tile_entity));
  ASSERT_TRUE(registry.has<CTile>(tile_entity));

  const auto& meta = registry.get<CMeta>(tile_entity);
  const auto& tile = registry.get<CTile>(tile_entity);

  EXPECT_EQ(meta.name, "");
  EXPECT_EQ(meta.properties.size(), 0);
  EXPECT_EQ(meta.components.size(), 0);

  EXPECT_EQ(tile.index, TileIndex {42});
  EXPECT_EQ(tile.objects.size(), 0);
}

/**
 * \trace tactile::destroy_tile
 */
TEST(Tile, DestroyTile)
{
  Registry registry {};

  const auto tile_entity = make_tile(registry, TileIndex {10});
  ASSERT_TRUE(registry.is_valid(tile_entity));

  {
    auto& tile = registry.get<CTile>(tile_entity);
    tile.objects.push_back(
        make_object(registry, ObjectID {1}, ObjectType::kRect));
    tile.objects.push_back(
        make_object(registry, ObjectID {2}, ObjectType::kEllipse));
    tile.objects.push_back(
        make_object(registry, ObjectID {3}, ObjectType::kPoint));
  }

  //   Tiles: 1 * (1 CMeta + 1 CTile)
  // Objects: 3 * (1 CMeta + 1 CObject)
  EXPECT_EQ(registry.count(), 8);
  EXPECT_EQ(registry.count<CTile>(), 1);
  EXPECT_EQ(registry.count<CMeta>(), 4);
  EXPECT_EQ(registry.count<CObject>(), 3);

  destroy_tile(registry, tile_entity);

  EXPECT_FALSE(registry.is_valid(tile_entity));
  EXPECT_FALSE(is_tile(registry, tile_entity));
  EXPECT_EQ(registry.count(), 0);
  EXPECT_EQ(registry.count<CTile>(), 0);
  EXPECT_EQ(registry.count<CObject>(), 0);
}

}  // namespace tactile
