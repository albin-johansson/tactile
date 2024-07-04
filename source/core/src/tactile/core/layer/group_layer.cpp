// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/group_layer.hpp"

#include <algorithm>  // find, iter_swap
#include <iterator>   // distance
#include <tuple>      // ignore
#include <utility>    // cmp_less

#include "tactile/base/numeric/saturate_cast.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/layer/layer.hpp"
#include "tactile/core/layer/object_layer.hpp"
#include "tactile/core/layer/tile_layer.hpp"
#include "tactile/core/meta/meta.hpp"

namespace tactile {
namespace group_layer_impl {

struct FindLayerResult final
{
  CGroupLayer* parent_layer;
  ssize layer_index;
};

struct FindLayerConstResult final
{
  const CGroupLayer* parent_layer;
  ssize layer_index;
};

[[nodiscard]]
auto find_layer(Registry& registry,
                const EntityID root_layer_id,
                const EntityID target_layer_id) -> Optional<FindLayerResult>
{
  const auto parent_id =
      find_parent_layer(registry, root_layer_id, target_layer_id);

  if (parent_id == kInvalidEntity) {
    return kNone;
  }

  auto& parent_layer = registry.get<CGroupLayer>(parent_id);

  // We already found the layer, so no check needed here.
  const auto iter = std::ranges::find(parent_layer.layers, target_layer_id);
  TACTILE_ASSERT(iter != parent_layer.layers.end());

  const auto layer_index = std::distance(parent_layer.layers.begin(), iter);

  return FindLayerResult {
    .parent_layer = &parent_layer,
    .layer_index = layer_index,
  };
}

[[nodiscard]]
auto find_layer(const Registry& registry,
                const EntityID root_layer_id,
                const EntityID target_layer_id)
    -> Optional<FindLayerConstResult>
{
  const auto parent_id =
      find_parent_layer(registry, root_layer_id, target_layer_id);

  if (parent_id == kInvalidEntity) {
    return kNone;
  }

  const auto& parent_layer = registry.get<CGroupLayer>(parent_id);

  // We already found the layer, so no check needed here.
  const auto iter = std::ranges::find(parent_layer.layers, target_layer_id);
  TACTILE_ASSERT(iter != parent_layer.layers.end());

  const auto layer_index = std::distance(parent_layer.layers.begin(), iter);

  return FindLayerConstResult {
    .parent_layer = &parent_layer,
    .layer_index = layer_index,
  };
}

[[nodiscard]]
auto can_move_layer(const Registry& registry,
                    const EntityID root_layer_id,
                    const EntityID target_layer_id,
                    const ssize offset) -> bool
{
  const auto find_result = find_layer(registry, root_layer_id, target_layer_id);

  if (!find_result.has_value()) {
    return false;
  }

  const auto new_index = find_result->layer_index + offset;
  const auto layer_count = find_result->parent_layer->layers.size();

  return new_index >= 0 && std::cmp_less(new_index, layer_count);
}

[[nodiscard]]
auto get_global_layer_index(const Registry& registry,
                            const EntityID root_layer_id,
                            const EntityID target_layer_id,
                            usize& index) -> bool
{
  const auto& root_layer = registry.get<CGroupLayer>(root_layer_id);

  for (const auto layer_id : root_layer.layers) {
    if (layer_id == target_layer_id) {
      return true;
    }

    ++index;

    if (is_group_layer(registry, layer_id)) {
      if (get_global_layer_index(registry, layer_id, target_layer_id, index)) {
        return true;
      }
    }
  }

  return false;
}

}  // namespace group_layer_impl

auto is_group_layer(const Registry& registry, const EntityID entity) -> bool
{
  return registry.has<CMeta>(entity) &&   //
         registry.has<CLayer>(entity) &&  //
         registry.has<CGroupLayer>(entity);
}

auto make_group_layer(Registry& registry) -> EntityID
{
  const auto layer_entity = make_unspecialized_layer(registry);

  registry.add<CGroupLayer>(layer_entity);

  TACTILE_ASSERT(is_group_layer(registry, layer_entity));
  return layer_entity;
}

void destroy_group_layer(Registry& registry, const EntityID group_layer_id)
{
  TACTILE_ASSERT(is_group_layer(registry, group_layer_id));

  const auto& group_layer = registry.get<CGroupLayer>(group_layer_id);
  for (const auto layer_id : group_layer.layers) {
    if (registry.has<CGroupLayer>(layer_id)) {
      destroy_group_layer(registry, layer_id);
    }
    else if (registry.has<CTileLayer>(layer_id)) {
      destroy_tile_layer(registry, layer_id);
    }
    else if (registry.has<CObjectLayer>(layer_id)) {
      destroy_object_layer(registry, layer_id);
    }
    else {
      TACTILE_ASSERT_MSG(false, "invalid layer entity");
    }
  }

  registry.destroy(group_layer_id);
}

auto find_parent_layer(const Registry& registry,
                       const EntityID root_layer_id,
                       const EntityID target_layer_id) -> EntityID
{
  TACTILE_ASSERT(is_group_layer(registry, root_layer_id));
  TACTILE_ASSERT(is_layer(registry, target_layer_id));

  const auto& root_layer = registry.get<CGroupLayer>(root_layer_id);

  for (const auto layer_id : root_layer.layers) {
    if (layer_id == target_layer_id) {
      return root_layer_id;
    }

    if (!is_group_layer(registry, layer_id)) {
      continue;
    }

    const auto found_id =
        find_parent_layer(registry, layer_id, target_layer_id);
    if (found_id != kInvalidEntity) {
      return found_id;
    }
  }

  return kInvalidEntity;
}

auto get_local_layer_index(const Registry& registry,
                           const EntityID root_layer_id,
                           const EntityID target_layer_id) -> Optional<usize>
{
  TACTILE_ASSERT(is_group_layer(registry, root_layer_id));
  TACTILE_ASSERT(is_layer(registry, target_layer_id));

  return group_layer_impl::find_layer(registry, root_layer_id, target_layer_id)
      .transform([](const group_layer_impl::FindLayerConstResult& result) {
        return saturate_cast<usize>(result.layer_index);
      });
}

auto get_global_layer_index(const Registry& registry,
                            const EntityID root_layer_id,
                            const EntityID target_layer_id) -> Optional<usize>
{
  TACTILE_ASSERT(is_group_layer(registry, root_layer_id));
  TACTILE_ASSERT(is_layer(registry, target_layer_id));

  usize index = 0;

  if (group_layer_impl::get_global_layer_index(registry,
                                               root_layer_id,
                                               target_layer_id,
                                               index)) {
    return index;
  }

  return kNone;
}

void move_layer_down(Registry& registry,
                     const EntityID root_layer_id,
                     const EntityID target_layer_id)
{
  TACTILE_ASSERT(is_group_layer(registry, root_layer_id));
  TACTILE_ASSERT(is_layer(registry, target_layer_id));
  TACTILE_ASSERT(can_move_layer_down(registry, root_layer_id, target_layer_id));

  std::ignore =
      group_layer_impl::find_layer(registry, root_layer_id, target_layer_id)
          .transform([](const group_layer_impl::FindLayerResult& result) {
            const auto layers_begin = result.parent_layer->layers.begin();

            const auto curr_pos = layers_begin + result.layer_index;
            const auto new_pos = curr_pos + 1;

            std::iter_swap(curr_pos, new_pos);
            return 0;
          });
}

void move_layer_up(Registry& registry,
                   const EntityID root_layer_id,
                   const EntityID target_layer_id)
{
  TACTILE_ASSERT(is_group_layer(registry, root_layer_id));
  TACTILE_ASSERT(is_layer(registry, target_layer_id));
  TACTILE_ASSERT(can_move_layer_up(registry, root_layer_id, target_layer_id));

  std::ignore =
      group_layer_impl::find_layer(registry, root_layer_id, target_layer_id)
          .transform([](const group_layer_impl::FindLayerResult& result) {
            const auto layers_begin = result.parent_layer->layers.begin();

            const auto curr_pos = layers_begin + result.layer_index;
            const auto new_pos = curr_pos - 1;

            std::iter_swap(curr_pos, new_pos);
            return 0;
          });
}

auto can_move_layer_up(const Registry& registry,
                       const EntityID root_layer_id,
                       const EntityID target_layer_id) -> bool
{
  TACTILE_ASSERT(is_group_layer(registry, root_layer_id));
  TACTILE_ASSERT(is_layer(registry, target_layer_id));

  return group_layer_impl::can_move_layer(registry,
                                          root_layer_id,
                                          target_layer_id,
                                          -1);
}

auto can_move_layer_down(const Registry& registry,
                         const EntityID root_layer_id,
                         const EntityID target_layer_id) -> bool
{
  TACTILE_ASSERT(is_group_layer(registry, root_layer_id));
  TACTILE_ASSERT(is_layer(registry, target_layer_id));

  return group_layer_impl::can_move_layer(registry,
                                          root_layer_id,
                                          target_layer_id,
                                          1);
}

}  // namespace tactile
