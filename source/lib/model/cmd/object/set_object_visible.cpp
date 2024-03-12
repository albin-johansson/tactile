// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "set_object_visible.hpp"

#include <utility>  // move

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

SetObjectVisible::SetObjectVisible(Shared<Object> object, const bool visible)
    : mObject {std::move(object)},
      mNewVisibility {visible}
{
  if (!mObject) {
    throw Exception {"Invalid null object!"};
  }
}

void SetObjectVisible::undo()
{
  mObject->set_visible(mOldVisibility.value());
  mOldVisibility.reset();
}

void SetObjectVisible::redo()
{
  mOldVisibility = mObject->is_visible();
  mObject->set_visible(mNewVisibility);
}

auto SetObjectVisible::get_name() const -> String
{
  const auto& lang = get_current_language();
  return mNewVisibility ? lang.cmd.show_object : lang.cmd.hide_object;
}

}  // namespace tactile::cmd
