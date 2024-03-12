// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "set_object_tag.hpp"

#include <utility>  // move

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

SetObjectTag::SetObjectTag(Shared<Object> object, String tag)
    : mObject {std::move(object)},
      mNewTag {std::move(tag)}
{
  if (!mObject) {
    throw Exception {"Invalid null object!"};
  }
}

void SetObjectTag::undo()
{
  mObject->set_tag(mOldTag.value());
  mOldTag.reset();
}

void SetObjectTag::redo()
{
  mOldTag = mObject->get_tag();
  mObject->set_tag(mNewTag);
}

auto SetObjectTag::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const SetObjectTag*>(cmd)) {
    if (mObject->get_uuid() == other->mObject->get_uuid()) {
      mNewTag = other->mNewTag;
      return true;
    }
  }

  return false;
}

auto SetObjectTag::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.update_object_tag;
}

}  // namespace tactile::cmd
