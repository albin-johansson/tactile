// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/object.hpp"

#include "tactile/base/io/save/ir.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/meta/meta.hpp"

namespace tactile::core {

auto is_object(const Registry& registry, const EntityID entity) -> bool
{
  return registry.has<CMeta>(entity) && registry.has<CObject>(entity);
}

auto make_object(Registry& registry, const ObjectID id, const ObjectType type) -> EntityID
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

auto make_object(Registry& registry, const ir::Object& ir_object) -> EntityID
{
  const auto object_id = make_object(registry, ir_object.id, ir_object.type);

  auto& object = registry.get<CObject>(object_id);
  object.position = ir_object.position;
  object.size = ir_object.size;
  object.tag = ir_object.tag;
  object.is_visible = ir_object.visible;

  convert_ir_metadata(registry, object_id, ir_object.meta);

  TACTILE_ASSERT(is_object(registry, object_id));
  return object_id;
}

void destroy_object(Registry& registry, const EntityID object_entity)
{
  TACTILE_ASSERT(is_object(registry, object_entity));
  registry.destroy(object_entity);
}

auto copy_object(Registry& registry, const EntityID object_entity) -> EntityID
{
  TACTILE_ASSERT(is_object(registry, object_entity));

  const auto copy_entity = registry.make_entity();
  registry.add<CMeta>(copy_entity, registry.get<CMeta>(object_entity));
  registry.add<CObject>(copy_entity, registry.get<CObject>(object_entity));

  TACTILE_ASSERT(is_object(registry, copy_entity));
  return copy_entity;
}

}  // namespace tactile::core
