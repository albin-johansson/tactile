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

#include "core/common/ecs.hpp"
#include "core/components/objects.hpp"
#include "core/documents/map_document.hpp"
#include "core/systems/context_system.hpp"
#include "misc/panic.hpp"

namespace tactile {

MoveObjectCmd::MoveObjectCmd(MapDocument* map,
                             const UUID& objectId,
                             const Vector2f& previous,
                             const Vector2f& updated)
    : ACommand{"Move Object"}
    , mMap{map}
    , mObjectId{objectId}
    , mPreviousPos{previous}
    , mUpdatedPos{updated}
{
  if (!mMap) {
    throw TactileError{"Invalid null map!"};
  }
}

void MoveObjectCmd::undo()
{
  auto& registry = mMap->get_registry();

  const auto objectEntity = sys::find_context(registry, mObjectId);
  auto& object = checked_get<comp::Object>(registry, objectEntity);
  object.pos = mPreviousPos;
}

void MoveObjectCmd::redo()
{
  auto& registry = mMap->get_registry();

  const auto objectEntity = sys::find_context(registry, mObjectId);
  auto& object = checked_get<comp::Object>(registry, objectEntity);
  object.pos = mUpdatedPos;
}

}  // namespace tactile
