#include "core/systems/layers/layer_system.hpp"

#include <gtest/gtest.h>

#include "core/components/layer.hpp"
#include "core/systems/registry_factory_system.hpp"

using namespace Tactile;

namespace {

constexpr entt::entity null_entity = entt::null;

}  // namespace

TEST(LayerSystem, SortLayers)
{
  auto registry = Sys::MakeRegistry();
  auto& active = registry.ctx<ActiveLayer>();

  const auto a = Sys::AddTileLayer(registry);
  const auto b = Sys::AddObjectLayer(registry);
  const auto c = Sys::AddGroupLayer(registry);

  active.entity = c;
  const auto d = Sys::AddTileLayer(registry);
  const auto e = Sys::AddTileLayer(registry);
  const auto f = Sys::AddGroupLayer(registry);

  active.entity = f;
  const auto g = Sys::AddTileLayer(registry);
  const auto h = Sys::AddTileLayer(registry);

  active.entity = c;
  const auto i = Sys::AddTileLayer(registry);
  const auto j = Sys::AddTileLayer(registry);

  active.entity = entt::null;
  const auto k = Sys::AddObjectLayer(registry);

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

  ASSERT_EQ(0, Sys::GetLayerIndex(registry, a));
  ASSERT_EQ(1, Sys::GetLayerIndex(registry, b));
  ASSERT_EQ(2, Sys::GetLayerIndex(registry, c));
  {
    ASSERT_EQ(0, Sys::GetLayerIndex(registry, d));
    ASSERT_EQ(1, Sys::GetLayerIndex(registry, e));
    ASSERT_EQ(2, Sys::GetLayerIndex(registry, f));
    {
      ASSERT_EQ(0, Sys::GetLayerIndex(registry, g));
      ASSERT_EQ(1, Sys::GetLayerIndex(registry, h));
    }

    ASSERT_EQ(3, Sys::GetLayerIndex(registry, i));
    ASSERT_EQ(4, Sys::GetLayerIndex(registry, j));
  }

  ASSERT_EQ(3, Sys::GetLayerIndex(registry, k));
}

TEST(LayerSystem, RemoveLayer)
{
  auto registry = Sys::MakeRegistry();
  auto& active = registry.ctx<ActiveLayer>();

  const auto a = Sys::AddTileLayer(registry);
  const auto b = Sys::AddGroupLayer(registry);

  active.entity = b;
  const auto c = Sys::AddObjectLayer(registry);
  const auto d = Sys::AddObjectLayer(registry);
  const auto e = Sys::AddGroupLayer(registry);

  active.entity = e;
  const auto f = Sys::AddTileLayer(registry);
  const auto g = Sys::AddTileLayer(registry);

  active.entity = entt::null;
  const auto h = Sys::AddTileLayer(registry);

  ASSERT_EQ(0, Sys::GetLayerIndex(registry, a));
  ASSERT_EQ(1, Sys::GetLayerIndex(registry, b));
  {
    ASSERT_EQ(0, Sys::GetLayerIndex(registry, c));
    ASSERT_EQ(1, Sys::GetLayerIndex(registry, d));
    ASSERT_EQ(2, Sys::GetLayerIndex(registry, e));
    {
      ASSERT_EQ(0, Sys::GetLayerIndex(registry, f));
      ASSERT_EQ(1, Sys::GetLayerIndex(registry, g));
    }
  }
  ASSERT_EQ(2, Sys::GetLayerIndex(registry, h));

  Sys::RemoveLayer(registry, a);
  ASSERT_EQ(0, Sys::GetLayerIndex(registry, b));
  {
    ASSERT_EQ(0, Sys::GetLayerIndex(registry, c));
    ASSERT_EQ(1, Sys::GetLayerIndex(registry, d));
    ASSERT_EQ(2, Sys::GetLayerIndex(registry, e));
    {
      ASSERT_EQ(0, Sys::GetLayerIndex(registry, f));
      ASSERT_EQ(1, Sys::GetLayerIndex(registry, g));
    }
  }
  ASSERT_EQ(1, Sys::GetLayerIndex(registry, h));

  Sys::RemoveLayer(registry, d);
  ASSERT_EQ(0, Sys::GetLayerIndex(registry, b));
  {
    ASSERT_EQ(0, Sys::GetLayerIndex(registry, c));
    ASSERT_EQ(1, Sys::GetLayerIndex(registry, e));
    {
      ASSERT_EQ(0, Sys::GetLayerIndex(registry, f));
      ASSERT_EQ(1, Sys::GetLayerIndex(registry, g));
    }
  }
  ASSERT_EQ(1, Sys::GetLayerIndex(registry, h));

  Sys::RemoveLayer(registry, b);
  ASSERT_EQ(0, Sys::GetLayerIndex(registry, h));
  ASSERT_EQ(1, registry.size<Layer>());

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

  auto registry = Sys::MakeRegistry();

  const auto a = Sys::AddTileLayer(registry);
  Sys::SetLayerOpacity(registry, a, 0.5f);
  Sys::SetLayerVisible(registry, a, false);

  const auto b = Sys::DuplicateLayer(registry, a);

  ASSERT_EQ(0, Sys::GetLayerIndex(registry, a));
  ASSERT_EQ(1, Sys::GetLayerIndex(registry, b));

  ASSERT_EQ(Sys::GetLayerOpacity(registry, a), Sys::GetLayerOpacity(registry, b));
  ASSERT_EQ(Sys::IsLayerVisible(registry, a), Sys::IsLayerVisible(registry, b));
}

TEST(LayerSystem, FindLayer)
{
  auto registry = Sys::MakeRegistry();
  ASSERT_EQ(null_entity, Sys::FindLayer(registry, 1_layer));

  const auto a = Sys::AddTileLayer(registry);
  const auto b = Sys::AddObjectLayer(registry);

  ASSERT_EQ(a, Sys::FindLayer(registry, 1_layer));
  ASSERT_EQ(b, Sys::FindLayer(registry, 2_layer));
}
