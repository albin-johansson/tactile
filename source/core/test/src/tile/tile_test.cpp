// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/tile/tile.hpp"

#include <gtest/gtest.h>

#include "tactile/core/entity/registry.hpp"
#include "tactile/core/layer/object.hpp"
#include "tactile/core/meta/meta.hpp"

namespace tactile {

class TileTest : public testing::Test
{
 protected:
  Registry mRegistry {};
};

/// \trace tactile::make_tile
/// \trace tactile::is_tile
TEST_F(TileTest, MakeTile)
{
  const auto tile_entity = make_tile(mRegistry, TileIndex {42});

  EXPECT_TRUE(is_tile(mRegistry, tile_entity));
  ASSERT_TRUE(mRegistry.has<CMeta>(tile_entity));
  ASSERT_TRUE(mRegistry.has<CTile>(tile_entity));

  const auto& meta = mRegistry.get<CMeta>(tile_entity);
  const auto& tile = mRegistry.get<CTile>(tile_entity);

  EXPECT_EQ(meta.name, "");
  EXPECT_EQ(meta.properties.size(), 0);
  EXPECT_EQ(meta.components.size(), 0);

  EXPECT_EQ(tile.index, TileIndex {42});
  EXPECT_EQ(tile.objects.size(), 0);
}

/// \trace tactile::destroy_tile
TEST_F(TileTest, DestroyTile)
{
  const auto tile_entity = make_tile(mRegistry, TileIndex {10});
  ASSERT_TRUE(mRegistry.is_valid(tile_entity));

  {
    auto& tile = mRegistry.get<CTile>(tile_entity);
    tile.objects.push_back(
        make_object(mRegistry, ObjectID {1}, ObjectType::kRect));
    tile.objects.push_back(
        make_object(mRegistry, ObjectID {2}, ObjectType::kEllipse));
    tile.objects.push_back(
        make_object(mRegistry, ObjectID {3}, ObjectType::kPoint));
  }

  //   Tiles: 1 * (1 CMeta + 1 CTile)
  // Objects: 3 * (1 CMeta + 1 CObject)
  EXPECT_EQ(mRegistry.count(), 8);
  EXPECT_EQ(mRegistry.count<CTile>(), 1);
  EXPECT_EQ(mRegistry.count<CMeta>(), 4);
  EXPECT_EQ(mRegistry.count<CObject>(), 3);

  destroy_tile(mRegistry, tile_entity);

  EXPECT_FALSE(mRegistry.is_valid(tile_entity));
  EXPECT_FALSE(is_tile(mRegistry, tile_entity));
  EXPECT_EQ(mRegistry.count(), 0);
  EXPECT_EQ(mRegistry.count<CTile>(), 0);
  EXPECT_EQ(mRegistry.count<CObject>(), 0);
}

/// \trace tactile::copy_tile
TEST_F(TileTest, CopyTile)
{
  const auto e1 = make_tile(mRegistry, TileIndex {35});

  auto& meta1 = mRegistry.get<CMeta>(e1);
  meta1.name = "abcdef";
  meta1.properties["x"] = Attribute {"y"};
  meta1.components[UUID::generate()];

  auto& tile1 = mRegistry.get<CTile>(e1);
  tile1.objects.push_back(
      make_object(mRegistry, ObjectID {1}, ObjectType::kPoint));
  tile1.objects.push_back(
      make_object(mRegistry, ObjectID {2}, ObjectType::kRect));
  tile1.objects.push_back(
      make_object(mRegistry, ObjectID {3}, ObjectType::kEllipse));

  const auto e2 = copy_tile(mRegistry, e1);
  EXPECT_TRUE(is_tile(mRegistry, e2));

  const auto& meta2 = mRegistry.get<CMeta>(e2);
  EXPECT_EQ(meta2.name, meta1.name);
  EXPECT_EQ(meta2.properties, meta1.properties);
  EXPECT_EQ(meta2.components, meta1.components);

  const auto& tile2 = mRegistry.get<CTile>(e2);
  EXPECT_EQ(tile2.index, tile1.index);
  EXPECT_EQ(tile2.objects.size(), tile1.objects.size());
  EXPECT_NE(tile2.objects, tile1.objects);
}

}  // namespace tactile
