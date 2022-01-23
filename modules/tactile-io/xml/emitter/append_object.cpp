#include "append_object.hpp"

#include <cstring>  // strcmp

#include "append_properties.hpp"

namespace tactile::IO {

void AppendObject(pugi::xml_node source,
                  const Object& object,
                  const std::filesystem::path& dir)
{
  auto node = source.append_child("object");
  node.append_attribute("id").set_value(GetId(object));

  if (const auto name = GetName(object); std::strcmp(name, "") != 0) {
    node.append_attribute("name").set_value(name);
  }

  if (const auto tag = GetTag(object); std::strcmp(tag, "") != 0) {
    node.append_attribute("type").set_value(tag);
  }

  if (const auto x = GetX(object); x != 0.0f) {
    node.append_attribute("x").set_value(x);
  }

  if (const auto y = GetY(object); y != 0.0f) {
    node.append_attribute("y").set_value(y);
  }

  if (const auto width = GetWidth(object); width != 0.0f) {
    node.append_attribute("width").set_value(width);
  }

  if (const auto height = GetHeight(object); height != 0.0f) {
    node.append_attribute("height").set_value(height);
  }

  if (const auto visible = IsVisible(object); !visible) {
    node.append_attribute("visible").set_value(0);
  }

  if (const auto type = GetType(object); type == ObjectType::Point) {
    node.append_child("point");
  }
  else if (type == ObjectType::Ellipse) {
    node.append_child("ellipse");
  }

  AppendProperties(node, object, dir);
}

}  // namespace tactile::IO
