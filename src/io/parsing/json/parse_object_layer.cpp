#include "parse_object_layer.hpp"

#include <string>  // string

#include "parse_properties.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseObject(const JSON& json, ObjectData& data) -> ParseError
{
  if (const auto it = json.find("id"); it != json.end())
  {
    data.id = object_id{it->get<object_id::value_type>()};
  }
  else
  {
    return ParseError::ObjectMissingId;
  }

  if (const auto it = json.find("x"); it != json.end())
  {
    data.x = it->get<float>();
  }
  else
  {
    data.x = 0;
  }

  if (const auto it = json.find("y"); it != json.end())
  {
    data.y = it->get<float>();
  }
  else
  {
    data.y = 0;
  }

  if (const auto it = json.find("width"); it != json.end())
  {
    data.width = it->get<float>();
  }
  else
  {
    data.width = 0;
  }

  if (const auto it = json.find("height"); it != json.end())
  {
    data.height = it->get<float>();
  }
  else
  {
    data.height = 0;
  }

  if (const auto it = json.find("name"); it != json.end())
  {
    data.name = it->get<std::string>();
  }

  if (const auto it = json.find("type"); it != json.end())
  {
    data.custom_type = it->get<std::string>();
  }

  if (const auto it = json.find("visible"); it != json.end())
  {
    data.visible = it->get<bool>();
  }
  else
  {
    data.visible = true;
  }

  if (json.contains("point"))
  {
    data.type = ObjectType::Point;
  }
  else if (json.contains("ellipse"))
  {
    data.type = ObjectType::Ellipse;
  }
  else
  {
    data.type = ObjectType::Rectangle;
  }

  if (const auto err = ParseProperties(json, data.properties);
      err != ParseError::None)
  {
    return err;
  }

  return ParseError::None;
}

}  // namespace

auto ParseObjectLayer(const JSON& json, LayerData& layer) -> ParseError
{
  auto& data = layer.data.emplace<ObjectLayerData>();

  if (const auto it = json.find("objects"); it != json.end())
  {
    for (const auto& [key, object] : it->items())
    {
      auto& objectData = data.objects.emplace_back();
      if (const auto err = ParseObject(object, objectData); err != ParseError::None)
      {
        return err;
      }
    }
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
