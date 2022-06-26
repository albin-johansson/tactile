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

#include "ellipse_tool_cmd.hpp"

#include <utility>  // move

#include "core/documents/map_document.hpp"
#include "core/layers/object.hpp"
#include "core/layers/object_layer.hpp"
#include "misc/panic.hpp"

namespace tactile {

EllipseToolCmd::EllipseToolCmd(MapDocument*    document,
                               const UUID&     layerId,
                               const Vector2f& pos,
                               const Vector2f& size)
    : ACommand{"Add Ellipse"}
    , mDocument{document}
    , mLayerId{layerId}
    , mPos{pos}
    , mSize{size}
{
  if (!mDocument) {
    throw TactileError{"Invalid null map document!"};
  }
}

void EllipseToolCmd::undo()
{
  auto& map = mDocument->get_map();
  auto& layer = map.view_object_layer(mLayerId);

  layer.remove_object(mObjectId.value());
  mObjectId.reset();
}

void EllipseToolCmd::redo()
{
  auto& map = mDocument->get_map();
  auto& layer = map.view_object_layer(mLayerId);

  core::Object object;
  mObjectId = object.get_uuid();

  object.set_type(ObjectType::Ellipse);
  object.set_pos(mPos);
  object.set_size(mSize);

  layer.add_object(std::move(object));
}

}  // namespace tactile