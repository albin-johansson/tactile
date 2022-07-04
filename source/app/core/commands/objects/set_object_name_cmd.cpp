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

#include "set_object_name_cmd.hpp"

#include <utility>  // move

#include "core/documents/map_document.hpp"
#include "core/layers/object_layer.hpp"
#include "misc/panic.hpp"

namespace tactile {

SetObjectNameCmd::SetObjectNameCmd(MapDocument* document,
                                   const UUID&  objectId,
                                   std::string  name)
    : mDocument{document}
    , mObjectId{objectId}
    , mNewName{std::move(name)}
{
  if (!mDocument) {
    throw TactileError{"Invalid null map document!"};
  }
}

void SetObjectNameCmd::undo()
{
  auto object = mDocument->get_object(mObjectId);
  object->set_name(mOldName.value());
  mOldName.reset();
}

void SetObjectNameCmd::redo()
{
  auto object = mDocument->get_object(mObjectId);
  mOldName = object->get_name();
  object->set_name(mNewName);
}

auto SetObjectNameCmd::merge_with(const ICommand* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const SetObjectNameCmd*>(cmd)) {
    if (mObjectId == other->mObjectId) {
      mNewName = other->mNewName;
      return true;
    }
  }

  return false;
}

auto SetObjectNameCmd::get_name() const -> const char*
{
  return "Set Object Name";
}

}  // namespace tactile
