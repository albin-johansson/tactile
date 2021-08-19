#include "parse_object_layer.hpp"

#include <string>  // string

#include "parse_properties.hpp"
#include "xml_utils.hpp"

namespace Tactile::IO {

auto ParseObjectLayer(const pugi::xml_node node, LayerData& layer) -> ParseError
{
  auto& data = layer.data.emplace<ObjectLayerData>();

  for (const auto objectNode : node.children("object"))
  {
    auto& object = data.objects.emplace_back();

    if (const auto id = GetInt(objectNode, "id"))
    {
      object.id = object_id{*id};
    }
    else
    {
      return ParseError::ObjectMissingId;
    }

    object.x = GetFloat(objectNode, "x").value_or(0.0f);
    object.y = GetFloat(objectNode, "y").value_or(0.0f);
    object.width = GetFloat(objectNode, "width").value_or(0.0f);
    object.height = GetFloat(objectNode, "height").value_or(0.0f);
    object.name = GetString(objectNode, "name").value_or(std::string{});
    object.custom_type = GetString(objectNode, "type").value_or(std::string{});
    object.visible = GetBool(objectNode, "visible").value_or(true);

    if (!objectNode.child("point").empty())
    {
      object.type = ObjectType::Point;
    }
    else if (!objectNode.child("ellipse").empty())
    {
      object.type = ObjectType::Ellipse;
    }
    else
    {
      object.type = ObjectType::Rectangle;
    }

    if (const auto err = ParseProperties(objectNode, object.properties);
        err != ParseError::None)
    {
      return err;
    }
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
