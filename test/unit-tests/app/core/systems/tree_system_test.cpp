#include <gtest/gtest.h>

#include "core/components/layers.hpp"
#include "core/components/parent.hpp"
#include "core/systems/layers/layer_tree_system.hpp"
#include "core/systems/registry_system.hpp"

using namespace tactile;

namespace {

constexpr entt::entity null_entity = entt::null;

// > a
//   |__b
//      |__d
//   |__c
//   |__f
// > e
struct SimpleTreeConfig final
{
  entt::registry registry;
  entt::entity a{entt::null};
  entt::entity b{entt::null};
  entt::entity c{entt::null};
  entt::entity d{entt::null};
  entt::entity e{entt::null};
  entt::entity f{entt::null};
};

[[nodiscard]] auto CreateTreeRegistrySimple() -> SimpleTreeConfig
{
  SimpleTreeConfig cfg;
  cfg.registry = sys::make_document_registry();

  cfg.a = cfg.registry.create();
  cfg.b = cfg.registry.create();
  cfg.c = cfg.registry.create();
  cfg.d = cfg.registry.create();
  cfg.e = cfg.registry.create();
  cfg.f = cfg.registry.create();

  cfg.registry.emplace<comp::Parent>(cfg.a, entt::null);
  cfg.registry.emplace<comp::Parent>(cfg.b, cfg.a);
  cfg.registry.emplace<comp::Parent>(cfg.c, cfg.a);
  cfg.registry.emplace<comp::Parent>(cfg.d, cfg.b);
  cfg.registry.emplace<comp::Parent>(cfg.e, entt::null);
  cfg.registry.emplace<comp::Parent>(cfg.f, cfg.a);

  {
    auto& node = cfg.registry.emplace<comp::LayerTreeNode>(cfg.a, 0u);
    node.children.push_back(cfg.b);
    node.children.push_back(cfg.c);
    node.children.push_back(cfg.f);
  }

  {
    auto& node = cfg.registry.emplace<comp::LayerTreeNode>(cfg.b, 0u);
    node.children.push_back(cfg.d);
  }

  cfg.registry.emplace<comp::LayerTreeNode>(cfg.c, 1u);
  cfg.registry.emplace<comp::LayerTreeNode>(cfg.d, 0u);
  cfg.registry.emplace<comp::LayerTreeNode>(cfg.e, 1u);
  cfg.registry.emplace<comp::LayerTreeNode>(cfg.f, 2u);

  sys::sort_layers(cfg.registry);

  return cfg;
}

}  // namespace

TEST(TreeSystem, MoveLayerUp)
{
  auto cfg = CreateTreeRegistrySimple();

  ASSERT_EQ(0u, cfg.registry.get<comp::LayerTreeNode>(cfg.a).index);
  ASSERT_EQ(1u, cfg.registry.get<comp::LayerTreeNode>(cfg.e).index);
  ASSERT_EQ(0u, sys::layer_global_index(cfg.registry, cfg.a));
  ASSERT_EQ(5u, sys::layer_global_index(cfg.registry, cfg.e));
  sys::move_layer_up(cfg.registry, cfg.e);

  // New tree
  // > e
  // > a
  //   |__b
  //      |__d
  //   |__c
  //   |__f
  ASSERT_EQ(1u, cfg.registry.get<comp::LayerTreeNode>(cfg.a).index);
  ASSERT_EQ(0u, cfg.registry.get<comp::LayerTreeNode>(cfg.e).index);
  ASSERT_EQ(1u, sys::layer_global_index(cfg.registry, cfg.a));
  ASSERT_EQ(0u, sys::layer_global_index(cfg.registry, cfg.e));

  ASSERT_EQ(0u, cfg.registry.get<comp::LayerTreeNode>(cfg.b).index);
  ASSERT_EQ(1u, cfg.registry.get<comp::LayerTreeNode>(cfg.c).index);
  ASSERT_EQ(2u, sys::layer_global_index(cfg.registry, cfg.b));
  ASSERT_EQ(4u, sys::layer_global_index(cfg.registry, cfg.c));
  sys::move_layer_up(cfg.registry, cfg.c);

  // New tree
  // > e
  // > a
  //   |__c
  //   |__b
  //      |__d
  //   |__f
  ASSERT_EQ(1u, cfg.registry.get<comp::LayerTreeNode>(cfg.b).index);
  ASSERT_EQ(0u, cfg.registry.get<comp::LayerTreeNode>(cfg.c).index);
  ASSERT_EQ(3u, sys::layer_global_index(cfg.registry, cfg.b));
  ASSERT_EQ(2u, sys::layer_global_index(cfg.registry, cfg.c));
}

