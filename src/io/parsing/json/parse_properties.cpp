#include "parse_properties.hpp"

#include <centurion.hpp>
#include <string>  // string

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ToProperty(const JSON& json,
                              const std::string& type,
                              PropertyValue& property) -> ParseError
{
  const auto value = json.at("value");

  if (type == "string")
  {
    property.SetValue(value.get<std::string>());
  }
  else if (type == "int")
  {
    property.SetValue(value.get<int>());
  }
  else if (type == "float")
  {
    property.SetValue(value.get<float>());
  }
  else if (type == "bool")
  {
    property.SetValue(value.get<bool>());
  }
  else if (type == "file")
  {
    const auto file = value.get<std::string>();
    property.SetValue(std::filesystem::path{file});
  }
  else if (type == "object")
  {
    const auto obj = value.get<int>();
    property.SetValue(ObjectRef{obj});
  }
  else if (type == "color")
  {
    const auto string = value.get<std::string>();
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

[[nodiscard]] auto ParseProperty(const JSON& json, PropertyData& data) -> ParseError
{
  if (const auto it = json.find("name"); it != json.end())
  {
    data.name = it->get<std::string>();
  }
  else
  {
    return ParseError::PropertyMissingName;
  }

  if (const auto it = json.find("type"); it != json.end())
  {
    const auto type = it->get<std::string>();
    if (const auto err = ToProperty(json, type, data.property);
        err != ParseError::None)
    {
      return err;
    }
  }
  else
  {
    return ParseError::PropertyMissingType;
  }

  return ParseError::None;
}

}  // namespace

auto ParseProperties(const JSON& json, std::vector<PropertyData>& properties)
    -> ParseError
{
  if (const auto it = json.find("properties"); it != json.end())
  {
    for (const auto& [key, value] : it->items())
    {
      auto& data = properties.emplace_back();
      if (const auto err = ParseProperty(value, data); err != ParseError::None)
      {
        return err;
      }
    }
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
