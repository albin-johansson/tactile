/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include <algorithm>  // find, iter_swap

#include "common/debug/assert.hpp"
#include "common/util/algorithms.hpp"
#include "common/util/functional.hpp"
#include "model/entity_validation.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _get_parent_layer(const Registry& registry,
                                     const Entity root_layer_entity,
                                     const Entity target_layer_entity) -> Entity
{
  TACTILE_ASSERT(is_group_layer_entity(registry, root_layer_entity));
  TACTILE_ASSERT(is_layer_entity(registry, target_layer_entity));

  const auto& root_layer = registry.get<GroupLayer>(root_layer_entity);

  for (const auto layer_entity: root_layer.children) {
    if (layer_entity == target_layer_entity) {
      return root_layer_entity;
    }
    else if (registry.has<GroupLayer>(layer_entity)) {
      const auto parent_entity =
          _get_parent_layer(registry, layer_entity, target_layer_entity);

      if (parent_entity != kNullEntity) {
        return parent_entity;
      }
    }
  }

  return kNullEntity;
}

[[nodiscard]] auto _get_sibling_count(const Registry& registry,
                                      const GroupLayer& root,
                                      const Entity target_layer_entity) -> Maybe<usize>
{
  for (const auto child_layer_entity: root.children) {
    if (child_layer_entity == target_layer_entity) {
      return root.children.size() - 1;
    }

    if (const auto* child_group = registry.try_get<GroupLayer>(child_layer_entity)) {
      if (const auto count =
              _get_sibling_count(registry, *child_group, target_layer_entity)) {
        return *count;
      }
    }
  }

  return nothing;
}

void _offset_layer(Registry& registry,
                   const Map& map,
                   const Entity layer_entity,
                   const ssize offset)
{
  TACTILE_ASSERT(is_layer_entity(registry, layer_entity));

  const auto parent_layer_entity = get_parent_layer(registry, map, layer_entity);
  auto& parent_layer = registry.get<GroupLayer>(parent_layer_entity);

  const auto begin = parent_layer.children.begin();
  const auto end = parent_layer.children.end();

  if (const auto iter = std::find(begin, end, layer_entity); iter != end) {
    std::iter_swap(iter, iter + offset);
  }
}

}  // namespace

void move_layer_up(Registry& registry, const Map& map, const Entity layer_entity)
{
  TACTILE_ASSERT(is_layer_entity(registry, layer_entity));
  TACTILE_ASSERT(can_move_layer_up(registry,
                                   registry.get<GroupLayer>(map.root_layer),
                                   layer_entity));
  _offset_layer(registry, map, layer_entity, -1);
}

void move_layer_down(Registry& registry, const Map& map, const Entity layer_entity)
{
  TACTILE_ASSERT(is_layer_entity(registry, layer_entity));
  TACTILE_ASSERT(can_move_layer_down(registry,
                                     registry.get<GroupLayer>(map.root_layer),
                                     layer_entity));
  _offset_layer(registry, map, layer_entity, 1);
}

void set_layer_local_index(Registry& registry,
                           const Map& map,
                           const Entity layer_entity,
                           const usize new_index)
{
  TACTILE_ASSERT(is_layer_entity(registry, layer_entity));

  const auto& root = registry.get<GroupLayer>(map.root_layer);

  const auto current_local_index = get_local_layer_index(registry, root, layer_entity);
  const auto steps = udiff(current_local_index.value(), new_index);

  if (new_index < current_local_index) {
    invoke_n(steps, [&] { move_layer_up(registry, map, layer_entity); });
  }
  else {
    invoke_n(steps, [&] { move_layer_down(registry, map, layer_entity); });
  }
}

auto can_move_layer_up(const Registry& registry,
                       const GroupLayer& root,
                       const Entity layer_entity) -> bool
{
  TACTILE_ASSERT(is_layer_entity(registry, layer_entity));
  return get_local_layer_index(registry, root, layer_entity) != 0;
}

auto can_move_layer_down(const Registry& registry,
                         const GroupLayer& root,
                         const Entity layer_entity) -> bool
{
  TACTILE_ASSERT(is_layer_entity(registry, layer_entity));
  const auto local_index = get_local_layer_index(registry, root, layer_entity);
  const auto sibling_count = _get_sibling_count(registry, root, layer_entity);
  return local_index < sibling_count;
}

auto get_local_layer_index(const Registry& registry,
                           const GroupLayer& root,
                           const Entity layer_entity) -> Maybe<usize>
{
  TACTILE_ASSERT(is_layer_entity(registry, layer_entity));

  usize local_index = 0;
  for (const auto child_layer_entity: root.children) {
    if (child_layer_entity == layer_entity) {
      return local_index;
    }

    if (const auto* child_group = registry.try_get<GroupLayer>(child_layer_entity)) {
      if (const auto recursive_local_index =
              get_local_layer_index(registry, *child_group, layer_entity)) {
        return *recursive_local_index;
      }
    }

    ++local_index;
  }

  return nothing;
}

auto get_parent_layer(const Registry& registry, const Map& map, const Entity layer_entity)
    -> Entity
{
  return _get_parent_layer(registry, map.root_layer, layer_entity);
}

}  // namespace tactile::sys
