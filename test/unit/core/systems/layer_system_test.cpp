#include "core/systems/layer_system.hpp"

#include <gtest/gtest.h>

#include "core/components/layer.hpp"
#include "core/systems/registry_factory_system.hpp"

using namespace Tactile;

namespace {

constexpr entt::entity null_entity = entt::null;

}  // namespace

TEST(LayerSystem, SortLayersSimple)
{
  auto registry = Sys::MakeRegistry();

  const auto a = Sys::AddTileLayer(registry);
  const auto b = Sys::AddTileLayer(registry);
  const auto c = Sys::AddTileLayer(registry);

  ASSERT_EQ(0, Sys::GetLayerIndex(registry, a));
  ASSERT_EQ(0, Sys::GetLayerGlobalIndex(registry, a));

  ASSERT_EQ(1, Sys::GetLayerIndex(registry, b));
  ASSERT_EQ(1, Sys::GetLayerGlobalIndex(registry, b));

  ASSERT_EQ(2, Sys::GetLayerIndex(registry, c));
  ASSERT_EQ(2, Sys::GetLayerGlobalIndex(registry, c));

  Sys::SortLayers(registry);

  ASSERT_EQ(0, Sys::GetLayerIndex(registry, a));
  ASSERT_EQ(0, Sys::GetLayerGlobalIndex(registry, a));

  ASSERT_EQ(1, Sys::GetLayerIndex(registry, b));
  ASSERT_EQ(1, Sys::GetLayerGlobalIndex(registry, b));

  ASSERT_EQ(2, Sys::GetLayerIndex(registry, c));
  ASSERT_EQ(2, Sys::GetLayerGlobalIndex(registry, c));

  // [a, b, c] -> [c, a, b]
  Sys::MoveLayerDown(registry, b);
  Sys::MoveLayerDown(registry, a);
  Sys::SortLayers(registry);

  ASSERT_EQ(0, Sys::GetLayerIndex(registry, c));
  ASSERT_EQ(0, Sys::GetLayerGlobalIndex(registry, c));

  ASSERT_EQ(1, Sys::GetLayerIndex(registry, a));
  ASSERT_EQ(1, Sys::GetLayerGlobalIndex(registry, a));

  ASSERT_EQ(2, Sys::GetLayerIndex(registry, b));
  ASSERT_EQ(2, Sys::GetLayerGlobalIndex(registry, b));
}

TEST(LayerSystem, SortLayersAdvanced)
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
{}

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

TEST(LayerSystem, MoveLayerUp)
{
  auto registry = Sys::MakeRegistry();
  auto& active = registry.ctx<ActiveLayer>();

  const auto a = Sys::AddTileLayer(registry);
  const auto b = Sys::AddObjectLayer(registry);
  const auto c = Sys::AddGroupLayer(registry);

  active.entity = c;
  const auto d = Sys::AddObjectLayer(registry);
  const auto e = Sys::AddGroupLayer(registry);

  active.entity = e;
  const auto f = Sys::AddTileLayer(registry);
  const auto g = Sys::AddObjectLayer(registry);

  active.entity = c;
  const auto h = Sys::AddTileLayer(registry);

  active.entity = entt::null;
  const auto i = Sys::AddObjectLayer(registry);

  ASSERT_FALSE(Sys::CanMoveLayerUp(registry, a));
  ASSERT_TRUE(Sys::CanMoveLayerUp(registry, b));
  ASSERT_TRUE(Sys::CanMoveLayerUp(registry, c));
  {
    ASSERT_FALSE(Sys::CanMoveLayerUp(registry, d));
    ASSERT_TRUE(Sys::CanMoveLayerUp(registry, e));
    {
      ASSERT_FALSE(Sys::CanMoveLayerUp(registry, f));
      ASSERT_TRUE(Sys::CanMoveLayerUp(registry, g));
    }
    ASSERT_TRUE(Sys::CanMoveLayerUp(registry, h));
  }
  ASSERT_TRUE(Sys::CanMoveLayerUp(registry, i));

  ASSERT_EQ(0, Sys::GetLayerIndex(registry, a));
  ASSERT_EQ(1, Sys::GetLayerIndex(registry, b));
  Sys::MoveLayerUp(registry, b);
  ASSERT_EQ(1, Sys::GetLayerIndex(registry, a));
  ASSERT_EQ(0, Sys::GetLayerIndex(registry, b));

  ASSERT_EQ(2, Sys::GetLayerIndex(registry, c));
  ASSERT_EQ(3, Sys::GetLayerIndex(registry, i));
  Sys::MoveLayerUp(registry, i);
  ASSERT_EQ(3, Sys::GetLayerIndex(registry, c));
  ASSERT_EQ(2, Sys::GetLayerIndex(registry, i));

  ASSERT_EQ(0, Sys::GetLayerIndex(registry, f));
  ASSERT_EQ(1, Sys::GetLayerIndex(registry, g));
  Sys::MoveLayerUp(registry, g);
  ASSERT_EQ(1, Sys::GetLayerIndex(registry, f));
  ASSERT_EQ(0, Sys::GetLayerIndex(registry, g));

  ASSERT_EQ(0, Sys::GetLayerIndex(registry, d));
  ASSERT_EQ(1, Sys::GetLayerIndex(registry, e));
  Sys::MoveLayerUp(registry, e);
  ASSERT_EQ(1, Sys::GetLayerIndex(registry, d));
  ASSERT_EQ(0, Sys::GetLayerIndex(registry, e));
}

