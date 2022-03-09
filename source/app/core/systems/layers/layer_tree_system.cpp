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

#include "core/components/layer.hpp"
#include "core/components/parent.hpp"
#include "misc/assert.hpp"

namespace tactile::sys {
namespace {

void _validate_layer_node_entity(const entt::registry& registry,
                                 const entt::entity entity)
{
  TACTILE_ASSERT(entity != entt::null);
  TACTILE_ASSERT(registry.all_of<comp::layer_tree_node>(entity));
  TACTILE_ASSERT(registry.all_of<comp::Parent>(entity));
}

void _swap_indices(entt::registry& registry, const entt::entity a, const entt::entity b)
{
  TACTILE_ASSERT(a != entt::null);
  TACTILE_ASSERT(b != entt::null);

  auto& fst = registry.get<comp::layer_tree_node>(a);
  auto& snd = registry.get<comp::layer_tree_node>(b);

  std::swap(fst.index, snd.index);
  sort_layers(registry);
}

void _offset_indices_of_siblings_below(entt::registry& registry,
                                       const entt::entity entity,
                                       const int64 offset)
{
  auto sibling = layer_sibling_below(registry, entity);
  while (sibling != entt::null) {
    auto& siblingNode = registry.get<comp::layer_tree_node>(sibling);
    const auto newIndex = static_cast<int64>(siblingNode.index) + offset;
    sibling = layer_sibling_below(registry, sibling);
    siblingNode.index = static_cast<usize>(newIndex);
  }
}

void _destroy_child_nodes(entt::registry& registry, const entt::entity entity)
{
  auto& node = registry.get<comp::layer_tree_node>(entity);
  for (const auto child : node.children) {
    if (registry.all_of<comp::layer_tree_node>(child)) {
      _destroy_child_nodes(registry, child);
    }
    registry.destroy(child);
  }
}

[[nodiscard]] auto _get_sibling(const entt::registry& registry,
                                const entt::entity entity,
                                const usize targetIndex) -> entt::entity
{
  const auto& parent = registry.get<comp::Parent>(entity);
  if (parent.entity != entt::null) {
    const auto& parentNode = registry.get<comp::layer_tree_node>(parent.entity);
    for (const auto child : parentNode.children) {
      const auto& childLayer = registry.get<comp::layer_tree_node>(child);
      if (childLayer.index == targetIndex) {
        return child;
      }
    }
  }
  else {
    for (auto&& [otherEntity, otherNode, otherParent] :
         registry.view<comp::layer_tree_node, comp::Parent>().each()) {
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
  registry.sort<comp::layer_tree_node>(
      [&](const entt::entity a, const entt::entity b) {
        const auto fst = layer_global_index(registry, a);
        const auto snd = layer_global_index(registry, b);
        return fst < snd;
      },
      entt::insertion_sort{});

  /* Ensure that nodes hold sorted child node lists */
  for (auto&& [entity, node] : registry.view<comp::layer_tree_node>().each()) {
    std::sort(node.children.begin(),
              node.children.end(),
              [&](const entt::entity a, const entt::entity b) {
                const auto& fst = registry.get<comp::layer_tree_node>(a);
                const auto& snd = registry.get<comp::layer_tree_node>(b);
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
  const auto& parent = registry.get<comp::Parent>(entity);
  if (parent.entity != entt::null) {
    auto& parentNode = registry.get<comp::layer_tree_node>(parent.entity);
    std::erase(parentNode.children, entity);
  }

  if (registry.all_of<comp::layer_tree_node>(entity)) {
    _destroy_child_nodes(registry, entity);
  }

  registry.destroy(entity);
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
  return registry.get<comp::layer_tree_node>(entity).index > 0u;
}

auto can_move_layer_down(const entt::registry& registry, const entt::entity entity)
    -> bool
{
  _validate_layer_node_entity(registry, entity);

  const auto index = registry.get<comp::layer_tree_node>(entity).index;
  const auto nSiblings = layer_sibling_count(registry, entity);

  return index < nSiblings;
}

auto is_child_layer_node(const entt::registry& registry,
                         const entt::entity parent,
                         const entt::entity entity) -> bool
{
  _validate_layer_node_entity(registry, parent);
  _validate_layer_node_entity(registry, entity);

  for (const auto child : registry.get<comp::layer_tree_node>(parent).children) {
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

  const auto index = registry.get<comp::layer_tree_node>(entity).index;
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

  const auto index = registry.get<comp::layer_tree_node>(entity).index;
  return _get_sibling(registry, entity, index + 1u);
}

auto layer_sibling_count(const entt::registry& registry, const entt::entity entity)
    -> usize
{
  _validate_layer_node_entity(registry, entity);

  const auto& parent = registry.get<comp::Parent>(entity);
  if (parent.entity == entt::null) {
    usize count = 0;

    for (auto&& [otherEntity, otherNode, otherParent] :
         registry.view<comp::layer_tree_node, comp::Parent>().each()) {
      if (otherEntity != entity && otherParent.entity == entt::null) {
        ++count;
      }
    }

    return count;
  }
  else {
    const auto& parentNode = registry.get<comp::layer_tree_node>(parent.entity);
    return parentNode.children.size() - 1;  // Exclude the queried layer
  }
}

auto layer_children_count(const entt::registry& registry, const entt::entity entity)
    -> usize
{
  _validate_layer_node_entity(registry, entity);

  const auto& node = registry.get<comp::layer_tree_node>(entity);
  auto count = node.children.size();

  for (const auto child : node.children) {
    count += layer_children_count(registry, child);
  }

  return count;
}

auto layer_local_index(const entt::registry& registry, const entt::entity entity) -> usize
{
  _validate_layer_node_entity(registry, entity);

  const auto& node = registry.get<comp::layer_tree_node>(entity);
  return node.index;
}

auto layer_global_index(const entt::registry& registry, const entt::entity entity)
    -> usize
{
  _validate_layer_node_entity(registry, entity);

  const auto& node = registry.get<comp::layer_tree_node>(entity);
  const auto& parent = registry.get<comp::Parent>(entity);

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