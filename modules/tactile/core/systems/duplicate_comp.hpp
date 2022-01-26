#pragma once

#include <entt/entt.hpp>  // registry, entity

#include "core/components/attribute_context.hpp"
#include "core/components/layer.hpp"
#include "core/components/object.hpp"
#include "core/components/parent.hpp"
#include "core/components/property.hpp"
#include "core/map.hpp"
#include "property_system.hpp"

namespace tactile::sys {

template <typename T>
auto DuplicateComp(entt::registry& registry,
                   const entt::entity source,
                   const entt::entity destination) -> T&
{
  return registry.emplace<T>(destination, registry.get<T>(source));
}

template <>
inline auto DuplicateComp<attribute_context>(entt::registry& registry,
                                             const entt::entity source,
                                             const entt::entity destination)
    -> attribute_context&
{
  auto& context = AddPropertyContext(registry, destination);

  const auto& srcContext = registry.get<attribute_context>(source);
  context.name = srcContext.name;

  for (const auto srcPropertyEntity : srcContext.properties) {
    const auto propertyEntity = registry.create();
    context.properties.push_back(propertyEntity);

    DuplicateComp<Property>(registry, srcPropertyEntity, propertyEntity);
  }

  return context;
}

template <>
inline auto DuplicateComp<ObjectLayer>(entt::registry& registry,
                                       const entt::entity source,
                                       const entt::entity destination) -> ObjectLayer&
{
  auto& map = registry.ctx<MapInfo>();
  auto& layer = registry.emplace<ObjectLayer>(destination);

  const auto& sourceLayer = registry.get<ObjectLayer>(source);
  for (const auto sourceObject : sourceLayer.objects) {
    const auto objectEntity = registry.create();
    layer.objects.push_back(objectEntity);

    DuplicateComp<attribute_context>(registry, sourceObject, objectEntity);

    auto& object = DuplicateComp<Object>(registry, sourceObject, objectEntity);
    object.id = map.next_object_id;
    ++map.next_object_id;
  }

  return layer;
}

auto DuplicateLayer(entt::registry& registry,
                    entt::entity entity,
                    entt::entity parent,
                    bool recursive) -> entt::entity;

}  // namespace tactile::sys
