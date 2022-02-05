#include "object_system.hpp"

#include "core/components/object.hpp"
#include "misc/throw.hpp"

namespace tactile::sys {

auto find_object(const entt::registry& registry, const object_id id) -> entt::entity
{
  for (auto&& [entity, object] : registry.view<comp::object>().each()) {
    if (object.id == id) {
      return entity;
    }
  }

  return entt::null;
}

auto get_object(const entt::registry& registry, object_id id) -> entt::entity
{
  const auto entity = find_object(registry, id);
  if (entity != entt::null) {
    return entity;
  }
  else {
    throw_traced(tactile_error{"Invalid object identifier!"});
  }
}

}  // namespace tactile::sys
