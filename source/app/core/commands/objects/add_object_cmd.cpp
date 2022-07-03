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

#include "add_object_cmd.hpp"

#include <utility>  // move

#include "core/documents/map_document.hpp"
#include "core/layers/object_layer.hpp"
#include "misc/panic.hpp"

namespace tactile {

AddObjectCmd::AddObjectCmd(MapDocument*    document,
                           const UUID&     layerId,
                           ObjectType      type,
                           const Vector2f& pos,
                           const Vector2f& size)
    : ACommand{"Add Object"}
    , mDocument{document}
    , mLayerId{layerId}
    , mObjectType{type}
    , mPos{pos}
    , mSize{size}
{
  if (!mDocument) {
    throw TactileError{"Invalid null map document!"};
  }
}

void AddObjectCmd::undo()
{
  auto& map = mDocument->get_map();
  auto& layer = map.view_object_layer(mLayerId);

  const auto objectId = mObjectId.value();
  layer.remove_object(objectId);

  mDocument->unregister_context(objectId);
}

void AddObjectCmd::redo()
{
  auto& map = mDocument->get_map();
  auto& layer = map.view_object_layer(mLayerId);

  auto object = std::make_shared<core::Object>();
  object->set_type(mObjectType);
  object->set_pos(mPos);
  object->set_size(mSize);
  object->set_meta_id(map.fetch_and_increment_next_object_id());

  mObjectId = object->get_uuid();
  layer.add_object(object);

  mDocument->register_context(std::move(object));
}

auto AddObjectCmd::get_name() const -> const char*
{
  switch (mObjectType) {
    case ObjectType::Point:
      return "Add Point Object";

    case ObjectType::Rect:
      return "Add Rectangle Object";

    case ObjectType::Ellipse:
      return "Add Ellipse Object";

    default:
      throw TactileError{"Invalid object type!"};
  }
}

}  // namespace tactile
