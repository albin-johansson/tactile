// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/object_layer.hpp"

#include "tactile/core/debug/assert.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/layer/layer.hpp"
#include "tactile/core/layer/layer_types.hpp"
#include "tactile/core/layer/object.hpp"
#include "tactile/core/meta/meta.hpp"

namespace tactile::core {

auto is_object_layer(const Registry& registry, const EntityID entity) -> bool
{
  return registry.has<CMeta>(entity) &&   //
         registry.has<CLayer>(entity) &&  //
         registry.has<CObjectLayer>(entity);
}

auto make_object_layer(Registry& registry) -> EntityID
{
  const auto layer_entity = make_unspecialized_layer(registry);

  registry.add<CObjectLayer>(layer_entity);

  TACTILE_ASSERT(is_object_layer(registry, layer_entity));
  return layer_entity;
}

void destroy_object_layer(Registry& registry, const EntityID object_layer_entity)
{
  TACTILE_ASSERT(is_object_layer(registry, object_layer_entity));

  const auto& object_layer = registry.get<CObjectLayer>(object_layer_entity);

  for (const auto object_entity : object_layer.objects) {
    destroy_object(registry, object_entity);
  }

  registry.destroy(object_layer_entity);
}

}  // namespace tactile::core
