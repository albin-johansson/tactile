/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "layer_tree_system.hpp"

#include <utility>  // swap

#include <entt/entity/registry.hpp>

#include "core/common/ecs.hpp"
#include "core/components/layers.hpp"
#include "core/components/parent.hpp"
#include "core/systems/context_system.hpp"
#include "misc/assert.hpp"

namespace tactile::sys {
namespace {

void _validate_layer_node_entity([[maybe_unused]] const entt::registry& registry,
                                 [[maybe_unused]] const entt::entity entity)
{
  TACTILE_ASSERT(entity != entt::null);
  TACTILE_ASSERT(registry.all_of<comp::LayerTreeNode>(entity));
  TACTILE_ASSERT(registry.all_of<comp::Parent>(entity));
}

void _swap_indices(entt::registry& registry, const entt::entity a, const entt::entity b)
{
  TACTILE_ASSERT(a != entt::null);
  TACTILE_ASSERT(b != entt::null);

  auto& fst = checked_get<comp::LayerTreeNode>(registry, a);
  auto& snd = checked_get<comp::LayerTreeNode>(registry, b);

  std::swap(fst.index, snd.index);
  sort_layers(registry);
}

void _offset_indices_of_siblings_below(entt::registry& registry,
                                       const entt::entity entity,
                                       const int64 offset)
{
  auto sibling = layer_sibling_below(registry, entity);
  while (sibling != entt::null) {
    auto& siblingNode = checked_get<comp::LayerTreeNode>(registry, sibling);
    const auto newIndex = static_cast<int64>(siblingNode.index) + offset;
    sibling = layer_sibling_below(registry, sibling);
    siblingNode.index = static_cast<usize>(newIndex);
  }
}

void _destroy_child_nodes(entt::registry& registry, const entt::entity entity)
{
  auto& node = checked_get<comp::LayerTreeNode>(registry, entity);
  for (const auto child : node.children) {
    if (registry.all_of<comp::LayerTreeNode>(child)) {
      _destroy_child_nodes(registry, child);
    }

    destroy_entity(registry, child);
  }
}

[[nodiscard]] auto _get_sibling(const entt::registry& registry,
                                const entt::entity entity,
                                const usize targetIndex) -> entt::entity
{
  const auto& parent = checked_get<comp::Parent>(registry, entity);
  if (parent.entity != entt::null) {
    const auto& parentNode = checked_get<comp::LayerTreeNode>(registry, parent.entity);
    for (const auto child : parentNode.children) {
      const auto& childLayer = checked_get<comp::LayerTreeNode>(registry, child);
      if (childLayer.index == targetIndex) {
        return child;
      }
    }
  }
  else {
    for (auto&& [otherEntity, otherNode, otherParent] :
         registry.view<comp::LayerTreeNode, comp::Parent>().each()) {
      if (otherParent.entity == entt::null && otherNode.index == targetIndex) {
        return otherEntity;
      }
    }
  }

  return entt::null;
}

/* Counts the amount of sibling nodes above a layer, including the child
   count of those siblings */
[[nodiscard]] auto _count_siblings_above_including_children(
    const entt::registry& registry,
    const entt::entity entity) -> usize
{
  usize count = 0u;

  auto sibling = layer_sibling_above(registry, entity);
  while (sibling != entt::null) {
    count += layer_children_count(registry, sibling);
    sibling = layer_sibling_above(registry, sibling);
  }

  return count;
}

}  // namespace

void sort_layers(entt::registry& registry)
{
  registry.sort<comp::LayerTreeNode>(
      [&](const entt::entity a, const entt::entity b) {
        const auto fst = layer_global_index(registry, a);
        const auto snd = layer_global_index(registry, b);
        return fst < snd;
      },
      entt::insertion_sort{});

  /* Ensure that nodes hold sorted child node lists */
  for (auto&& [entity, node] : registry.view<comp::LayerTreeNode>().each()) {
    std::sort(node.children.begin(),
              node.children.end(),
              [&](const entt::entity a, const entt::entity b) {
                const auto& fst = checked_get<comp::LayerTreeNode>(registry, a);
                const auto& snd = checked_get<comp::LayerTreeNode>(registry, b);
                return fst.index < snd.index;
              });
  }
}

void increment_layer_indices_of_siblings_below(entt::registry& registry,
                                               const entt::entity entity)
{
  _offset_indices_of_siblings_below(registry, entity, 1);
}

void decrement_layer_indices_of_siblings_below(entt::registry& registry,
                                               const entt::entity entity)
{
  _offset_indices_of_siblings_below(registry, entity, -1);
}

void destroy_layer_node(entt::registry& registry, const entt::entity entity)
{
  _validate_layer_node_entity(registry, entity);

  /* Fix indices of siblings that are below the removed layer */
  decrement_layer_indices_of_siblings_below(registry, entity);

  /* Remove the node from the parent node, if there is one. */
  const auto& parent = checked_get<comp::Parent>(registry, entity);
  if (parent.entity != entt::null) {
    auto& parentNode = checked_get<comp::LayerTreeNode>(registry, parent.entity);
    std::erase(parentNode.children, entity);
  }

  if (registry.all_of<comp::LayerTreeNode>(entity)) {
    _destroy_child_nodes(registry, entity);
  }

  destroy_entity(registry, entity);
  sort_layers(registry);
}

void move_layer_up(entt::registry& registry, const entt::entity entity)
{
  _validate_layer_node_entity(registry, entity);
  TACTILE_ASSERT(can_move_layer_up(registry, entity));

  const auto target = layer_sibling_above(registry, entity);
  TACTILE_ASSERT(target != entt::null);

  _swap_indices(registry, entity, target);
}

void move_layer_down(entt::registry& registry, const entt::entity entity)
{
  _validate_layer_node_entity(registry, entity);
  TACTILE_ASSERT(can_move_layer_down(registry, entity));

  const auto target = layer_sibling_below(registry, entity);
  TACTILE_ASSERT(target != entt::null);

  _swap_indices(registry, entity, target);
}

auto can_move_layer_up(const entt::registry& registry, const entt::entity entity) -> bool
{
  _validate_layer_node_entity(registry, entity);
  return checked_get<comp::LayerTreeNode>(registry, entity).index > 0u;
}

auto can_move_layer_down(const entt::registry& registry, const entt::entity entity)
    -> bool
{
  _validate_layer_node_entity(registry, entity);

  const auto index = checked_get<comp::LayerTreeNode>(registry, entity).index;
  const auto nSiblings = layer_sibling_count(registry, entity);

  return index < nSiblings;
}

auto is_child_layer_node(const entt::registry& registry,
                         const entt::entity parent,
                         const entt::entity entity) -> bool
{
  _validate_layer_node_entity(registry, parent);
  _validate_layer_node_entity(registry, entity);

  for (const auto child : checked_get<comp::LayerTreeNode>(registry, parent).children) {
    if (child == entity) {
      return true;
    }
    else {
      return is_child_layer_node(registry, child, entity);
    }
  }

  return false;
}

auto layer_sibling_above(const entt::registry& registry, const entt::entity entity)
    -> entt::entity
{
  _validate_layer_node_entity(registry, entity);

  const auto index = checked_get<comp::LayerTreeNode>(registry, entity).index;
  if (index != 0u) {
    return _get_sibling(registry, entity, index - 1u);
  }
  else {
    return entt::null;
  }
}

auto layer_sibling_below(const entt::registry& registry, const entt::entity entity)
    -> entt::entity
{
  _validate_layer_node_entity(registry, entity);

  const auto index = checked_get<comp::LayerTreeNode>(registry, entity).index;
  return _get_sibling(registry, entity, index + 1u);
}

auto layer_sibling_count(const entt::registry& registry, const entt::entity entity)
    -> usize
{
  _validate_layer_node_entity(registry, entity);

  const auto& parent = checked_get<comp::Parent>(registry, entity);
  if (parent.entity == entt::null) {
    usize count = 0;

    for (auto&& [otherEntity, otherNode, otherParent] :
         registry.view<comp::LayerTreeNode, comp::Parent>().each()) {
      if (otherEntity != entity && otherParent.entity == entt::null) {
        ++count;
      }
    }

    return count;
  }
  else {
    const auto& parentNode = checked_get<comp::LayerTreeNode>(registry, parent.entity);
    return parentNode.children.size() - 1;  // Exclude the queried layer
  }
}

auto layer_children_count(const entt::registry& registry, const entt::entity entity)
    -> usize
{
  _validate_layer_node_entity(registry, entity);

  const auto& node = checked_get<comp::LayerTreeNode>(registry, entity);
  auto count = node.children.size();

  for (const auto child : node.children) {
    count += layer_children_count(registry, child);
  }

  return count;
}

auto layer_local_index(const entt::registry& registry, const entt::entity entity) -> usize
{
  _validate_layer_node_entity(registry, entity);

  const auto& node = checked_get<comp::LayerTreeNode>(registry, entity);
  return node.index;
}

auto layer_global_index(const entt::registry& registry, const entt::entity entity)
    -> usize
{
  _validate_layer_node_entity(registry, entity);

  const auto& node = checked_get<comp::LayerTreeNode>(registry, entity);
  const auto& parent = checked_get<comp::Parent>(registry, entity);

  const auto base =
      node.index + _count_siblings_above_including_children(registry, entity);

  if (parent.entity == entt::null) {
    return base;
  }
  else {
    return base + layer_global_index(registry, parent.entity) + 1u;
  }
}

}  // namespace tactile::sys