#include "parse_object.hpp"

#include <string>  // string

#include "parse_properties.hpp"
#include "xml_utils.hpp"

namespace Tactile::IO {

auto ParseObject(const pugi::xml_node node, Object& object) -> ParseError
{
  if (const auto id = GetInt(node, "id")) {
    IO::SetId(object, *id);
  }
  else {
    return ParseError::ObjectMissingId;
  }

  IO::SetName(object, node.attribute("name").as_string());
  IO::SetX(object, GetFloat(node, "x").value_or(0.0f));
  IO::SetY(object, GetFloat(node, "y").value_or(0.0f));
  IO::SetWidth(object, GetFloat(node, "width").value_or(0.0f));
  IO::SetHeight(object, GetFloat(node, "height").value_or(0.0f));
  IO::SetTag(object, node.attribute("type").as_string());
  IO::SetVisible(object, GetBool(node, "visible").value_or(true));

  if (!node.child("point").empty()) {
    IO::SetType(object, ObjectType::Point);
  }
  else if (!node.child("ellipse").empty()) {
    IO::SetType(object, ObjectType::Ellipse);
  }
  else {
    IO::SetType(object, ObjectType::Rectangle);
  }

  if (const auto err = ParseProperties(node, object); err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
