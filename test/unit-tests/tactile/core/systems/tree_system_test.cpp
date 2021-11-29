#include <gtest/gtest.h>

#include "core/components/layer.hpp"
#include "core/components/parent.hpp"
#include "core/systems/layers/layer_tree_system.hpp"
#include "core/systems/registry_system.hpp"

using namespace Tactile;

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
  cfg.registry = Sys::MakeRegistry();

  cfg.a = cfg.registry.create();
  cfg.b = cfg.registry.create();
  cfg.c = cfg.registry.create();
  cfg.d = cfg.registry.create();
  cfg.e = cfg.registry.create();
  cfg.f = cfg.registry.create();

  cfg.registry.emplace<Parent>(cfg.a, entt::null);
  cfg.registry.emplace<Parent>(cfg.b, cfg.a);
  cfg.registry.emplace<Parent>(cfg.c, cfg.a);
  cfg.registry.emplace<Parent>(cfg.d, cfg.b);
  cfg.registry.emplace<Parent>(cfg.e, entt::null);
  cfg.registry.emplace<Parent>(cfg.f, cfg.a);

  {
    auto& node = cfg.registry.emplace<LayerTreeNode>(cfg.a, 0u);
    node.children.push_back(cfg.b);
    node.children.push_back(cfg.c);
    node.children.push_back(cfg.f);
  }

  {
    auto& node = cfg.registry.emplace<LayerTreeNode>(cfg.b, 0u);
    node.children.push_back(cfg.d);
  }

  cfg.registry.emplace<LayerTreeNode>(cfg.c, 1u);
  cfg.registry.emplace<LayerTreeNode>(cfg.d, 0u);
  cfg.registry.emplace<LayerTreeNode>(cfg.e, 1u);
  cfg.registry.emplace<LayerTreeNode>(cfg.f, 2u);

  Sys::LayerTree::SortNodes(cfg.registry);

  return cfg;
}

}  // namespace

TEST(TreeSystem, MoveNodeUp)
{
  auto cfg = CreateTreeRegistrySimple();

  ASSERT_EQ(0u, cfg.registry.get<LayerTreeNode>(cfg.a).index);
  ASSERT_EQ(1u, cfg.registry.get<LayerTreeNode>(cfg.e).index);
  ASSERT_EQ(0u, Sys::LayerTree::GetGlobalIndex(cfg.registry, cfg.a));
  ASSERT_EQ(5u, Sys::LayerTree::GetGlobalIndex(cfg.registry, cfg.e));
  Sys::LayerTree::MoveNodeUp(cfg.registry, cfg.e);

  // New tree
  // > e
  // > a
  //   |__b
  //      |__d
  //   |__c
  //   |__f
  ASSERT_EQ(1u, cfg.registry.get<LayerTreeNode>(cfg.a).index);
  ASSERT_EQ(0u, cfg.registry.get<LayerTreeNode>(cfg.e).index);
  ASSERT_EQ(1u, Sys::LayerTree::GetGlobalIndex(cfg.registry, cfg.a));
  ASSERT_EQ(0u, Sys::LayerTree::GetGlobalIndex(cfg.registry, cfg.e));

  ASSERT_EQ(0u, cfg.registry.get<LayerTreeNode>(cfg.b).index);
  ASSERT_EQ(1u, cfg.registry.get<LayerTreeNode>(cfg.c).index);
  ASSERT_EQ(2u, Sys::LayerTree::GetGlobalIndex(cfg.registry, cfg.b));
  ASSERT_EQ(4u, Sys::LayerTree::GetGlobalIndex(cfg.registry, cfg.c));
  Sys::LayerTree::MoveNodeUp(cfg.registry, cfg.c);

  // New tree
  // > e
  // > a
  //   |__c
  //   |__b
  //      |__d
  //   |__f
  ASSERT_EQ(1u, cfg.registry.get<LayerTreeNode>(cfg.b).index);
  ASSERT_EQ(0u, cfg.registry.get<LayerTreeNode>(cfg.c).index);
  ASSERT_EQ(3u, Sys::LayerTree::GetGlobalIndex(cfg.registry, cfg.b));
  ASSERT_EQ(2u, Sys::LayerTree::GetGlobalIndex(cfg.registry, cfg.c));
}

