#include "core/systems/layers/layer_system.hpp"

#include <gtest/gtest.h>

#include "core/components/layer.hpp"
#include "core/systems/registry_system.hpp"

using namespace tactile;

namespace {

constexpr entt::entity null_entity = entt::null;

}  // namespace

TEST(LayerSystem, SortLayers)
{
  auto registry = sys::make_document_registry();
  auto& active = registry.ctx<ActiveLayer>();

  const auto a = sys::AddTileLayer(registry);
  const auto b = sys::AddObjectLayer(registry);
  const auto c = sys::AddGroupLayer(registry);

  active.entity = c;
  const auto d = sys::AddTileLayer(registry);
  const auto e = sys::AddTileLayer(registry);
  const auto f = sys::AddGroupLayer(registry);

  active.entity = f;
  const auto g = sys::AddTileLayer(registry);
  const auto h = sys::AddTileLayer(registry);

  active.entity = c;
  const auto i = sys::AddTileLayer(registry);
  const auto j = sys::AddTileLayer(registry);

  active.entity = entt::null;
  const auto k = sys::AddObjectLayer(registry);

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

  ASSERT_EQ(0, sys::GetLayerIndex(registry, a));
  ASSERT_EQ(1, sys::GetLayerIndex(registry, b));
  ASSERT_EQ(2, sys::GetLayerIndex(registry, c));
  {
    ASSERT_EQ(0, sys::GetLayerIndex(registry, d));
    ASSERT_EQ(1, sys::GetLayerIndex(registry, e));
    ASSERT_EQ(2, sys::GetLayerIndex(registry, f));
    {
      ASSERT_EQ(0, sys::GetLayerIndex(registry, g));
      ASSERT_EQ(1, sys::GetLayerIndex(registry, h));
    }

    ASSERT_EQ(3, sys::GetLayerIndex(registry, i));
    ASSERT_EQ(4, sys::GetLayerIndex(registry, j));
  }

  ASSERT_EQ(3, sys::GetLayerIndex(registry, k));
}

TEST(LayerSystem, RemoveLayer)
{
  auto registry = sys::make_document_registry();
  auto& active = registry.ctx<ActiveLayer>();

  const auto a = sys::AddTileLayer(registry);
  const auto b = sys::AddGroupLayer(registry);

  active.entity = b;
  const auto c = sys::AddObjectLayer(registry);
  const auto d = sys::AddObjectLayer(registry);
  const auto e = sys::AddGroupLayer(registry);

  active.entity = e;
  const auto f = sys::AddTileLayer(registry);
  const auto g = sys::AddTileLayer(registry);

  active.entity = entt::null;
  const auto h = sys::AddTileLayer(registry);

  ASSERT_EQ(0, sys::GetLayerIndex(registry, a));
  ASSERT_EQ(1, sys::GetLayerIndex(registry, b));
  {
    ASSERT_EQ(0, sys::GetLayerIndex(registry, c));
    ASSERT_EQ(1, sys::GetLayerIndex(registry, d));
    ASSERT_EQ(2, sys::GetLayerIndex(registry, e));
    {
      ASSERT_EQ(0, sys::GetLayerIndex(registry, f));
      ASSERT_EQ(1, sys::GetLayerIndex(registry, g));
    }
  }
  ASSERT_EQ(2, sys::GetLayerIndex(registry, h));

  sys::RemoveLayer(registry, a);
  ASSERT_EQ(0, sys::GetLayerIndex(registry, b));
  {
    ASSERT_EQ(0, sys::GetLayerIndex(registry, c));
    ASSERT_EQ(1, sys::GetLayerIndex(registry, d));
    ASSERT_EQ(2, sys::GetLayerIndex(registry, e));
    {
      ASSERT_EQ(0, sys::GetLayerIndex(registry, f));
      ASSERT_EQ(1, sys::GetLayerIndex(registry, g));
    }
  }
  ASSERT_EQ(1, sys::GetLayerIndex(registry, h));

  sys::RemoveLayer(registry, d);
  ASSERT_EQ(0, sys::GetLayerIndex(registry, b));
  {
    ASSERT_EQ(0, sys::GetLayerIndex(registry, c));
    ASSERT_EQ(1, sys::GetLayerIndex(registry, e));
    {
      ASSERT_EQ(0, sys::GetLayerIndex(registry, f));
      ASSERT_EQ(1, sys::GetLayerIndex(registry, g));
    }
  }
  ASSERT_EQ(1, sys::GetLayerIndex(registry, h));

  sys::RemoveLayer(registry, b);
  ASSERT_EQ(0, sys::GetLayerIndex(registry, h));
  ASSERT_EQ(1, registry.storage<Layer>().size());

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

  const auto a = sys::AddTileLayer(registry);
  sys::SetLayerOpacity(registry, a, 0.5f);
  sys::SetLayerVisible(registry, a, false);

  const auto b = sys::DuplicateLayer(registry, a);

  ASSERT_EQ(0, sys::GetLayerIndex(registry, a));
  ASSERT_EQ(1, sys::GetLayerIndex(registry, b));

  ASSERT_EQ(sys::GetLayerOpacity(registry, a), sys::GetLayerOpacity(registry, b));
  ASSERT_EQ(sys::IsLayerVisible(registry, a), sys::IsLayerVisible(registry, b));
}

TEST(LayerSystem, FindLayer)
{
  auto registry = sys::make_document_registry();
  ASSERT_EQ(null_entity, sys::FindLayer(registry, 1));

  const auto a = sys::AddTileLayer(registry);
  const auto b = sys::AddObjectLayer(registry);

  ASSERT_EQ(a, sys::FindLayer(registry, 1));
  ASSERT_EQ(b, sys::FindLayer(registry, 2));
}
