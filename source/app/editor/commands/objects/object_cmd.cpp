#include "object_cmd.hpp"

#include <utility>  // move

#include "core/components/attribute_context.hpp"
#include "core/systems/object_system.hpp"
#include "misc/assert.hpp"

namespace tactile {

AObjectCmd::AObjectCmd(std::string name, registry_ref registry, const object_id id)
    : command_base{std::move(name)}
    , mRegistry{registry}
    , mObjectId{id}
{}

auto AObjectCmd::GetTargetObjectContext() const -> comp::attribute_context&
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_object(registry, mObjectId);
  TACTILE_ASSERT(entity != entt::null);

  return registry.get<comp::attribute_context>(entity);
}

auto AObjectCmd::GetTargetObject() -> comp::object&
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_object(registry, mObjectId);
  TACTILE_ASSERT(entity != entt::null);

  return registry.get<comp::object>(entity);
}

}  // namespace tactile