TEST(TreeSystem, MoveNodeDown)
{
  auto cfg = CreateTreeRegistrySimple();

  ASSERT_EQ(0u, cfg.registry.get<LayerTreeNode>(cfg.b).index);
  ASSERT_EQ(1u, cfg.registry.get<LayerTreeNode>(cfg.c).index);
  ASSERT_EQ(1u, Sys::LayerTree::GetGlobalIndex(cfg.registry, cfg.b));
  ASSERT_EQ(3u, Sys::LayerTree::GetGlobalIndex(cfg.registry, cfg.c));
  Sys::LayerTree::MoveNodeDown(cfg.registry, cfg.b);
  // New tree
  // > a
  //   |__c
  //   |__b
  //      |__d
  //   |__f
  // > e
  ASSERT_EQ(1u, cfg.registry.get<LayerTreeNode>(cfg.b).index);
  ASSERT_EQ(0u, cfg.registry.get<LayerTreeNode>(cfg.c).index);
  ASSERT_EQ(2u, Sys::LayerTree::GetGlobalIndex(cfg.registry, cfg.b));
  ASSERT_EQ(1u, Sys::LayerTree::GetGlobalIndex(cfg.registry, cfg.c));

  ASSERT_EQ(0u, cfg.registry.get<LayerTreeNode>(cfg.a).index);
  ASSERT_EQ(1u, cfg.registry.get<LayerTreeNode>(cfg.e).index);
  ASSERT_EQ(0u, Sys::LayerTree::GetGlobalIndex(cfg.registry, cfg.a));
  ASSERT_EQ(5u, Sys::LayerTree::GetGlobalIndex(cfg.registry, cfg.e));
  Sys::LayerTree::MoveNodeDown(cfg.registry, cfg.a);
  // New tree
  // > e
  // > a
  //   |__c
  //   |__b
  //      |__d
  //   |__f
  ASSERT_EQ(1u, cfg.registry.get<LayerTreeNode>(cfg.a).index);
  ASSERT_EQ(0u, cfg.registry.get<LayerTreeNode>(cfg.e).index);
  ASSERT_EQ(1u, Sys::LayerTree::GetGlobalIndex(cfg.registry, cfg.a));
  ASSERT_EQ(0u, Sys::LayerTree::GetGlobalIndex(cfg.registry, cfg.e));
}

TEST(TreeSystem, CanMoveNodeUp)
{
  const auto cfg = CreateTreeRegistrySimple();
  ASSERT_FALSE(Sys::LayerTree::CanMoveNodeUp(cfg.registry, cfg.a));
  ASSERT_FALSE(Sys::LayerTree::CanMoveNodeUp(cfg.registry, cfg.b));
  ASSERT_TRUE(Sys::LayerTree::CanMoveNodeUp(cfg.registry, cfg.c));
  ASSERT_FALSE(Sys::LayerTree::CanMoveNodeUp(cfg.registry, cfg.d));
  ASSERT_TRUE(Sys::LayerTree::CanMoveNodeUp(cfg.registry, cfg.e));
  ASSERT_TRUE(Sys::LayerTree::CanMoveNodeUp(cfg.registry, cfg.f));
}

TEST(TreeSystem, CanMoveNodeDown)
{
  const auto cfg = CreateTreeRegistrySimple();
  ASSERT_TRUE(Sys::LayerTree::CanMoveNodeDown(cfg.registry, cfg.a));
  ASSERT_TRUE(Sys::LayerTree::CanMoveNodeDown(cfg.registry, cfg.b));
  ASSERT_TRUE(Sys::LayerTree::CanMoveNodeDown(cfg.registry, cfg.c));
  ASSERT_FALSE(Sys::LayerTree::CanMoveNodeDown(cfg.registry, cfg.d));
  ASSERT_FALSE(Sys::LayerTree::CanMoveNodeDown(cfg.registry, cfg.e));
  ASSERT_FALSE(Sys::LayerTree::CanMoveNodeDown(cfg.registry, cfg.f));
}