TEST(TreeSystem, MoveLayerDown)
{
  auto cfg = CreateTreeRegistrySimple();

  ASSERT_EQ(0u, cfg.registry.get<comp::LayerTreeNode>(cfg.b).index);
  ASSERT_EQ(1u, cfg.registry.get<comp::LayerTreeNode>(cfg.c).index);
  ASSERT_EQ(1u, sys::layer_global_index(cfg.registry, cfg.b));
  ASSERT_EQ(3u, sys::layer_global_index(cfg.registry, cfg.c));
  sys::move_layer_down(cfg.registry, cfg.b);
  // New tree
  // > a
  //   |__c
  //   |__b
  //      |__d
  //   |__f
  // > e
  ASSERT_EQ(1u, cfg.registry.get<comp::LayerTreeNode>(cfg.b).index);
  ASSERT_EQ(0u, cfg.registry.get<comp::LayerTreeNode>(cfg.c).index);
  ASSERT_EQ(2u, sys::layer_global_index(cfg.registry, cfg.b));
  ASSERT_EQ(1u, sys::layer_global_index(cfg.registry, cfg.c));

  ASSERT_EQ(0u, cfg.registry.get<comp::LayerTreeNode>(cfg.a).index);
  ASSERT_EQ(1u, cfg.registry.get<comp::LayerTreeNode>(cfg.e).index);
  ASSERT_EQ(0u, sys::layer_global_index(cfg.registry, cfg.a));
  ASSERT_EQ(5u, sys::layer_global_index(cfg.registry, cfg.e));
  sys::move_layer_down(cfg.registry, cfg.a);
  // New tree
  // > e
  // > a
  //   |__c
  //   |__b
  //      |__d
  //   |__f
  ASSERT_EQ(1u, cfg.registry.get<comp::LayerTreeNode>(cfg.a).index);
  ASSERT_EQ(0u, cfg.registry.get<comp::LayerTreeNode>(cfg.e).index);
  ASSERT_EQ(1u, sys::layer_global_index(cfg.registry, cfg.a));
  ASSERT_EQ(0u, sys::layer_global_index(cfg.registry, cfg.e));
}

TEST(TreeSystem, CanMoveLayerUp)
{
  const auto cfg = CreateTreeRegistrySimple();
  ASSERT_FALSE(sys::can_move_layer_up(cfg.registry, cfg.a));
  ASSERT_FALSE(sys::can_move_layer_up(cfg.registry, cfg.b));
  ASSERT_TRUE(sys::can_move_layer_up(cfg.registry, cfg.c));
  ASSERT_FALSE(sys::can_move_layer_up(cfg.registry, cfg.d));
  ASSERT_TRUE(sys::can_move_layer_up(cfg.registry, cfg.e));
  ASSERT_TRUE(sys::can_move_layer_up(cfg.registry, cfg.f));
}

TEST(TreeSystem, CanMoveLayerDown)
{
  const auto cfg = CreateTreeRegistrySimple();
  ASSERT_TRUE(sys::can_move_layer_down(cfg.registry, cfg.a));
  ASSERT_TRUE(sys::can_move_layer_down(cfg.registry, cfg.b));
  ASSERT_TRUE(sys::can_move_layer_down(cfg.registry, cfg.c));
  ASSERT_FALSE(sys::can_move_layer_down(cfg.registry, cfg.d));
  ASSERT_FALSE(sys::can_move_layer_down(cfg.registry, cfg.e));
  ASSERT_FALSE(sys::can_move_layer_down(cfg.registry, cfg.f));
}

