/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "set_object_tag.hpp"

#include <utility>  // move

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "misc/panic.hpp"

namespace tactile::cmd {

SetObjectTag::SetObjectTag(Shared<Object> object, String tag)
    : mObject {std::move(object)}
    , mNewTag {std::move(tag)}
{
  if (!mObject) {
    throw TactileError {"Invalid null object!"};
  }
}

void SetObjectTag::undo()
{
  mObject->set_tag(mOldTag.value());
  mOldTag.reset();
}

void SetObjectTag::redo()
{
  mOldTag = mObject->tag();
  mObject->set_tag(mNewTag);
}

auto SetObjectTag::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const SetObjectTag*>(cmd)) {
    if (mObject->uuid() == other->mObject->uuid()) {
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
