#include "parse_object.hpp"

#include <string>  // string

#include "parse_properties.hpp"
#include "xml_utils.hpp"

namespace Tactile::IO {

auto ParseObject(const pugi::xml_node node, ObjectData& object) -> ParseError
{
  if (const auto id = GetInt(node, "id")) {
    object.id = ObjectID{*id};
  }
  else {
    return ParseError::ObjectMissingId;
  }

  object.x = GetFloat(node, "x").value_or(0.0f);
  object.y = GetFloat(node, "y").value_or(0.0f);
  object.width = GetFloat(node, "width").value_or(0.0f);
  object.height = GetFloat(node, "height").value_or(0.0f);
  object.name = GetString(node, "name").value_or(std::string{});
  object.tag = GetString(node, "type").value_or(std::string{});
  object.visible = GetBool(node, "visible").value_or(true);

  if (!node.child("point").empty()) {
    object.type = ObjectType::Point;
  }
  else if (!node.child("ellipse").empty()) {
    object.type = ObjectType::Ellipse;
  }
  else {
    object.type = ObjectType::Rectangle;
  }

  if (const auto err = ParseProperties(node, object.properties); err != ParseError::None)
  {
    return err;
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
