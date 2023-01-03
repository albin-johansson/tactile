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

#include "add_object.hpp"

#include <utility>  // move

#include "common/debug/panic.hpp"
#include "core/layer/group_layer.hpp"
#include "core/layer/object.hpp"
#include "core/layer/object_layer.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/map_document.hpp"

namespace tactile::cmd {

AddObject::AddObject(MapDocument* document,
                     const UUID& layer_id,
                     ObjectType type,
                     const Float2& pos,
                     const Float2& size)
    : mDocument {document},
      mLayerId {layer_id},
      mObjectType {type},
      mPos {pos},
      mSize {size}
{
  if (!mDocument) {
    throw TactileError {"Invalid null map document!"};
  }
}

void AddObject::undo()
{
  auto& map = mDocument->get_map();
  auto& layer = map.invisible_root().get_object_layer(mLayerId);

  const auto object_id = mObjectId.value();
  layer.remove_object(object_id);
  mDocument->get_contexts().erase(object_id);
}

void AddObject::redo()
{
  auto& map = mDocument->get_map();
  auto& layer = map.invisible_root().get_object_layer(mLayerId);

  if (!mObject) {
    mObject = std::make_shared<Object>();
    mObjectId = mObject->get_uuid();
  }

  mObject->set_type(mObjectType);
  mObject->set_pos(mPos);
  mObject->set_size(mSize);
  mObject->set_meta_id(map.fetch_and_increment_next_object_id());

  layer.add_object(mObject);
  mDocument->get_contexts().add_context(mObject);
}

auto AddObject::get_name() const -> String
{
  const auto& lang = get_current_language();
  switch (mObjectType) {
    case ObjectType::Point:
      return lang.cmd.add_point_object;

    case ObjectType::Rect:
      return lang.cmd.add_rectangle_object;

    case ObjectType::Ellipse:
      return lang.cmd.add_ellipse_object;

    default:
      throw TactileError {"Invalid object type!"};
  }
}

}  // namespace tactile::cmd
