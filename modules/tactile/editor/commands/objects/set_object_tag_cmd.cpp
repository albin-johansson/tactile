#include "set_object_tag_cmd.hpp"

#include <cassert>  // assert
#include <utility>  // move

#include "core/components/object.hpp"
#include "core/systems/object_system.hpp"

namespace Tactile {

SetObjectTagCmd::SetObjectTagCmd(Ref<entt::registry> registry,
                                 const ObjectID id,
                                 std::string tag)
    : ACommand{"Set Object Tag"}
    , mRegistry{registry}
    , mObjectId{id}
    , mNewTag{std::move(tag)}
{}

void SetObjectTagCmd::Undo()
{
  auto& object = GetObject();
  object.custom_type = mOldTag.value();
}

void SetObjectTagCmd::Redo()
{
  auto& object = GetObject();
  mOldTag = object.custom_type;
  object.custom_type = mNewTag;
}

auto SetObjectTagCmd::MergeWith(const ACommand& cmd) -> bool
{
  if (GetId() == cmd.GetId()) {
    const auto& other = dynamic_cast<const SetObjectTagCmd&>(cmd);
    if (mObjectId == other.mObjectId) {
      mNewTag = other.mNewTag;
      return true;
    }
  }

  return false;
}

auto SetObjectTagCmd::GetObject() -> Object&
{
  auto& registry = mRegistry.get();

  const auto entity = Sys::FindObject(registry, mObjectId);
  assert(entity != entt::null);

  return registry.get<Object>(entity);
}

}  // namespace Tactile
