// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "remove_object.hpp"

#include "core/layer/group_layer.hpp"
#include "core/layer/object.hpp"
#include "core/layer/object_layer.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/map_document.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

RemoveObject::RemoveObject(MapDocument* document,
                           const UUID& layer_id,
                           const UUID& object_id)
    : mDocument {document},
      mLayerId {layer_id}
{
  if (!mDocument) {
    throw Exception {"Invalid null document!"};
  }

  TACTILE_ASSERT(
      document->get_map().get_invisible_root().get_object_layer(layer_id).has_object(
          object_id));

  mObject = document->get_object(object_id);
  if (!mObject) {
    throw Exception {"Invalid object identifier!"};
  }
}

void RemoveObject::undo()
{
  auto& map = mDocument->get_map();
  auto& layer = map.get_invisible_root().get_object_layer(mLayerId);

  layer.add_object(mObject);
  mDocument->get_contexts().add_context(mObject);
}

void RemoveObject::redo()
{
  auto& map = mDocument->get_map();
  auto& layer = map.get_invisible_root().get_object_layer(mLayerId);

  const auto& id = mObject->get_uuid();

  if (layer.active_object_id() == id) {
    layer.select_object(nothing);
  }

  layer.remove_object(id);
  mDocument->get_contexts().remove_context(id);
}

auto RemoveObject::get_name() const -> String
{
  const auto& lang = get_current_language();
  switch (mObject->get_type()) {
    case ObjectType::Point:
      return lang.cmd.remove_point_object;

    case ObjectType::Rect:
      return lang.cmd.remove_rectangle_object;

    case ObjectType::Ellipse:
      return lang.cmd.remove_ellipse_object;

    default:
      throw Exception {"Invalid object type!"};
  }
}

}  // namespace tactile::cmd
