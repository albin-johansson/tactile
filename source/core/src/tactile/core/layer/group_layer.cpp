// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/group_layer.hpp"

#include "tactile/core/debug/assert.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/layer/layer.hpp"
#include "tactile/core/layer/object_layer.hpp"
#include "tactile/core/layer/tile_layer.hpp"
#include "tactile/core/meta/meta.hpp"

namespace tactile {

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
  for (const auto layer_entity : group_layer.layers) {
    if (registry.has<CGroupLayer>(layer_entity)) {
      destroy_group_layer(registry, layer_entity);
    }
    else if (registry.has<CTileLayer>(layer_entity)) {
      destroy_tile_layer(registry, layer_entity);
    }
    else if (registry.has<CObjectLayer>(layer_entity)) {
      destroy_object_layer(registry, layer_entity);
    }
    else {
      TACTILE_ASSERT_MSG(false, "invalid layer entity");
    }
  }

  registry.destroy(group_layer_entity);
}

}  // namespace tactile
