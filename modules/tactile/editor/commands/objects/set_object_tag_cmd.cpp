#include "set_object_tag_cmd.hpp"

#include <utility>  // move

#include "core/components/object.hpp"

namespace Tactile {

SetObjectTagCmd::SetObjectTagCmd(Ref<entt::registry> registry,
                                 const ObjectID id,
                                 std::string tag)
    : AObjectCmd{"Set Object Tag", registry, id}
    , mNewTag{std::move(tag)}
{}

void SetObjectTagCmd::Undo()
{
  auto& object = GetTargetObject();
  object.tag = mOldTag.value();
}

void SetObjectTagCmd::Redo()
{
  auto& object = GetTargetObject();
  mOldTag = object.tag;
  object.tag = mNewTag;
}

auto SetObjectTagCmd::MergeWith(const ACommand& cmd) -> bool
{
  if (GetId() == cmd.GetId()) {
    const auto& other = dynamic_cast<const SetObjectTagCmd&>(cmd);
    if (GetTargetObjectId() == other.GetTargetObjectId()) {
      mNewTag = other.mNewTag;
      return true;
    }
  }

  return false;
}

}  // namespace Tactile
