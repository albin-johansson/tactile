// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/group_layer.hpp"

#include <gtest/gtest.h>

#include "tactile/core/entity/registry.hpp"
#include "tactile/core/layer/layer.hpp"
#include "tactile/core/layer/object_layer.hpp"
#include "tactile/core/layer/tile_layer.hpp"
#include "tactile/core/meta/meta.hpp"

namespace tactile {

class GroupLayerTest : public testing::Test
{
 protected:
  Registry mRegistry {};
};

/// \trace tactile::is_group_layer
TEST_F(GroupLayerTest, IsGroupLayer)
{
  EXPECT_FALSE(is_group_layer(mRegistry, kInvalidEntity));
  EXPECT_FALSE(is_group_layer(mRegistry, mRegistry.make_entity()));

  const auto group_layer_entity = make_group_layer(mRegistry);
  EXPECT_TRUE(is_group_layer(mRegistry, group_layer_entity));
}

/// \trace tactile::make_group_layer
TEST_F(GroupLayerTest, MakeGroupLayer)
{
  const auto group_layer_entity = make_group_layer(mRegistry);

  EXPECT_TRUE(is_group_layer(mRegistry, group_layer_entity));
  EXPECT_TRUE(mRegistry.has<CMeta>(group_layer_entity));
  EXPECT_TRUE(mRegistry.has<CLayer>(group_layer_entity));
  EXPECT_TRUE(mRegistry.has<CGroupLayer>(group_layer_entity));

  const auto& meta = mRegistry.get<CMeta>(group_layer_entity);
  const auto& layer = mRegistry.get<CLayer>(group_layer_entity);
  const auto& group_layer = mRegistry.get<CGroupLayer>(group_layer_entity);

  EXPECT_EQ(meta.name, "");
  EXPECT_EQ(meta.properties.size(), 0);
  EXPECT_EQ(meta.components.size(), 0);

  EXPECT_EQ(layer.persistent_id, kNone);
  EXPECT_EQ(layer.opacity, 1.0f);
  EXPECT_TRUE(layer.visible);

  EXPECT_EQ(group_layer.layers.size(), 0);
}

/// \trace tactile::destroy_group_layer
TEST_F(GroupLayerTest, DestroyEmptyGroupLayer)
{
  const auto group_layer_entity = make_group_layer(mRegistry);

  EXPECT_TRUE(mRegistry.is_valid(group_layer_entity));
  EXPECT_EQ(mRegistry.count<CMeta>(), 1);
  EXPECT_EQ(mRegistry.count<CLayer>(), 1);
  EXPECT_EQ(mRegistry.count<CGroupLayer>(), 1);
  EXPECT_EQ(mRegistry.count(), 3);

  destroy_group_layer(mRegistry, group_layer_entity);

  EXPECT_FALSE(mRegistry.is_valid(group_layer_entity));
  EXPECT_EQ(mRegistry.count<CMeta>(), 0);
  EXPECT_EQ(mRegistry.count<CLayer>(), 0);
  EXPECT_EQ(mRegistry.count<CGroupLayer>(), 0);
  EXPECT_EQ(mRegistry.count(), 0);
}

/// \trace tactile::destroy_group_layer
TEST_F(GroupLayerTest, DestroyGroupLayerWithStoredLayers)
{
  const auto group_layer_entity = make_group_layer(mRegistry);

  {
    const MatrixExtent extent {5, 5};
    auto& group_layer = mRegistry.get<CGroupLayer>(group_layer_entity);

    const auto other_group_layer_entity = make_group_layer(mRegistry);
    auto& other_group_layer =
        mRegistry.get<CGroupLayer>(other_group_layer_entity);
    other_group_layer.layers.push_back(make_tile_layer(mRegistry, extent));

    group_layer.layers.push_back(make_tile_layer(mRegistry, extent));
    group_layer.layers.push_back(make_tile_layer(mRegistry, extent));
    group_layer.layers.push_back(make_object_layer(mRegistry));
    group_layer.layers.push_back(other_group_layer_entity);
  }

  EXPECT_TRUE(mRegistry.is_valid(group_layer_entity));
  EXPECT_EQ(mRegistry.count<CMeta>(), 6);
  EXPECT_EQ(mRegistry.count<CLayer>(), 6);
  EXPECT_EQ(mRegistry.count<CGroupLayer>(), 2);
  EXPECT_EQ(mRegistry.count<CTileLayer>(), 3);
  EXPECT_EQ(mRegistry.count<CDenseTileLayer>(), 3);
  EXPECT_EQ(mRegistry.count<CSparseTileLayer>(), 0);
  EXPECT_EQ(mRegistry.count<CObjectLayer>(), 1);
  EXPECT_EQ(mRegistry.count(), 21);

  destroy_group_layer(mRegistry, group_layer_entity);

  EXPECT_FALSE(mRegistry.is_valid(group_layer_entity));
  EXPECT_EQ(mRegistry.count<CMeta>(), 0);
  EXPECT_EQ(mRegistry.count<CLayer>(), 0);
  EXPECT_EQ(mRegistry.count<CGroupLayer>(), 0);
  EXPECT_EQ(mRegistry.count<CTileLayer>(), 0);
  EXPECT_EQ(mRegistry.count<CDenseTileLayer>(), 0);
  EXPECT_EQ(mRegistry.count<CSparseTileLayer>(), 0);
  EXPECT_EQ(mRegistry.count<CObjectLayer>(), 0);
  EXPECT_EQ(mRegistry.count(), 0);
}

}  // namespace tactile
