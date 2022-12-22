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

#include "remove_object.hpp"

#include "core/layer/group_layer.hpp"
#include "core/layer/object.hpp"
#include "core/layer/object_layer.hpp"
#include "debug/assert.hpp"
#include "debug/panic.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/map_document.hpp"

namespace tactile::cmd {

RemoveObject::RemoveObject(MapDocument* document,
                           const UUID& layer_id,
                           const UUID& object_id)
    : mDocument {document},
      mLayerId {layer_id}
{
  if (!mDocument) {
    throw TactileError {"Invalid null document!"};
  }

  TACTILE_ASSERT(
      document->get_map().invisible_root().object_layer(layer_id).has_object(object_id));

  mObject = document->get_object(object_id);
  if (!mObject) {
    throw TactileError {"Invalid object identifier!"};
  }
}

void RemoveObject::undo()
{
  auto& map = mDocument->get_map();
  auto& layer = map.invisible_root().object_layer(mLayerId);

  layer.add_object(mObject);
  mDocument->get_contexts().add_context(mObject);
}

void RemoveObject::redo()
{
  auto& map = mDocument->get_map();
  auto& layer = map.invisible_root().object_layer(mLayerId);

  const auto& id = mObject->uuid();

  if (layer.active_object_id() == id) {
    layer.select_object(nothing);
  }

  layer.remove_object(id);
  mDocument->get_contexts().erase(id);
}

auto RemoveObject::get_name() const -> String
{
  const auto& lang = get_current_language();
  switch (mObject->type()) {
    case ObjectType::Point:
      return lang.cmd.remove_point_object;

    case ObjectType::Rect:
      return lang.cmd.remove_rectangle_object;

    case ObjectType::Ellipse:
      return lang.cmd.remove_ellipse_object;

    default:
      throw TactileError {"Invalid object type!"};
  }
}

}  // namespace tactile::cmd
