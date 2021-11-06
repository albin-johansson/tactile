#include "object_cmd.hpp"

#include <cassert>  // assert
#include <utility>  // move

#include "core/components/object.hpp"
#include "core/components/property_context.hpp"
#include "core/systems/object_system.hpp"

namespace Tactile {

AObjectCmd::AObjectCmd(std::string name, Ref<entt::registry> registry, const ObjectID id)
    : ACommand{std::move(name)}
    , mRegistry{registry}
    , mObjectId{id}
{}

auto AObjectCmd::GetTargetObjectContext() const -> PropertyContext&
{
  auto& registry = mRegistry.get();

  const auto entity = Sys::FindObject(registry, mObjectId);
  assert(entity != entt::null);

  return registry.get<PropertyContext>(entity);
}

auto AObjectCmd::GetTargetObject() -> Object&
{
  auto& registry = mRegistry.get();

  const auto entity = Sys::FindObject(registry, mObjectId);
  assert(entity != entt::null);

  return registry.get<Object>(entity);
}

}  // namespace Tactile
