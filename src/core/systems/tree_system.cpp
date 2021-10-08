#include "tree_system.hpp"

#include <cassert>  // assert

#include "core/components/parent.hpp"
#include "core/components/tree.hpp"

namespace Tactile::Sys::Tree {
namespace {

[[nodiscard]] auto GetSibling(const entt::registry& registry,
                              const entt::entity entity,
                              const usize targetIndex) -> entt::entity
{
  const auto& parent = registry.get<Parent>(entity);
  if (parent.entity != entt::null) {
    const auto& parentNode = registry.get<TreeNode>(parent.entity);
    for (const auto child : parentNode.children) {
      const auto& childLayer = registry.get<TreeNode>(child);
      if (childLayer.index == targetIndex) {
        return child;
      }
    }
  }
  else {
    for (auto&& [otherEntity, otherNode, otherParent] :
         registry.view<TreeNode, Parent>().each())
    {
      if (otherParent.entity == entt::null && otherNode.index == targetIndex) {
        return otherEntity;
      }
    }
  }

  return entt::null;
}

/* Counts the amount of sibling nodes above a layer, including the child
   count of those siblings */
[[nodiscard]] auto CountSiblingsAboveInclusiveChildren(const entt::registry& registry,
                                                       const entt::entity entity) -> usize
{
  usize count = 0u;

  auto sibling = GetSiblingAbove(registry, entity);
  while (sibling != entt::null) {
    count += GetChildrenCount(registry, sibling);
    sibling = GetSiblingAbove(registry, sibling);
  }

  return count;
}

}  // namespace

auto GetSiblingAbove(const entt::registry& registry, const entt::entity entity)
    -> entt::entity
{
  assert(entity != entt::null);
  assert(registry.all_of<TreeNode>(entity));

  const auto index = registry.get<TreeNode>(entity).index;
  if (index != 0u) {
    return GetSibling(registry, entity, index - 1u);
  }
  else {
    return entt::null;
  }
}

auto GetSiblingBelow(const entt::registry& registry, const entt::entity entity)
    -> entt::entity
{
  assert(entity != entt::null);
  assert(registry.all_of<TreeNode>(entity));

  const auto index = registry.get<TreeNode>(entity).index;
  return GetSibling(registry, entity, index + 1u);
}

auto GetSiblingCount(const entt::registry& registry, const entt::entity entity) -> usize
{
  assert(entity != entt::null);
  assert(registry.all_of<Parent>(entity));

  const auto& parent = registry.get<Parent>(entity);

  if (parent.entity == entt::null) {
    usize count = 0;

    for (auto&& [otherEntity, otherNode, otherParent] :
         registry.view<TreeNode, Parent>().each())
    {
      if (otherEntity != entity && otherParent.entity == entt::null) {
        ++count;
      }
    }

    return count;
  }
  else {
    const auto& parentNode = registry.get<TreeNode>(parent.entity);
    return parentNode.children.size() - 1;  // Exclude the queried layer
  }
}

auto GetChildrenCount(const entt::registry& registry, const entt::entity entity) -> usize
{
  assert(entity != entt::null);
  assert(registry.all_of<TreeNode>(entity));

  const auto& node = registry.get<TreeNode>(entity);
  auto count = node.children.size();

  for (const auto child : node.children) {
    count += GetChildrenCount(registry, child);
  }

  return count;
}

auto GetGlobalIndex(const entt::registry& registry, const entt::entity entity) -> usize
{
  assert(entity != entt::null);
  assert(registry.all_of<TreeNode>(entity));
  assert(registry.all_of<Parent>(entity));

  const auto& node = registry.get<TreeNode>(entity);
  const auto& parent = registry.get<Parent>(entity);

  const auto base = node.index + CountSiblingsAboveInclusiveChildren(registry, entity);

  if (parent.entity == entt::null) {
    return base;
  }
  else {
    return base + GetGlobalIndex(registry, parent.entity) + 1u;
  }
}

}  // namespace Tactile::Sys::Tree