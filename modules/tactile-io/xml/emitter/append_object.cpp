#include "append_object.hpp"

#include "append_properties.hpp"

namespace Tactile::IO {

void AppendObject(pugi::xml_node source,
                  const ObjectData& object,
                  const std::filesystem::path& dir)
{
  auto node = source.append_child("object");
  node.append_attribute("id").set_value(object.id);

  if (!object.name.empty()) {
    node.append_attribute("name").set_value(object.name.c_str());
  }

  if (!object.tag.empty()) {
    node.append_attribute("type").set_value(object.tag.c_str());
  }

  if (object.x != 0) {
    node.append_attribute("x").set_value(object.x);
  }

  if (object.y != 0) {
    node.append_attribute("y").set_value(object.y);
  }

  if (object.width != 0) {
    node.append_attribute("width").set_value(object.width);
  }

  if (object.height != 0) {
    node.append_attribute("height").set_value(object.height);
  }

  if (!object.visible) {
    node.append_attribute("visible").set_value(0);
  }

  if (object.type == ObjectType::Point) {
    node.append_child("point");
  }
  else if (object.type == ObjectType::Ellipse) {
    node.append_child("ellipse");
  }

  AppendProperties(node, object.properties, dir);
}

}  // namespace Tactile::IO
