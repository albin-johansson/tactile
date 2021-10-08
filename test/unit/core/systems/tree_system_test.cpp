#include "core/systems/tree_system.hpp"

#include <gtest/gtest.h>

#include "core/components/parent.hpp"
#include "core/components/tree.hpp"
#include "core/systems/registry_factory_system.hpp"

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
    auto& node = cfg.registry.emplace<TreeNode>(cfg.a, 0u);
    node.children.push_back(cfg.b);
    node.children.push_back(cfg.c);
    node.children.push_back(cfg.f);
  }

  {
    auto& node = cfg.registry.emplace<TreeNode>(cfg.b, 0u);
    node.children.push_back(cfg.d);
  }

  cfg.registry.emplace<TreeNode>(cfg.c, 1u);
  cfg.registry.emplace<TreeNode>(cfg.d, 0u);
  cfg.registry.emplace<TreeNode>(cfg.e, 1u);
  cfg.registry.emplace<TreeNode>(cfg.f, 2u);

  cfg.registry.emplace<TreeLeaf>(cfg.c);
  cfg.registry.emplace<TreeLeaf>(cfg.d);
  cfg.registry.emplace<TreeLeaf>(cfg.e);
  cfg.registry.emplace<TreeLeaf>(cfg.f);

  Sys::Tree::SortNodes(cfg.registry);

  return cfg;
}

}  // namespace

TEST(TreeSystem, MoveNodeUp)
{
  auto cfg = CreateTreeRegistrySimple();

  ASSERT_EQ(0u, cfg.registry.get<TreeNode>(cfg.a).index);
  ASSERT_EQ(1u, cfg.registry.get<TreeNode>(cfg.e).index);
  ASSERT_EQ(0u, Sys::Tree::GetGlobalIndex(cfg.registry, cfg.a));
  ASSERT_EQ(5u, Sys::Tree::GetGlobalIndex(cfg.registry, cfg.e));
  Sys::Tree::MoveNodeUp(cfg.registry, cfg.e);

  // New tree
  // > e
  // > a
  //   |__b
  //      |__d
  //   |__c
  //   |__f
  ASSERT_EQ(1u, cfg.registry.get<TreeNode>(cfg.a).index);
  ASSERT_EQ(0u, cfg.registry.get<TreeNode>(cfg.e).index);
  ASSERT_EQ(1u, Sys::Tree::GetGlobalIndex(cfg.registry, cfg.a));
  ASSERT_EQ(0u, Sys::Tree::GetGlobalIndex(cfg.registry, cfg.e));

  ASSERT_EQ(0u, cfg.registry.get<TreeNode>(cfg.b).index);
  ASSERT_EQ(1u, cfg.registry.get<TreeNode>(cfg.c).index);
  ASSERT_EQ(2u, Sys::Tree::GetGlobalIndex(cfg.registry, cfg.b));
  ASSERT_EQ(4u, Sys::Tree::GetGlobalIndex(cfg.registry, cfg.c));
  Sys::Tree::MoveNodeUp(cfg.registry, cfg.c);

  // New tree
  // > e
  // > a
  //   |__c
  //   |__b
  //      |__d
  //   |__f
  ASSERT_EQ(1u, cfg.registry.get<TreeNode>(cfg.b).index);
  ASSERT_EQ(0u, cfg.registry.get<TreeNode>(cfg.c).index);
  ASSERT_EQ(3u, Sys::Tree::GetGlobalIndex(cfg.registry, cfg.b));
  ASSERT_EQ(2u, Sys::Tree::GetGlobalIndex(cfg.registry, cfg.c));
}

TEST(TreeSystem, MoveNodeDown)
{
  auto cfg = CreateTreeRegistrySimple();

  ASSERT_EQ(0u, cfg.registry.get<TreeNode>(cfg.b).index);
  ASSERT_EQ(1u, cfg.registry.get<TreeNode>(cfg.c).index);
  ASSERT_EQ(1u, Sys::Tree::GetGlobalIndex(cfg.registry, cfg.b));
  ASSERT_EQ(3u, Sys::Tree::GetGlobalIndex(cfg.registry, cfg.c));
  Sys::Tree::MoveNodeDown(cfg.registry, cfg.b);
  // New tree
  // > a
  //   |__c
  //   |__b
  //      |__d
  //   |__f
  // > e
  ASSERT_EQ(1u, cfg.registry.get<TreeNode>(cfg.b).index);
  ASSERT_EQ(0u, cfg.registry.get<TreeNode>(cfg.c).index);
  ASSERT_EQ(2u, Sys::Tree::GetGlobalIndex(cfg.registry, cfg.b));
  ASSERT_EQ(1u, Sys::Tree::GetGlobalIndex(cfg.registry, cfg.c));

  ASSERT_EQ(0u, cfg.registry.get<TreeNode>(cfg.a).index);
  ASSERT_EQ(1u, cfg.registry.get<TreeNode>(cfg.e).index);
  ASSERT_EQ(0u, Sys::Tree::GetGlobalIndex(cfg.registry, cfg.a));
  ASSERT_EQ(5u, Sys::Tree::GetGlobalIndex(cfg.registry, cfg.e));
  Sys::Tree::MoveNodeDown(cfg.registry, cfg.a);
  // New tree
  // > e
  // > a
  //   |__c
  //   |__b
  //      |__d
  //   |__f
  ASSERT_EQ(1u, cfg.registry.get<TreeNode>(cfg.a).index);
  ASSERT_EQ(0u, cfg.registry.get<TreeNode>(cfg.e).index);
  ASSERT_EQ(1u, Sys::Tree::GetGlobalIndex(cfg.registry, cfg.a));
  ASSERT_EQ(0u, Sys::Tree::GetGlobalIndex(cfg.registry, cfg.e));
}

