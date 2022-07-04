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

#include "move_object_cmd.hpp"

#include "core/documents/map_document.hpp"
#include "core/layers/object_layer.hpp"
#include "misc/panic.hpp"

namespace tactile {

MoveObjectCmd::MoveObjectCmd(MapDocument*    document,
                             const UUID&     objectId,
                             const Vector2f& previous,
                             const Vector2f& updated)
    : mDocument{document}
    , mObjectId{objectId}
    , mPreviousPos{previous}
    , mUpdatedPos{updated}
{
  if (!mDocument) {
    throw TactileError{"Invalid null map document!"};
  }
}

void MoveObjectCmd::undo()
{
  auto object = mDocument->get_object(mObjectId);
  object->set_pos(mPreviousPos);
}

void MoveObjectCmd::redo()
{
  auto object = mDocument->get_object(mObjectId);
  object->set_pos(mUpdatedPos);
}

auto MoveObjectCmd::get_name() const -> const char*
{
  return "Move Object";
}

}  // namespace tactile
