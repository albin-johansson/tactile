#include "set_object_tag_cmd.hpp"

#include <utility>  // move

#include "core/components/object.hpp"

namespace tactile {

set_object_tag_cmd::set_object_tag_cmd(registry_ref registry,
                                       const object_id id,
                                       std::string tag)
    : object_cmd{"Set Object Tag", registry, id}
    , mNewTag{std::move(tag)}
{}

void set_object_tag_cmd::undo()
{
  auto& object = target_object();
  object.tag = mOldTag.value();
}

void set_object_tag_cmd::redo()
{
  auto& object = target_object();
  mOldTag = object.tag;
  object.tag = mNewTag;
}

auto set_object_tag_cmd::merge_with(const command_base& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const set_object_tag_cmd&>(cmd);
    if (target_object_id() == other.target_object_id()) {
      mNewTag = other.mNewTag;
      return true;
    }
  }

  return false;
}

}  // namespace tactile
