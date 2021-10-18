#include "object_layer_system.hpp"

#include "tactile/core/components/object.hpp"
#include "tactile/core/components/object_layer.hpp"

namespace Tactile::Sys {

auto HasObject(const entt::registry& registry,
               const entt::entity entity,
               const ObjectID id) -> bool
{
  return FindObject(registry, entity, id) != entt::null;
}

auto FindObject(const entt::registry& registry,
                const entt::entity entity,
                const ObjectID id) -> entt::entity
{
  const auto& layer = registry.get<ObjectLayer>(entity);

  for (const auto objectEntity : layer.objects) {
    const auto& object = registry.get<Object>(objectEntity);
    if (object.id == id) {
      return objectEntity;
    }
  }

  return entt::null;
}

}  // namespace Tactile::Sys
