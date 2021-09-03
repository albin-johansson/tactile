#include "object_layer_system.hpp"

#include "core/components/object.hpp"
#include "core/components/object_layer.hpp"

namespace Tactile::Sys {

auto HasObject(const entt::registry& registry,
               const entt::entity entity,
               const object_id id) -> bool
{
  return FindObject(registry, entity, id) != entt::null;
}

auto FindObject(const entt::registry& registry,
                const entt::entity entity,
                const object_id id) -> entt::entity
{
  const auto& layer = registry.get<ObjectLayer>(entity);

  for (const auto objectEntity : layer.objects)
  {
    const auto& object = registry.get<Object>(objectEntity);
    if (object.id == id)
    {
      return objectEntity;
    }
  }

  return entt::null;
}

}  // namespace Tactile::Sys
