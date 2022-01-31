#include "set_object_name_cmd.hpp"

#include <utility>  // move

#include "core/components/attribute_context.hpp"

namespace tactile {

SetObjectNameCmd::SetObjectNameCmd(registry_ref registry,
                                   const object_id id,
                                   std::string name)
    : AObjectCmd{"Set Object Name", registry, id}
    , mNewName{std::move(name)}
{}

void SetObjectNameCmd::undo()
{
  auto& context = GetTargetObjectContext();
  context.name = mOldName.value();
}

void SetObjectNameCmd::redo()
{
  auto& context = GetTargetObjectContext();
  mOldName = context.name;
  context.name = mNewName;
}

auto SetObjectNameCmd::merge_with(const command_base& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const SetObjectNameCmd&>(cmd);
    if (GetTargetObjectId() == other.GetTargetObjectId()) {
      mNewName = other.mNewName;
      return true;
    }
  }

  return false;
}

}  // namespace tactile
