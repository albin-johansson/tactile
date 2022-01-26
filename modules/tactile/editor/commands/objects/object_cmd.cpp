#include "object_cmd.hpp"

#include <utility>  // move

#include "assert.hpp"
#include "core/components/attribute_context.hpp"
#include "core/components/object.hpp"
#include "core/systems/object_system.hpp"

namespace tactile {

AObjectCmd::AObjectCmd(std::string name, RegistryRef registry, const ObjectID id)
    : ACommand{std::move(name)}
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

auto AObjectCmd::GetTargetObject() -> Object&
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_object(registry, mObjectId);
  TACTILE_ASSERT(entity != entt::null);

  return registry.get<Object>(entity);
}

}  // namespace tactile
