#include "parse_properties.hpp"

#include <filesystem>  // path
#include <string>      // string
#include <utility>     // move

#include <centurion.hpp>
#include <json.hpp>  // json

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseValue(const JSON& json, const std::string& type)
    -> tl::expected<PropertyValue, ParseError>
{
  PropertyValue result;

  if (const auto value = json.at("value"); type == "string") {
    result.SetValue(value.get<std::string>());
  }
  else if (type == "int") {
    result.SetValue(value.get<int>());
  }
  else if (type == "float") {
    result.SetValue(value.get<float>());
  }
  else if (type == "bool") {
    result.SetValue(value.get<bool>());
  }
  else if (type == "file") {
    result.SetValue(std::filesystem::path{value.get<std::string>()});
  }
  else if (type == "object") {
    const auto obj = value.get<int>();
    result.SetValue(ObjectRef{obj});
  }
  else if (type == "color") {
    const auto string = value.get<std::string>();
    const auto color = (string.size() == 9) ? cen::color::from_argb(string)
                                            : cen::color::from_rgb(string);
    if (color) {
      result.SetValue(*color);
    }
    else {
      return tl::make_unexpected(ParseError::CouldNotParseProperty);
    }
  }
  else {
    return tl::make_unexpected(ParseError::PropertyUnknownType);
  }

  return result;
}

[[nodiscard]] auto ParseProperty(const JSON& json)
    -> tl::expected<PropertyData, ParseError>
{
  PropertyData result;

  if (const auto it = json.find("name"); it != json.end()) {
    it->get_to(result.name);
  }
  else {
    return tl::make_unexpected(ParseError::PropertyMissingName);
  }

  if (const auto it = json.find("type"); it != json.end()) {
    const auto type = it->get<std::string>();

    if (auto value = ParseValue(json, type)) {
      result.value = std::move(*value);
    }
    else {
      return tl::make_unexpected(value.error());
    }
  }
  else {
    return tl::make_unexpected(ParseError::PropertyMissingType);
  }

  return result;
}

}  // namespace

auto ParseProperties(const JSON& json)
    -> tl::expected<std::vector<PropertyData>, ParseError>
{
  std::vector<PropertyData> properties;

  if (const auto it = json.find("properties"); it != json.end()) {
    properties.reserve(it->size());
    for (const auto& [key, value] : it->items()) {
      if (auto property = ParseProperty(value)) {
        properties.push_back(std::move(*property));
      }
      else {
        return tl::make_unexpected(property.error());
      }
    }
  }

  return properties;
}

}  // namespace Tactile::IO
