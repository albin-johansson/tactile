// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "rename_object.hpp"

#include <utility>  // move

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

RenameObject::RenameObject(Shared<Object> object, String name)
    : mObject {std::move(object)},
      mNewName {std::move(name)}
{
  if (!mObject) {
    throw Exception {"Invalid null object!"};
  }
}

void RenameObject::undo()
{
  mObject->get_ctx().set_name(mOldName.value());
  mOldName.reset();
}

void RenameObject::redo()
{
  mOldName = mObject->get_ctx().name();
  mObject->get_ctx().set_name(mNewName);
}

auto RenameObject::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const RenameObject*>(cmd)) {
    if (mObject->get_uuid() == other->mObject->get_uuid()) {
      mNewName = other->mNewName;
      return true;
    }
  }

  return false;
}

auto RenameObject::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.rename_object;
}

}  // namespace tactile::cmd
