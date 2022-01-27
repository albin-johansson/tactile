#include "core/systems/layers/layer_system.hpp"

#include <gtest/gtest.h>

#include "core/components/layer.hpp"
#include "core/systems/layers/layer_tree_system.hpp"
#include "core/systems/registry_system.hpp"

using namespace tactile;

namespace {

constexpr entt::entity null_entity = entt::null;

}  // namespace

TEST(LayerSystem, SortLayers)
{
  auto registry = sys::make_document_registry();
  auto& active = registry.ctx<comp::active_layer>();

  const auto a = sys::make_tile_layer(registry);
  const auto b = sys::make_object_layer(registry);
  const auto c = sys::make_group_layer(registry);

  active.entity = c;
  const auto d = sys::make_tile_layer(registry);
  const auto e = sys::make_tile_layer(registry);
  const auto f = sys::make_group_layer(registry);

  active.entity = f;
  const auto g = sys::make_tile_layer(registry);
  const auto h = sys::make_tile_layer(registry);

  active.entity = c;
  const auto i = sys::make_tile_layer(registry);
  const auto j = sys::make_tile_layer(registry);

  active.entity = entt::null;
  const auto k = sys::make_object_layer(registry);

  /*
   - A
   - B
   - C
     - D
     - E
     - F
       - G
       - H
     - I
     - J
   - K
   */

  ASSERT_EQ(0, sys::layer_local_index(registry, a));
  ASSERT_EQ(1, sys::layer_local_index(registry, b));
  ASSERT_EQ(2, sys::layer_local_index(registry, c));
  {
    ASSERT_EQ(0, sys::layer_local_index(registry, d));
    ASSERT_EQ(1, sys::layer_local_index(registry, e));
    ASSERT_EQ(2, sys::layer_local_index(registry, f));
    {
      ASSERT_EQ(0, sys::layer_local_index(registry, g));
      ASSERT_EQ(1, sys::layer_local_index(registry, h));
    }

    ASSERT_EQ(3, sys::layer_local_index(registry, i));
    ASSERT_EQ(4, sys::layer_local_index(registry, j));
  }

  ASSERT_EQ(3, sys::layer_local_index(registry, k));
}

TEST(LayerSystem, RemoveLayer)
{
  auto registry = sys::make_document_registry();
  auto& active = registry.ctx<comp::active_layer>();

  const auto a = sys::make_tile_layer(registry);
  const auto b = sys::make_group_layer(registry);

  active.entity = b;
  const auto c = sys::make_object_layer(registry);
  const auto d = sys::make_object_layer(registry);
  const auto e = sys::make_group_layer(registry);

  active.entity = e;
  const auto f = sys::make_tile_layer(registry);
  const auto g = sys::make_tile_layer(registry);

  active.entity = entt::null;
  const auto h = sys::make_tile_layer(registry);

  ASSERT_EQ(0, sys::layer_local_index(registry, a));
  ASSERT_EQ(1, sys::layer_local_index(registry, b));
  {
    ASSERT_EQ(0, sys::layer_local_index(registry, c));
    ASSERT_EQ(1, sys::layer_local_index(registry, d));
    ASSERT_EQ(2, sys::layer_local_index(registry, e));
    {
      ASSERT_EQ(0, sys::layer_local_index(registry, f));
      ASSERT_EQ(1, sys::layer_local_index(registry, g));
    }
  }
  ASSERT_EQ(2, sys::layer_local_index(registry, h));

  sys::remove_layer(registry, a);
  ASSERT_EQ(0, sys::layer_local_index(registry, b));
  {
    ASSERT_EQ(0, sys::layer_local_index(registry, c));
    ASSERT_EQ(1, sys::layer_local_index(registry, d));
    ASSERT_EQ(2, sys::layer_local_index(registry, e));
    {
      ASSERT_EQ(0, sys::layer_local_index(registry, f));
      ASSERT_EQ(1, sys::layer_local_index(registry, g));
    }
  }
  ASSERT_EQ(1, sys::layer_local_index(registry, h));

  sys::remove_layer(registry, d);
  ASSERT_EQ(0, sys::layer_local_index(registry, b));
  {
    ASSERT_EQ(0, sys::layer_local_index(registry, c));
    ASSERT_EQ(1, sys::layer_local_index(registry, e));
    {
      ASSERT_EQ(0, sys::layer_local_index(registry, f));
      ASSERT_EQ(1, sys::layer_local_index(registry, g));
    }
  }
  ASSERT_EQ(1, sys::layer_local_index(registry, h));

  sys::remove_layer(registry, b);
  ASSERT_EQ(0, sys::layer_local_index(registry, h));
  ASSERT_EQ(1, registry.storage<comp::layer>().size());

  // Make sure all child layers were destroyed
  ASSERT_FALSE(registry.valid(a));
  ASSERT_FALSE(registry.valid(c));
  ASSERT_FALSE(registry.valid(d));
  ASSERT_FALSE(registry.valid(e));
  ASSERT_FALSE(registry.valid(f));
  ASSERT_FALSE(registry.valid(g));
  ASSERT_TRUE(registry.valid(h));
}

TEST(LayerSystem, SelectLayer)
{
  // TODO
}

TEST(LayerSystem, DuplicateLayer)
{
  // TODO improve this test

  auto registry = sys::make_document_registry();

  const auto a = sys::make_tile_layer(registry);
  auto& layerA = registry.get<comp::layer>(a);
  layerA.opacity = 0.5f;
  layerA.visible = false;

  const auto b = sys::duplicate_layer(registry, a);
  const auto& layerB = registry.get<comp::layer>(a);

  ASSERT_EQ(0, registry.get<comp::layer_tree_node>(a).index);
  ASSERT_EQ(1, registry.get<comp::layer_tree_node>(b).index);

  ASSERT_EQ(layerA.opacity, layerB.opacity);
  ASSERT_EQ(layerA.visible, layerB.visible);
}

TEST(LayerSystem, FindLayer)
{
  auto registry = sys::make_document_registry();
  ASSERT_EQ(null_entity, sys::find_layer(registry, 1));

  const auto a = sys::make_tile_layer(registry);
  const auto b = sys::make_object_layer(registry);

  ASSERT_EQ(a, sys::find_layer(registry, 1));
  ASSERT_EQ(b, sys::find_layer(registry, 2));
}
