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

#include "set_object_visible_cmd.hpp"

#include "core/documents/map_document.hpp"
#include "core/layers/object_layer.hpp"
#include "misc/panic.hpp"

namespace tactile {

SetObjectVisibleCmd::SetObjectVisibleCmd(MapDocument* document,
                                         const UUID&  objectId,
                                         const bool   visible)
    : mDocument{document}
    , mObjectId{objectId}
    , mVisible{visible}
{
  if (!mDocument) {
    throw TactileError{"Invalid null map document!"};
  }
}

void SetObjectVisibleCmd::undo()
{
  auto object = mDocument->get_object(mObjectId);
  object->set_visible(mPrevious.value());
  mPrevious.value();
}

void SetObjectVisibleCmd::redo()
{
  auto object = mDocument->get_object(mObjectId);
  mPrevious = object->is_visible();
  object->set_visible(mVisible);
}

auto SetObjectVisibleCmd::get_name() const -> const char*
{
  return mVisible ? "Show Object" : "Hide Object";
}

}  // namespace tactile
