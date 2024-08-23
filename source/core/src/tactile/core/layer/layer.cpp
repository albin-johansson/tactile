// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/layer.hpp"

#include "tactile/core/debug/assert.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/meta/meta.hpp"

namespace tactile {

auto is_layer(const Registry& registry, const EntityID entity) -> bool
{
  return registry.has<CMeta>(entity) && registry.has<CLayer>(entity);
}

auto make_unspecialized_layer(Registry& registry) -> EntityID
{
  const auto layer_entity = registry.make_entity();

  auto& layer = registry.add<CLayer>(layer_entity);
  layer.persistent_id = std::nullopt;
  layer.opacity = 1.0f;
  layer.visible = true;

  registry.add<CMeta>(layer_entity);

  TACTILE_ASSERT(is_layer(registry, layer_entity));
  return layer_entity;
}

}  // namespace tactile
