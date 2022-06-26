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

#include "point_tool_cmd.hpp"

#include <utility>  // move

#include "core/documents/map_document.hpp"
#include "core/layers/object_layer.hpp"
#include "misc/panic.hpp"

namespace tactile {

PointToolCmd::PointToolCmd(MapDocument*    document,
                           const UUID&     layerId,
                           const Vector2f& pos)
    : ACommand{"Add Point"}
    , mDocument{document}
    , mLayerId{layerId}
    , mPos{pos}
{
  if (!mDocument) {
    throw TactileError{"Invalid null map document!"};
  }
}

void PointToolCmd::undo()
{
  auto& map = mDocument->get_map();
  auto& layer = map.view_object_layer(mLayerId);

  layer.remove_object(mObjectId.value());
  mObjectId.reset();
}

void PointToolCmd::redo()
{
  auto& map = mDocument->get_map();
  auto& layer = map.view_object_layer(mLayerId);

  core::Object object;
  object.set_type(ObjectType::Point);
  object.set_pos(mPos);

  mObjectId = object.get_uuid();
  layer.add_object(std::move(object));
}

}  // namespace tactile