TEST(TreeSystem, LayerSiblingAbove)
{
  const auto cfg = CreateTreeRegistrySimple();
  ASSERT_EQ(null_entity, sys::layer_sibling_above(cfg.registry, cfg.a));
  ASSERT_EQ(null_entity, sys::layer_sibling_above(cfg.registry, cfg.b));
  ASSERT_EQ(cfg.b, sys::layer_sibling_above(cfg.registry, cfg.c));
  ASSERT_EQ(null_entity, sys::layer_sibling_above(cfg.registry, cfg.d));
  ASSERT_EQ(cfg.c, sys::layer_sibling_above(cfg.registry, cfg.f));
  ASSERT_EQ(cfg.a, sys::layer_sibling_above(cfg.registry, cfg.e));
}

TEST(TreeSystem, LayerSiblingBelow)
{
  const auto cfg = CreateTreeRegistrySimple();
  ASSERT_EQ(cfg.e, sys::layer_sibling_below(cfg.registry, cfg.a));
  ASSERT_EQ(cfg.c, sys::layer_sibling_below(cfg.registry, cfg.b));
  ASSERT_EQ(cfg.f, sys::layer_sibling_below(cfg.registry, cfg.c));
  ASSERT_EQ(null_entity, sys::layer_sibling_below(cfg.registry, cfg.d));
  ASSERT_EQ(null_entity, sys::layer_sibling_below(cfg.registry, cfg.f));
  ASSERT_EQ(null_entity, sys::layer_sibling_below(cfg.registry, cfg.e));
}

TEST(TreeSystem, LayerSiblingCount)
{
  const auto cfg = CreateTreeRegistrySimple();
  ASSERT_EQ(1u, sys::layer_sibling_count(cfg.registry, cfg.a));
  ASSERT_EQ(2u, sys::layer_sibling_count(cfg.registry, cfg.b));
  ASSERT_EQ(2u, sys::layer_sibling_count(cfg.registry, cfg.c));
  ASSERT_EQ(0u, sys::layer_sibling_count(cfg.registry, cfg.d));
  ASSERT_EQ(1u, sys::layer_sibling_count(cfg.registry, cfg.e));
  ASSERT_EQ(2u, sys::layer_sibling_count(cfg.registry, cfg.f));
}

TEST(TreeSystem, LayerChildrenCount)
{
  const auto cfg = CreateTreeRegistrySimple();
  ASSERT_EQ(4u, sys::layer_children_count(cfg.registry, cfg.a));
  ASSERT_EQ(1u, sys::layer_children_count(cfg.registry, cfg.b));
  ASSERT_EQ(0u, sys::layer_children_count(cfg.registry, cfg.c));
  ASSERT_EQ(0u, sys::layer_children_count(cfg.registry, cfg.d));
  ASSERT_EQ(0u, sys::layer_children_count(cfg.registry, cfg.e));
  ASSERT_EQ(0u, sys::layer_children_count(cfg.registry, cfg.f));
}

TEST(TreeSystem, LayerGlobalIndex)
{
  const auto cfg = CreateTreeRegistrySimple();
  ASSERT_EQ(0u, sys::layer_global_index(cfg.registry, cfg.a));
  ASSERT_EQ(1u, sys::layer_global_index(cfg.registry, cfg.b));
  ASSERT_EQ(2u, sys::layer_global_index(cfg.registry, cfg.d));
  ASSERT_EQ(3u, sys::layer_global_index(cfg.registry, cfg.c));
  ASSERT_EQ(4u, sys::layer_global_index(cfg.registry, cfg.f));
  ASSERT_EQ(5u, sys::layer_global_index(cfg.registry, cfg.e));
}