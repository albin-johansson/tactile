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
#include "tactile/core/layer/layer_types.hpp"
#include "tactile/core/layer/object_layer.hpp"
#include "tactile/core/layer/tile_layer.hpp"
#include "tactile/core/meta/meta.hpp"

namespace tactile::core {
namespace {

struct FindLayerResult final
{
  CGroupLayer* parent_layer;
  std::ptrdiff_t layer_index;
};

struct FindLayerConstResult final
{
  const CGroupLayer* parent_layer;
  std::ptrdiff_t layer_index;
};

[[nodiscard]]
auto _find_layer(Registry& registry,
                 const EntityID root_layer_entity,
                 const EntityID target_layer_entity) -> std::optional<FindLayerResult>
{
  const auto parent_entity =
      find_parent_layer(registry, root_layer_entity, target_layer_entity);

  if (!parent_entity.has_value()) {
    return std::nullopt;
  }

  auto& parent_layer = registry.get<CGroupLayer>(*parent_entity);

  // We already found the layer, so no check needed here.
  const auto iter = std::ranges::find(parent_layer.layers, target_layer_entity);
  TACTILE_ASSERT(iter != parent_layer.layers.end());

  const auto layer_index = std::distance(parent_layer.layers.begin(), iter);

  return FindLayerResult {
    .parent_layer = &parent_layer,
    .layer_index = layer_index,
  };
}

[[nodiscard]]
auto _find_layer(const Registry& registry,
                 const EntityID root_layer_entity,
                 const EntityID target_layer_entity) -> std::optional<FindLayerConstResult>
{
  const auto parent_entity =
      find_parent_layer(registry, root_layer_entity, target_layer_entity);

  if (!parent_entity.has_value()) {
    return std::nullopt;
  }

  const auto& parent_layer = registry.get<CGroupLayer>(*parent_entity);

  // We already found the layer, so no check needed here.
  const auto iter = std::ranges::find(parent_layer.layers, target_layer_entity);
  TACTILE_ASSERT(iter != parent_layer.layers.end());

  const auto layer_index = std::distance(parent_layer.layers.begin(), iter);

  return FindLayerConstResult {
    .parent_layer = &parent_layer,
    .layer_index = layer_index,
  };
}

[[nodiscard]]
auto _can_move_layer(const Registry& registry,
                     const EntityID root_layer_id,
                     const EntityID target_layer_id,
                     const std::ptrdiff_t offset) -> bool
{
  const auto find_result = _find_layer(registry, root_layer_id, target_layer_id);

  if (!find_result.has_value()) {
    return false;
  }

  const auto new_index = find_result->layer_index + offset;
  const auto layer_count = find_result->parent_layer->layers.size();

  return new_index >= 0 && std::cmp_less(new_index, layer_count);
}

[[nodiscard]]
auto _get_global_layer_index(const Registry& registry,
                             const EntityID root_layer_id,
                             const EntityID target_layer_id,
                             std::size_t& index) -> bool
{
  const auto& root_layer = registry.get<CGroupLayer>(root_layer_id);

  for (const auto layer_id : root_layer.layers) {
    if (layer_id == target_layer_id) {
      return true;
    }

    ++index;

    if (is_group_layer(registry, layer_id)) {
      if (_get_global_layer_index(registry, layer_id, target_layer_id, index)) {
        return true;
      }
    }
  }

  return false;
}

}  // namespace

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

void destroy_group_layer(Registry& registry, const EntityID group_layer_entity)
{
  TACTILE_ASSERT(is_group_layer(registry, group_layer_entity));

  const auto& group_layer = registry.get<CGroupLayer>(group_layer_entity);
  for (const auto sublayer_entity : group_layer.layers) {
    if (registry.has<CGroupLayer>(sublayer_entity)) {
      destroy_group_layer(registry, sublayer_entity);
    }
    else if (registry.has<CTileLayer>(sublayer_entity)) {
      destroy_tile_layer(registry, sublayer_entity);
    }
    else if (registry.has<CObjectLayer>(sublayer_entity)) {
      destroy_object_layer(registry, sublayer_entity);
    }
    else {
      TACTILE_ASSERT_MSG(false, "invalid layer entity");
    }
  }

  registry.destroy(group_layer_entity);
}

auto count_layers(const Registry& registry, const EntityID root_layer_entity) -> std::size_t
{
  TACTILE_ASSERT(is_group_layer(registry, root_layer_entity));

  const auto& root_layer = registry.get<CGroupLayer>(root_layer_entity);
  std::size_t count = 0;

  for (const auto sublayer_entity : root_layer.layers) {
    ++count;
    if (is_group_layer(registry, sublayer_entity)) {
      count += count_layers(registry, sublayer_entity);
    }
  }

  return count;
}

auto find_parent_layer(const Registry& registry,
                       const EntityID root_layer_entity,
                       const EntityID target_layer_entity) -> std::optional<EntityID>
{
  TACTILE_ASSERT(is_group_layer(registry, root_layer_entity));
  TACTILE_ASSERT(is_layer(registry, target_layer_entity));

  const auto& root_layer = registry.get<CGroupLayer>(root_layer_entity);

  for (const auto sublayer_entity : root_layer.layers) {
    if (sublayer_entity == target_layer_entity) {
      return root_layer_entity;
    }

    if (!is_group_layer(registry, sublayer_entity)) {
      continue;
    }

    const auto found_entity =
        find_parent_layer(registry, sublayer_entity, target_layer_entity);
    if (found_entity.has_value()) {
      return found_entity;
    }
  }

  return std::nullopt;
}

auto get_local_layer_index(const Registry& registry,
                           const EntityID root_layer_entity,
                           const EntityID target_layer_entity) -> std::optional<std::size_t>
{
  TACTILE_ASSERT(is_group_layer(registry, root_layer_entity));
  TACTILE_ASSERT(is_layer(registry, target_layer_entity));

  return _find_layer(registry, root_layer_entity, target_layer_entity)
      .transform([](const FindLayerConstResult& result) {
        return saturate_cast<std::size_t>(result.layer_index);
      });
}

auto get_global_layer_index(const Registry& registry,
                            const EntityID root_layer_entity,
                            const EntityID target_layer_entity) -> std::optional<std::size_t>
{
  TACTILE_ASSERT(is_group_layer(registry, root_layer_entity));
  TACTILE_ASSERT(is_layer(registry, target_layer_entity));

  std::size_t index = 0;

  if (_get_global_layer_index(registry, root_layer_entity, target_layer_entity, index)) {
    return index;
  }

  return std::nullopt;
}

void move_layer_up(Registry& registry,
                   const EntityID root_layer_entity,
                   const EntityID target_layer_entity)
{
  TACTILE_ASSERT(is_group_layer(registry, root_layer_entity));
  TACTILE_ASSERT(is_layer(registry, target_layer_entity));
  TACTILE_ASSERT(can_move_layer_up(registry, root_layer_entity, target_layer_entity));

  std::ignore = _find_layer(registry, root_layer_entity, target_layer_entity)
                    .transform([](const FindLayerResult& result) {
                      const auto layers_begin = result.parent_layer->layers.begin();

                      const auto curr_pos = layers_begin + result.layer_index;
                      const auto new_pos = curr_pos - 1;

                      std::iter_swap(curr_pos, new_pos);
                      return 0;
                    });
}

void move_layer_down(Registry& registry,
                     const EntityID root_layer_entity,
                     const EntityID target_layer_entity)
{
  TACTILE_ASSERT(is_group_layer(registry, root_layer_entity));
  TACTILE_ASSERT(is_layer(registry, target_layer_entity));
  TACTILE_ASSERT(can_move_layer_down(registry, root_layer_entity, target_layer_entity));

  std::ignore = _find_layer(registry, root_layer_entity, target_layer_entity)
                    .transform([](const FindLayerResult& result) {
                      const auto layers_begin = result.parent_layer->layers.begin();

                      const auto curr_pos = layers_begin + result.layer_index;
                      const auto new_pos = curr_pos + 1;

                      std::iter_swap(curr_pos, new_pos);
                      return 0;
                    });
}

auto can_move_layer_up(const Registry& registry,
                       const EntityID root_layer_entity,
                       const EntityID target_layer_entity) -> bool
{
  TACTILE_ASSERT(is_group_layer(registry, root_layer_entity));
  TACTILE_ASSERT(is_layer(registry, target_layer_entity));

  return _can_move_layer(registry, root_layer_entity, target_layer_entity, -1);
}

auto can_move_layer_down(const Registry& registry,
                         const EntityID root_layer_entity,
                         const EntityID target_layer_entity) -> bool
{
  TACTILE_ASSERT(is_group_layer(registry, root_layer_entity));
  TACTILE_ASSERT(is_layer(registry, target_layer_entity));

  return _can_move_layer(registry, root_layer_entity, target_layer_entity, 1);
}

}  // namespace tactile::core
