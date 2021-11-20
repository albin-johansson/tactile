#include "parse_object.hpp"

#include <string>  // string

#include "parse_properties.hpp"
#include "xml_utils.hpp"

namespace Tactile::IO {

auto ParseObject(const pugi::xml_node node, Object& object) -> ParseError
{
  if (const auto id = GetInt(node, "id")) {
    SetId(object, *id);
  }
  else {
    return ParseError::ObjectMissingId;
  }

  SetName(object, node.attribute("name").as_string());
  SetX(object, GetFloat(node, "x").value_or(0.0f));
  SetY(object, GetFloat(node, "y").value_or(0.0f));
  SetWidth(object, GetFloat(node, "width").value_or(0.0f));
  SetHeight(object, GetFloat(node, "height").value_or(0.0f));
  SetTag(object, node.attribute("type").as_string());
  SetVisible(object, GetBool(node, "visible").value_or(true));

  if (!node.child("point").empty()) {
    SetType(object, ObjectType::Point);
  }
  else if (!node.child("ellipse").empty()) {
    SetType(object, ObjectType::Ellipse);
  }
  else {
    SetType(object, ObjectType::Rectangle);
  }

  if (const auto err = ParseProperties(node, object); err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
