#include "set_object_visibility_cmd.hpp"

#include <cassert>  // assert

#include "core/components/object.hpp"
#include "core/systems/object_system.hpp"

namespace Tactile {

SetObjectVisibilityCmd::SetObjectVisibilityCmd(Ref<entt::registry> registry,
                                               const ObjectID id,
                                               const bool visible)
    : ACommand{"Set Object Visibility"}
    , mRegistry{registry}
    , mObjectId{id}
    , mVisible{visible}
{}

void SetObjectVisibilityCmd::Undo()
{
  auto& object = GetObject();
  object.visible = mPreviousVisibility.value();
}

void SetObjectVisibilityCmd::Redo()
{
  auto& object = GetObject();
  mPreviousVisibility = object.visible;
  object.visible = mVisible;
}

auto SetObjectVisibilityCmd::GetObject() -> Object&
{
  auto& registry = mRegistry.get();

  const auto entity = Sys::FindObject(registry, mObjectId);
  assert(entity != entt::null);

  return registry.get<Object>(entity);
}

}  // namespace Tactile
