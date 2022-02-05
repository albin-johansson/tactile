#include "object_cmd.hpp"

#include <utility>  // move

#include "core/components/attribute_context.hpp"
#include "core/systems/object_system.hpp"
#include "misc/assert.hpp"

namespace tactile {

object_cmd::object_cmd(std::string name, registry_ref registry, const object_id id)
    : command_base{std::move(name)}
    , mRegistry{registry}
    , mObjectId{id}
{}

auto object_cmd::target_object_context() const -> comp::attribute_context&
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_object(registry, mObjectId);
  TACTILE_ASSERT(entity != entt::null);

  return registry.get<comp::attribute_context>(entity);
}

auto object_cmd::target_object() -> comp::object&
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_object(registry, mObjectId);
  TACTILE_ASSERT(entity != entt::null);

  return registry.get<comp::object>(entity);
}

}  // namespace tactile