TEST(LayerSystem, MoveLayerDown)
{
  auto registry = Sys::MakeRegistry();
  auto& active = registry.ctx<ActiveLayer>();

  const auto a = Sys::AddTileLayer(registry);
  const auto b = Sys::AddObjectLayer(registry);
  const auto c = Sys::AddGroupLayer(registry);

  active.entity = c;
  const auto d = Sys::AddObjectLayer(registry);
  const auto e = Sys::AddGroupLayer(registry);

  active.entity = e;
  const auto f = Sys::AddTileLayer(registry);
  const auto g = Sys::AddObjectLayer(registry);

  active.entity = c;
  const auto h = Sys::AddTileLayer(registry);

  active.entity = entt::null;
  const auto i = Sys::AddObjectLayer(registry);

  ASSERT_TRUE(Sys::CanMoveLayerDown(registry, a));
  ASSERT_TRUE(Sys::CanMoveLayerDown(registry, b));
  ASSERT_TRUE(Sys::CanMoveLayerDown(registry, c));
  {
    ASSERT_TRUE(Sys::CanMoveLayerDown(registry, d));
    ASSERT_TRUE(Sys::CanMoveLayerDown(registry, e));
    {
      ASSERT_TRUE(Sys::CanMoveLayerDown(registry, f));
      ASSERT_FALSE(Sys::CanMoveLayerDown(registry, g));
    }
    ASSERT_FALSE(Sys::CanMoveLayerDown(registry, h));
  }
  ASSERT_FALSE(Sys::CanMoveLayerDown(registry, i));

  ASSERT_EQ(0, Sys::GetLayerIndex(registry, a));
  ASSERT_EQ(1, Sys::GetLayerIndex(registry, b));
  Sys::MoveLayerDown(registry, a);
  ASSERT_EQ(1, Sys::GetLayerIndex(registry, a));
  ASSERT_EQ(0, Sys::GetLayerIndex(registry, b));

  ASSERT_EQ(1, Sys::GetLayerIndex(registry, a));
  ASSERT_EQ(2, Sys::GetLayerIndex(registry, c));
  Sys::MoveLayerDown(registry, a);
  ASSERT_EQ(2, Sys::GetLayerIndex(registry, a));
  ASSERT_EQ(1, Sys::GetLayerIndex(registry, c));

  ASSERT_EQ(1, Sys::GetLayerIndex(registry, e));
  ASSERT_EQ(2, Sys::GetLayerIndex(registry, h));
  Sys::MoveLayerDown(registry, e);
  ASSERT_EQ(2, Sys::GetLayerIndex(registry, e));
  ASSERT_EQ(1, Sys::GetLayerIndex(registry, h));
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

TEST(LayerSystem, GetLayerGlobalIndex)
{
  auto registry = Sys::MakeRegistry();
  auto& active = registry.ctx<ActiveLayer>();

  const auto a = Sys::AddTileLayer(registry);
  const auto b = Sys::AddObjectLayer(registry);
  const auto c = Sys::AddGroupLayer(registry);

  active.entity = c;
  const auto d = Sys::AddTileLayer(registry);
  const auto e = Sys::AddGroupLayer(registry);

  active.entity = e;
  const auto f = Sys::AddTileLayer(registry);
  const auto g = Sys::AddTileLayer(registry);
  const auto h = Sys::AddTileLayer(registry);

  active.entity = c;
  const auto i = Sys::AddTileLayer(registry);

  active.entity = entt::null;
  const auto j = Sys::AddObjectLayer(registry);

  ASSERT_EQ(0, Sys::GetLayerIndex(registry, a));
  ASSERT_EQ(1, Sys::GetLayerIndex(registry, b));
  ASSERT_EQ(2, Sys::GetLayerIndex(registry, c));
  {
    ASSERT_EQ(0, Sys::GetLayerIndex(registry, d));
    ASSERT_EQ(1, Sys::GetLayerIndex(registry, e));
    {
      ASSERT_EQ(0, Sys::GetLayerIndex(registry, f));
      ASSERT_EQ(1, Sys::GetLayerIndex(registry, g));
      ASSERT_EQ(2, Sys::GetLayerIndex(registry, h));
    }
    ASSERT_EQ(2, Sys::GetLayerIndex(registry, i));
  }
  ASSERT_EQ(3, Sys::GetLayerIndex(registry, j));

  ASSERT_EQ(0, Sys::GetLayerGlobalIndex(registry, a));
  ASSERT_EQ(1, Sys::GetLayerGlobalIndex(registry, b));
  ASSERT_EQ(2, Sys::GetLayerGlobalIndex(registry, c));
  {
    ASSERT_EQ(3, Sys::GetLayerGlobalIndex(registry, d));
    EXPECT_EQ(4, Sys::GetLayerGlobalIndex(registry, e));
    {
      EXPECT_EQ(5, Sys::GetLayerGlobalIndex(registry, f));
      ASSERT_EQ(6, Sys::GetLayerGlobalIndex(registry, g));
      ASSERT_EQ(7, Sys::GetLayerGlobalIndex(registry, h));
    }
    ASSERT_EQ(8, Sys::GetLayerGlobalIndex(registry, i));
  }
  ASSERT_EQ(9, Sys::GetLayerGlobalIndex(registry, j));
}

TEST(LayerSystem, GetLayerSiblingAbove)
{
  auto registry = Sys::MakeRegistry();
  auto& active = registry.ctx<ActiveLayer>();

  const auto a = Sys::AddTileLayer(registry);
  const auto b = Sys::AddObjectLayer(registry);
  const auto c = Sys::AddGroupLayer(registry);

  active.entity = c;
  const auto d = Sys::AddGroupLayer(registry);

  active.entity = d;
  const auto e = Sys::AddTileLayer(registry);
  const auto f = Sys::AddTileLayer(registry);

  active.entity = c;
  const auto g = Sys::AddTileLayer(registry);

  active.entity = entt::null;
  const auto h = Sys::AddObjectLayer(registry);

  ASSERT_EQ(null_entity, Sys::GetLayerSiblingAbove(registry, a));
  ASSERT_EQ(a, Sys::GetLayerSiblingAbove(registry, b));
  ASSERT_EQ(b, Sys::GetLayerSiblingAbove(registry, c));
  {
    ASSERT_EQ(null_entity, Sys::GetLayerSiblingAbove(registry, d));
    {
      ASSERT_EQ(null_entity, Sys::GetLayerSiblingAbove(registry, e));
      ASSERT_EQ(e, Sys::GetLayerSiblingAbove(registry, f));
    }
    ASSERT_EQ(d, Sys::GetLayerSiblingAbove(registry, g));
  }
  ASSERT_EQ(c, Sys::GetLayerSiblingAbove(registry, h));

  ASSERT_EQ(0, Sys::GetLayerGlobalIndex(registry, a));
  ASSERT_EQ(1, Sys::GetLayerGlobalIndex(registry, b));
  ASSERT_EQ(2, Sys::GetLayerGlobalIndex(registry, c));
  ASSERT_EQ(3, Sys::GetLayerGlobalIndex(registry, d));
  ASSERT_EQ(4, Sys::GetLayerGlobalIndex(registry, e));
  ASSERT_EQ(5, Sys::GetLayerGlobalIndex(registry, f));
  ASSERT_EQ(6, Sys::GetLayerGlobalIndex(registry, g));
  ASSERT_EQ(7, Sys::GetLayerGlobalIndex(registry, h));
}

TEST(LayerSystem, GetLayerSiblingBelow)
{
  auto registry = Sys::MakeRegistry();
  auto& activeLayer = registry.ctx<ActiveLayer>();

  const auto a = Sys::AddTileLayer(registry);
  const auto b = Sys::AddObjectLayer(registry);
  const auto c = Sys::AddGroupLayer(registry);

  activeLayer.entity = c;
  const auto d = Sys::AddTileLayer(registry);
  const auto e = Sys::AddGroupLayer(registry);

  activeLayer.entity = e;
  const auto f = Sys::AddObjectLayer(registry);
  const auto g = Sys::AddTileLayer(registry);

  activeLayer.entity = c;
  const auto h = Sys::AddTileLayer(registry);

  activeLayer.entity = entt::null;
  const auto i = Sys::AddTileLayer(registry);

  ASSERT_EQ(b, Sys::GetLayerSiblingBelow(registry, a));
  ASSERT_EQ(c, Sys::GetLayerSiblingBelow(registry, b));
  ASSERT_EQ(i, Sys::GetLayerSiblingBelow(registry, c));
  {
    ASSERT_EQ(e, Sys::GetLayerSiblingBelow(registry, d));
    ASSERT_EQ(h, Sys::GetLayerSiblingBelow(registry, e));
    {
      ASSERT_EQ(g, Sys::GetLayerSiblingBelow(registry, f));
      ASSERT_EQ(null_entity, Sys::GetLayerSiblingBelow(registry, g));
    }
    ASSERT_EQ(null_entity, Sys::GetLayerSiblingBelow(registry, h));
  }
  ASSERT_EQ(null_entity, Sys::GetLayerSiblingBelow(registry, i));
}

TEST(LayerSystem, GetSiblingCount)
{
  auto registry = Sys::MakeRegistry();
  auto& active = registry.ctx<ActiveLayer>();

  const auto a = Sys::AddTileLayer(registry);
  const auto b = Sys::AddTileLayer(registry);
  const auto c = Sys::AddObjectLayer(registry);
  const auto d = Sys::AddGroupLayer(registry);

  active.entity = d;
  const auto e = Sys::AddTileLayer(registry);
  const auto f = Sys::AddObjectLayer(registry);
  const auto g = Sys::AddGroupLayer(registry);

  active.entity = g;
  const auto h = Sys::AddTileLayer(registry);

  active.entity = d;
  const auto i = Sys::AddTileLayer(registry);

  active.entity = entt::null;
  const auto j = Sys::AddObjectLayer(registry);

  ASSERT_EQ(4, Sys::GetSiblingCount(registry, a));
  ASSERT_EQ(4, Sys::GetSiblingCount(registry, b));
  ASSERT_EQ(4, Sys::GetSiblingCount(registry, c));
  ASSERT_EQ(4, Sys::GetSiblingCount(registry, d));
  {
    ASSERT_EQ(3, Sys::GetSiblingCount(registry, e));
    ASSERT_EQ(3, Sys::GetSiblingCount(registry, f));
    ASSERT_EQ(3, Sys::GetSiblingCount(registry, g));
    {
      ASSERT_EQ(0, Sys::GetSiblingCount(registry, h));
    }
    ASSERT_EQ(3, Sys::GetSiblingCount(registry, i));
  }
  ASSERT_EQ(4, Sys::GetSiblingCount(registry, j));
}

TEST(LayerSystem, GetLayerChildrenCount)
{
  auto registry = Sys::MakeRegistry();
  auto& active = registry.ctx<ActiveLayer>();

  const auto a = Sys::AddTileLayer(registry);
  const auto b = Sys::AddGroupLayer(registry);
  const auto c = Sys::AddTileLayer(registry);

  active.entity = b;
  const auto d = Sys::AddGroupLayer(registry);
  const auto e = Sys::AddTileLayer(registry);

  active.entity = d;
  const auto f = Sys::AddTileLayer(registry);
  const auto g = Sys::AddObjectLayer(registry);
  const auto h = Sys::AddObjectLayer(registry);

  ASSERT_EQ(0, Sys::GetLayerChildrenCount(registry, a));
  ASSERT_EQ(5, Sys::GetLayerChildrenCount(registry, b));
  {
    ASSERT_EQ(3, Sys::GetLayerChildrenCount(registry, d));
    {
      ASSERT_EQ(0, Sys::GetLayerChildrenCount(registry, f));
      ASSERT_EQ(0, Sys::GetLayerChildrenCount(registry, g));
      ASSERT_EQ(0, Sys::GetLayerChildrenCount(registry, h));
    }
    ASSERT_EQ(0, Sys::GetLayerChildrenCount(registry, e));
  }
  ASSERT_EQ(0, Sys::GetLayerChildrenCount(registry, c));
}
