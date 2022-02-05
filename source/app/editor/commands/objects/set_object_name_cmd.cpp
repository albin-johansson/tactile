#include "set_object_name_cmd.hpp"

#include <utility>  // move

#include "core/components/attribute_context.hpp"

namespace tactile {

set_object_name_cmd::set_object_name_cmd(registry_ref registry,
                                         const object_id id,
                                         std::string name)
    : object_cmd{"Set Object Name", registry, id}
    , mNewName{std::move(name)}
{}

void set_object_name_cmd::undo()
{
  auto& context = target_object_context();
  context.name = mOldName.value();
}

void set_object_name_cmd::redo()
{
  auto& context = target_object_context();
  mOldName = context.name;
  context.name = mNewName;
}

auto set_object_name_cmd::merge_with(const command_base& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const set_object_name_cmd&>(cmd);
    if (target_object_id() == other.target_object_id()) {
      mNewName = other.mNewName;
      return true;
    }
  }

  return false;
}

}  // namespace tactile