TEST(TreeSystem, GetSiblingAbove)
{
  const auto cfg = CreateTreeRegistrySimple();
  ASSERT_EQ(null_entity, Sys::LayerTree::GetSiblingAbove(cfg.registry, cfg.a));
  ASSERT_EQ(null_entity, Sys::LayerTree::GetSiblingAbove(cfg.registry, cfg.b));
  ASSERT_EQ(cfg.b, Sys::LayerTree::GetSiblingAbove(cfg.registry, cfg.c));
  ASSERT_EQ(null_entity, Sys::LayerTree::GetSiblingAbove(cfg.registry, cfg.d));
  ASSERT_EQ(cfg.c, Sys::LayerTree::GetSiblingAbove(cfg.registry, cfg.f));
  ASSERT_EQ(cfg.a, Sys::LayerTree::GetSiblingAbove(cfg.registry, cfg.e));
}

TEST(TreeSystem, GetSiblingBelow)
{
  const auto cfg = CreateTreeRegistrySimple();
  ASSERT_EQ(cfg.e, Sys::LayerTree::GetSiblingBelow(cfg.registry, cfg.a));
  ASSERT_EQ(cfg.c, Sys::LayerTree::GetSiblingBelow(cfg.registry, cfg.b));
  ASSERT_EQ(cfg.f, Sys::LayerTree::GetSiblingBelow(cfg.registry, cfg.c));
  ASSERT_EQ(null_entity, Sys::LayerTree::GetSiblingBelow(cfg.registry, cfg.d));
  ASSERT_EQ(null_entity, Sys::LayerTree::GetSiblingBelow(cfg.registry, cfg.f));
  ASSERT_EQ(null_entity, Sys::LayerTree::GetSiblingBelow(cfg.registry, cfg.e));
}

TEST(TreeSystem, GetSiblingCount)
{
  const auto cfg = CreateTreeRegistrySimple();
  ASSERT_EQ(1u, Sys::LayerTree::GetSiblingCount(cfg.registry, cfg.a));
  ASSERT_EQ(2u, Sys::LayerTree::GetSiblingCount(cfg.registry, cfg.b));
  ASSERT_EQ(2u, Sys::LayerTree::GetSiblingCount(cfg.registry, cfg.c));
  ASSERT_EQ(0u, Sys::LayerTree::GetSiblingCount(cfg.registry, cfg.d));
  ASSERT_EQ(1u, Sys::LayerTree::GetSiblingCount(cfg.registry, cfg.e));
  ASSERT_EQ(2u, Sys::LayerTree::GetSiblingCount(cfg.registry, cfg.f));
}

TEST(TreeSystem, GetChildrenCount)
{
  const auto cfg = CreateTreeRegistrySimple();
  ASSERT_EQ(4u, Sys::LayerTree::GetChildrenCount(cfg.registry, cfg.a));
  ASSERT_EQ(1u, Sys::LayerTree::GetChildrenCount(cfg.registry, cfg.b));
  ASSERT_EQ(0u, Sys::LayerTree::GetChildrenCount(cfg.registry, cfg.c));
  ASSERT_EQ(0u, Sys::LayerTree::GetChildrenCount(cfg.registry, cfg.d));
  ASSERT_EQ(0u, Sys::LayerTree::GetChildrenCount(cfg.registry, cfg.e));
  ASSERT_EQ(0u, Sys::LayerTree::GetChildrenCount(cfg.registry, cfg.f));
}

TEST(TreeSystem, GetGlobalIndex)
{
  const auto cfg = CreateTreeRegistrySimple();
  ASSERT_EQ(0u, Sys::LayerTree::GetGlobalIndex(cfg.registry, cfg.a));
  ASSERT_EQ(1u, Sys::LayerTree::GetGlobalIndex(cfg.registry, cfg.b));
  ASSERT_EQ(2u, Sys::LayerTree::GetGlobalIndex(cfg.registry, cfg.d));
  ASSERT_EQ(3u, Sys::LayerTree::GetGlobalIndex(cfg.registry, cfg.c));
  ASSERT_EQ(4u, Sys::LayerTree::GetGlobalIndex(cfg.registry, cfg.f));
  ASSERT_EQ(5u, Sys::LayerTree::GetGlobalIndex(cfg.registry, cfg.e));
}