// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/object.hpp"

#include "tactile/core/debug/assert.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/meta/meta.hpp"

namespace tactile {

auto is_object(const Registry& registry, const EntityID entity) -> bool
{
  return registry.has<CMeta>(entity) && registry.has<CObject>(entity);
}

auto make_object(Registry& registry,
                 const ObjectID id,
                 const ObjectType type) -> EntityID
{
  const auto object_entity = registry.make_entity();

  registry.add<CMeta>(object_entity);

  auto& object = registry.add<CObject>(object_entity);
  object.id = id;
  object.position = Float2 {0.0f, 0.0f};
  object.size = Float2 {0.0f, 0.0f};
  object.type = type;
  object.is_visible = true;

  TACTILE_ASSERT(is_object(registry, object_entity));
  return object_entity;
}

void destroy_object(Registry& registry, const EntityID object_entity)
{
  TACTILE_ASSERT(is_object(registry, object_entity));
  registry.destroy(object_entity);
}

}  // namespace tactile
