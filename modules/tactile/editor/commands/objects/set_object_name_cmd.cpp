#include "set_object_name_cmd.hpp"

#include <utility>  // move

#include "core/components/attribute_context.hpp"

namespace tactile {

SetObjectNameCmd::SetObjectNameCmd(RegistryRef registry,
                                   const ObjectID id,
                                   std::string name)
    : AObjectCmd{"Set Object Name", registry, id}
    , mNewName{std::move(name)}
{}

void SetObjectNameCmd::Undo()
{
  auto& context = GetTargetObjectContext();
  context.name = mOldName.value();
}

void SetObjectNameCmd::Redo()
{
  auto& context = GetTargetObjectContext();
  mOldName = context.name;
  context.name = mNewName;
}

auto SetObjectNameCmd::MergeWith(const ACommand& cmd) -> bool
{
  if (GetId() == cmd.GetId()) {
    const auto& other = dynamic_cast<const SetObjectNameCmd&>(cmd);
    if (GetTargetObjectId() == other.GetTargetObjectId()) {
      mNewName = other.mNewName;
      return true;
    }
  }

  return false;
}

}  // namespace tactile
