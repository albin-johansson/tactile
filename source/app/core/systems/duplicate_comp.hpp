#pragma once

#include <entt/entt.hpp>  // registry, entity

#include "context_system.hpp"
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
inline auto DuplicateComp<comp::attribute_context>(entt::registry& registry,
                                                   const entt::entity source,
                                                   const entt::entity destination)
    -> comp::attribute_context&
{
  auto& context = add_attribute_context(registry, destination);

  const auto& srcContext = registry.get<comp::attribute_context>(source);
  context.name = srcContext.name;

  for (const auto srcPropertyEntity : srcContext.properties) {
    const auto propertyEntity = registry.create();
    context.properties.push_back(propertyEntity);

    DuplicateComp<comp::property>(registry, srcPropertyEntity, propertyEntity);
  }

  return context;
}

template <>
inline auto DuplicateComp<comp::object_layer>(entt::registry& registry,
                                              const entt::entity source,
                                              const entt::entity destination)
    -> comp::object_layer&
{
  auto& map = registry.ctx<MapInfo>();
  auto& layer = registry.emplace<comp::object_layer>(destination);

  const auto& sourceLayer = registry.get<comp::object_layer>(source);
  for (const auto sourceObject : sourceLayer.objects) {
    const auto objectEntity = registry.create();
    layer.objects.push_back(objectEntity);

    DuplicateComp<comp::attribute_context>(registry, sourceObject, objectEntity);

    auto& object = DuplicateComp<comp::object>(registry, sourceObject, objectEntity);
    object.id = map.next_object_id;
    ++map.next_object_id;
  }

  return layer;
}

auto duplicate_layer(entt::registry& registry,
                     entt::entity entity,
                     entt::entity parent,
                     bool recursive) -> entt::entity;

}  // namespace tactile::sys
