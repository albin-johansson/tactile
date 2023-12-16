// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled-xml-format/emit/object_emitter.hpp"

#include "tactile/tiled-xml-format/emit/property_emitter.hpp"

namespace tactile::tiled::tmx {

void append_object_node(pugi::xml_node parent_node, const ir::Object& object)
{
  auto object_node = parent_node.append_child("object");
  object_node.append_attribute("id").set_value(object.id.value);

  if (!object.name.empty()) {
    object_node.append_attribute("name").set_value(object.name.c_str());
  }

  if (!object.tag.empty()) {
    object_node.append_attribute("type").set_value(object.tag.c_str());
  }

  if (object.x != 0.0f) {
    object_node.append_attribute("x").set_value(object.x);
  }

  if (object.y != 0.0f) {
    object_node.append_attribute("y").set_value(object.y);
  }

  if (object.width != 0.0f) {
    object_node.append_attribute("width").set_value(object.width);
  }

  if (object.height != 0.0f) {
    object_node.append_attribute("height").set_value(object.height);
  }

  if (!object.visible) {
    object_node.append_attribute("visible").set_value(0);
  }

  // Objects are assumed to be rectangles by default.
  if (object.type == ObjectType::kPoint) {
    object_node.append_child("point");
  }
  else if (object.type == ObjectType::kEllipse) {
    object_node.append_child("ellipse");
  }

  append_properties_node(object_node, object.meta.properties);
}

}  // namespace tactile::tiled::tmx
