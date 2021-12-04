#include "layer_tree_system.hpp"

#include <cassert>  // assert
#include <utility>  // swap

#include "core/components/layer.hpp"
#include "core/components/parent.hpp"

namespace Tactile::Sys::LayerTree {
namespace {

void SwapIndices(entt::registry& registry, const entt::entity a, const entt::entity b)
{
  assert(a != entt::null);
  assert(b != entt::null);

  auto& fst = registry.get<LayerTreeNode>(a);
  auto& snd = registry.get<LayerTreeNode>(b);

  std::swap(fst.index, snd.index);
  SortNodes(registry);
}

void OffsetIndicesOfSiblingsBelow(entt::registry& registry,
                                  const entt::entity entity,
                                  const int64 offset)
{
  auto sibling = GetSiblingBelow(registry, entity);
  while (sibling != entt::null) {
    auto& siblingNode = registry.get<LayerTreeNode>(sibling);
    const auto newIndex = static_cast<int64>(siblingNode.index) + offset;
    sibling = GetSiblingBelow(registry, sibling);
    siblingNode.index = static_cast<usize>(newIndex);
  }
}

void DestroyChildNodes(entt::registry& registry, const entt::entity entity)
{
  auto& node = registry.get<LayerTreeNode>(entity);
  for (const auto child : node.children) {
    if (registry.all_of<LayerTreeNode>(child)) {
      DestroyChildNodes(registry, child);
    }
    registry.destroy(child);
  }
}

[[nodiscard]] auto GetSibling(const entt::registry& registry,
                              const entt::entity entity,
                              const usize targetIndex) -> entt::entity
{
  const auto& parent = registry.get<Parent>(entity);
  if (parent.entity != entt::null) {
    const auto& parentNode = registry.get<LayerTreeNode>(parent.entity);
    for (const auto child : parentNode.children) {
      const auto& childLayer = registry.get<LayerTreeNode>(child);
      if (childLayer.index == targetIndex) {
        return child;
      }
    }
  }
  else {
    for (auto&& [otherEntity, otherNode, otherParent] :
         registry.view<LayerTreeNode, Parent>().each()) {
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

void SortNodes(entt::registry& registry)
{
  registry.sort<LayerTreeNode>(
      [&](const entt::entity a, const entt::entity b) {
        const auto fst = GetGlobalIndex(registry, a);
        const auto snd = GetGlobalIndex(registry, b);
        return fst < snd;
      },
      entt::insertion_sort{});

  /* Ensure that nodes hold sorted child node lists */
  for (auto&& [entity, node] : registry.view<LayerTreeNode>().each()) {
    std::sort(node.children.begin(),
              node.children.end(),
              [&](const entt::entity a, const entt::entity b) {
                const auto& fst = registry.get<LayerTreeNode>(a);
                const auto& snd = registry.get<LayerTreeNode>(b);
                return fst.index < snd.index;
              });
  }
}

void IncrementIndicesOfSiblingsBelow(entt::registry& registry, const entt::entity entity)
{
  OffsetIndicesOfSiblingsBelow(registry, entity, 1);
}

void DecrementIndicesOfSiblingsBelow(entt::registry& registry, const entt::entity entity)
{
  OffsetIndicesOfSiblingsBelow(registry, entity, -1);
}

void DestroyNode(entt::registry& registry, const entt::entity entity)
{
  assert(entity != entt::null);
  assert(registry.all_of<LayerTreeNode>(entity));

  /* Fix indices of siblings that are below the removed layer */
  DecrementIndicesOfSiblingsBelow(registry, entity);

  /* Remove the node from the parent node, if there is one. */
  const auto& parent = registry.get<Parent>(entity);
  if (parent.entity != entt::null) {
    auto& parentNode = registry.get<LayerTreeNode>(parent.entity);
    std::erase(parentNode.children, entity);
  }

  if (registry.all_of<LayerTreeNode>(entity)) {
    DestroyChildNodes(registry, entity);
  }

  registry.destroy(entity);
  SortNodes(registry);
}

void MoveNodeUp(entt::registry& registry, const entt::entity entity)
{
  assert(entity != entt::null);
  assert(registry.all_of<LayerTreeNode>(entity));
  assert(CanMoveNodeUp(registry, entity));

  const auto target = GetSiblingAbove(registry, entity);
  assert(target != entt::null);

  SwapIndices(registry, entity, target);
}

void MoveNodeDown(entt::registry& registry, const entt::entity entity)
{
  assert(entity != entt::null);
  assert(registry.all_of<LayerTreeNode>(entity));
  assert(CanMoveNodeDown(registry, entity));

  const auto target = GetSiblingBelow(registry, entity);
  assert(target != entt::null);

  SwapIndices(registry, entity, target);
}

auto CanMoveNodeUp(const entt::registry& registry, const entt::entity entity) -> bool
{
  assert(entity != entt::null);
  assert(registry.all_of<LayerTreeNode>(entity));

  return registry.get<LayerTreeNode>(entity).index > 0u;
}

auto CanMoveNodeDown(const entt::registry& registry, const entt::entity entity) -> bool
{
  assert(entity != entt::null);
  assert(registry.all_of<LayerTreeNode>(entity));

  const auto index = registry.get<LayerTreeNode>(entity).index;
  const auto nSiblings = GetSiblingCount(registry, entity);

  return index < nSiblings;
}

auto IsChildNode(const entt::registry& registry,
                 const entt::entity parent,
                 const entt::entity entity) -> bool
{
  assert(parent != entt::null);
  assert(entity != entt::null);
  assert(registry.all_of<LayerTreeNode>(parent));
  assert(registry.all_of<LayerTreeNode>(entity));

  for (const auto child : registry.get<LayerTreeNode>(parent).children) {
    if (child == entity) {
      return true;
    }
    else {
      return IsChildNode(registry, child, entity);
    }
  }

  return false;
}

auto GetSiblingAbove(const entt::registry& registry, const entt::entity entity)
    -> entt::entity
{
  assert(entity != entt::null);
  assert(registry.all_of<LayerTreeNode>(entity));

  const auto index = registry.get<LayerTreeNode>(entity).index;
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
  assert(registry.all_of<LayerTreeNode>(entity));

  const auto index = registry.get<LayerTreeNode>(entity).index;
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
         registry.view<LayerTreeNode, Parent>().each()) {
      if (otherEntity != entity && otherParent.entity == entt::null) {
        ++count;
      }
    }

    return count;
  }
  else {
    const auto& parentNode = registry.get<LayerTreeNode>(parent.entity);
    return parentNode.children.size() - 1;  // Exclude the queried layer
  }
}

auto GetChildrenCount(const entt::registry& registry, const entt::entity entity) -> usize
{
  assert(entity != entt::null);
  assert(registry.all_of<LayerTreeNode>(entity));

  const auto& node = registry.get<LayerTreeNode>(entity);
  auto count = node.children.size();

  for (const auto child : node.children) {
    count += GetChildrenCount(registry, child);
  }

  return count;
}

auto GetGlobalIndex(const entt::registry& registry, const entt::entity entity) -> usize
{
  assert(entity != entt::null);
  assert(registry.all_of<LayerTreeNode>(entity));
  assert(registry.all_of<Parent>(entity));

  const auto& node = registry.get<LayerTreeNode>(entity);
  const auto& parent = registry.get<Parent>(entity);

  const auto base = node.index + CountSiblingsAboveInclusiveChildren(registry, entity);

  if (parent.entity == entt::null) {
    return base;
  }
  else {
    return base + GetGlobalIndex(registry, parent.entity) + 1u;
  }
}

}  // namespace Tactile::Sys::LayerTree