TEST(TreeSystem, CanMoveNodeUp)
{
  const auto cfg = CreateTreeRegistrySimple();
  ASSERT_FALSE(Sys::Tree::CanMoveNodeUp(cfg.registry, cfg.a));
  ASSERT_FALSE(Sys::Tree::CanMoveNodeUp(cfg.registry, cfg.b));
  ASSERT_TRUE(Sys::Tree::CanMoveNodeUp(cfg.registry, cfg.c));
  ASSERT_FALSE(Sys::Tree::CanMoveNodeUp(cfg.registry, cfg.d));
  ASSERT_TRUE(Sys::Tree::CanMoveNodeUp(cfg.registry, cfg.e));
  ASSERT_TRUE(Sys::Tree::CanMoveNodeUp(cfg.registry, cfg.f));
}

TEST(TreeSystem, CanMoveNodeDown)
{
  const auto cfg = CreateTreeRegistrySimple();
  ASSERT_TRUE(Sys::Tree::CanMoveNodeDown(cfg.registry, cfg.a));
  ASSERT_TRUE(Sys::Tree::CanMoveNodeDown(cfg.registry, cfg.b));
  ASSERT_TRUE(Sys::Tree::CanMoveNodeDown(cfg.registry, cfg.c));
  ASSERT_FALSE(Sys::Tree::CanMoveNodeDown(cfg.registry, cfg.d));
  ASSERT_FALSE(Sys::Tree::CanMoveNodeDown(cfg.registry, cfg.e));
  ASSERT_FALSE(Sys::Tree::CanMoveNodeDown(cfg.registry, cfg.f));
}

TEST(TreeSystem, GetSiblingAbove)
{
  const auto cfg = CreateTreeRegistrySimple();
  ASSERT_EQ(null_entity, Sys::Tree::GetSiblingAbove(cfg.registry, cfg.a));
  ASSERT_EQ(null_entity, Sys::Tree::GetSiblingAbove(cfg.registry, cfg.b));
  ASSERT_EQ(cfg.b, Sys::Tree::GetSiblingAbove(cfg.registry, cfg.c));
  ASSERT_EQ(null_entity, Sys::Tree::GetSiblingAbove(cfg.registry, cfg.d));
  ASSERT_EQ(cfg.c, Sys::Tree::GetSiblingAbove(cfg.registry, cfg.f));
  ASSERT_EQ(cfg.a, Sys::Tree::GetSiblingAbove(cfg.registry, cfg.e));
}

TEST(TreeSystem, GetSiblingBelow)
{
  const auto cfg = CreateTreeRegistrySimple();
  ASSERT_EQ(cfg.e, Sys::Tree::GetSiblingBelow(cfg.registry, cfg.a));
  ASSERT_EQ(cfg.c, Sys::Tree::GetSiblingBelow(cfg.registry, cfg.b));
  ASSERT_EQ(cfg.f, Sys::Tree::GetSiblingBelow(cfg.registry, cfg.c));
  ASSERT_EQ(null_entity, Sys::Tree::GetSiblingBelow(cfg.registry, cfg.d));
  ASSERT_EQ(null_entity, Sys::Tree::GetSiblingBelow(cfg.registry, cfg.f));
  ASSERT_EQ(null_entity, Sys::Tree::GetSiblingBelow(cfg.registry, cfg.e));
}

TEST(TreeSystem, GetSiblingCount)
{
  const auto cfg = CreateTreeRegistrySimple();
  ASSERT_EQ(1u, Sys::Tree::GetSiblingCount(cfg.registry, cfg.a));
  ASSERT_EQ(2u, Sys::Tree::GetSiblingCount(cfg.registry, cfg.b));
  ASSERT_EQ(2u, Sys::Tree::GetSiblingCount(cfg.registry, cfg.c));
  ASSERT_EQ(0u, Sys::Tree::GetSiblingCount(cfg.registry, cfg.d));
  ASSERT_EQ(1u, Sys::Tree::GetSiblingCount(cfg.registry, cfg.e));
  ASSERT_EQ(2u, Sys::Tree::GetSiblingCount(cfg.registry, cfg.f));
}

TEST(TreeSystem, GetChildrenCount)
{
  const auto cfg = CreateTreeRegistrySimple();
  ASSERT_EQ(4u, Sys::Tree::GetChildrenCount(cfg.registry, cfg.a));
  ASSERT_EQ(1u, Sys::Tree::GetChildrenCount(cfg.registry, cfg.b));
  ASSERT_EQ(0u, Sys::Tree::GetChildrenCount(cfg.registry, cfg.c));
  ASSERT_EQ(0u, Sys::Tree::GetChildrenCount(cfg.registry, cfg.d));
  ASSERT_EQ(0u, Sys::Tree::GetChildrenCount(cfg.registry, cfg.e));
  ASSERT_EQ(0u, Sys::Tree::GetChildrenCount(cfg.registry, cfg.f));
}

TEST(TreeSystem, GetGlobalIndex)
{
  const auto cfg = CreateTreeRegistrySimple();
  ASSERT_EQ(0u, Sys::Tree::GetGlobalIndex(cfg.registry, cfg.a));
  ASSERT_EQ(1u, Sys::Tree::GetGlobalIndex(cfg.registry, cfg.b));
  ASSERT_EQ(2u, Sys::Tree::GetGlobalIndex(cfg.registry, cfg.d));
  ASSERT_EQ(3u, Sys::Tree::GetGlobalIndex(cfg.registry, cfg.c));
  ASSERT_EQ(4u, Sys::Tree::GetGlobalIndex(cfg.registry, cfg.f));
  ASSERT_EQ(5u, Sys::Tree::GetGlobalIndex(cfg.registry, cfg.e));
}