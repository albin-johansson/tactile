#include "set_object_tag_cmd.hpp"

#include <utility>  // move

#include "core/components/object.hpp"

namespace tactile {

SetObjectTagCmd::SetObjectTagCmd(registry_ref registry,
                                 const object_id id,
                                 std::string tag)
    : AObjectCmd{"Set Object Tag", registry, id}
    , mNewTag{std::move(tag)}
{}

void SetObjectTagCmd::undo()
{
  auto& object = GetTargetObject();
  object.tag = mOldTag.value();
}

void SetObjectTagCmd::redo()
{
  auto& object = GetTargetObject();
  mOldTag = object.tag;
  object.tag = mNewTag;
}

auto SetObjectTagCmd::merge_with(const command_base& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const SetObjectTagCmd&>(cmd);
    if (GetTargetObjectId() == other.GetTargetObjectId()) {
      mNewTag = other.mNewTag;
      return true;
    }
  }

  return false;
}

}  // namespace tactile
