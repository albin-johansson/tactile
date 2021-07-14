#include "parse_properties.hpp"

#include <filesystem>  // path
#include <string>      // string
#include <utility>     // move

#include "utils/color_utils.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ToProperty(const IMapObject& object,
                              const std::string& type,
                              Property& property) -> ParseError
{
  if (type == "string")
  {
    property.SetValue(object.GetString(MapAttribute::Value).value());
  }
  else if (type == "int")
  {
    property.SetValue(object.GetInt(MapAttribute::Value).value());
  }
  else if (type == "float")
  {
    property.SetValue(object.GetFloat(MapAttribute::Value).value());
  }
  else if (type == "bool")
  {
    property.SetValue(object.GetBool(MapAttribute::Value).value());
  }
  else if (type == "file")
  {
    const auto file = object.GetString(MapAttribute::Value).value();
    property.SetValue(std::filesystem::path{file});
  }
  else if (type == "object")
  {
    const auto obj = object.GetInt(MapAttribute::Value).value();
    property.SetValue(object_ref{obj});
  }
  else if (type == "color")
  {
    const auto string = object.GetString(MapAttribute::Value).value();
    const auto color = (string.size() == 9) ? cen::color::from_argb(string)
                                            : cen::color::from_rgb(string);
    if (color)
    {
      property.SetValue(*color);
    }
    else
    {
      return ParseError::CouldNotParseProperty;
    }
  }
  else
  {
    return ParseError::PropertyUnknownType;
  }

  return ParseError::None;
}

[[nodiscard]] auto ParseProperty(const IMapObject& object, PropertyData& data)
    -> ParseError
{
  if (auto name = object.GetString(MapAttribute::Name))
  {
    data.name = std::move(*name);
  }
  else
  {
    return ParseError::PropertyMissingName;
  }

  if (object.IsImplicitStringProperty())
  {
    Property property;
    if (const auto error = ToProperty(object, "string", property);
        error == ParseError::None)
    {
      data.property = std::move(property);
    }
    else
    {
      return error;
    }
  }
  else if (auto type = object.GetString(MapAttribute::Type))
  {
    Property property;
    if (const auto error = ToProperty(object, *type, property);
        error == ParseError::None)
    {
      data.property = std::move(property);
    }
    else
    {
      return error;
    }
  }
  else
  {
    return ParseError::PropertyUnknownType;
  }

  return ParseError::None;
}

}  // namespace

auto ParseProperties(const IMapObject& object, std::vector<PropertyData>& properties)
    -> ParseError
{
  const auto objects = object.GetProperties();
  properties.reserve(objects.size());

  for (const auto& elem : objects)
  {
    PropertyData data;
    if (const auto error = ParseProperty(*elem, data); error == ParseError::None)
    {
      properties.push_back(data);
    }
    else
    {
      return error;
    }
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
