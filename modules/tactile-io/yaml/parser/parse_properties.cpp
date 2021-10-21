#include "parse_properties.hpp"

#include <string>   // string
#include <utility>  // move

#include <magic_enum.hpp>  // enum_cast
#include <yaml-cpp/yaml.h>

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseType(const YAML::Node& node)
    -> tl::expected<PropertyType, ParseError>
{
  PropertyType type;

  if (auto propType = node["type"]) {
    const auto name = propType.as<std::string>();
    if (const auto value = magic_enum::enum_cast<PropertyType>(name)) {
      type = *value;
    }
    else {
      return tl::make_unexpected(ParseError::PropertyUnknownType);
    }
  }
  else {
    return tl::make_unexpected(ParseError::PropertyMissingType);
  }

  return type;
}

[[nodiscard]] auto ParseValue(const YAML::Node& node, const PropertyType type)
    -> tl::expected<PropertyValue, ParseError>
{
  PropertyValue result;

  if (auto value = node["value"]) {
    try {
      switch (type) {
        case PropertyType::String:
          result.SetValue(value.as<std::string>());
          break;

        case PropertyType::Integer:
          result.SetValue(value.as<PropertyValue::integer_type>());
          break;

        case PropertyType::Floating:
          result.SetValue(value.as<PropertyValue::float_type>());
          break;

        case PropertyType::Boolean:
          result.SetValue(value.as<bool>());
          break;

        case PropertyType::File:
          result.SetValue(std::filesystem::path{value.as<std::string>()});
          break;

        case PropertyType::Color: {
          const auto hex = value.as<std::string>();
          if (const auto color = cen::color::from_rgba(hex)) {
            result.SetValue(*color);
          }
          else {
            return tl::make_unexpected(ParseError::CouldNotParseProperty);
          }
          break;
        }

        case PropertyType::Object:
          result.SetValue(ObjectRef{value.as<ObjectRef::value_type>()});
          break;
      }
    }
    catch (...) {
      return tl::make_unexpected(ParseError::CouldNotParseProperty);
    }
  }
  else {
    return tl::make_unexpected(ParseError::CouldNotParseProperty);
  }

  return result;
}

[[nodiscard]] auto ParseProperty(const YAML::Node& node)
    -> tl::expected<PropertyData, ParseError>
{
  PropertyData data;

  if (auto name = node["name"]) {
    data.name = name.as<std::string>();
  }
  else {
    return tl::make_unexpected(ParseError::PropertyMissingName);
  }

  if (const auto type = ParseType(node)) {
    if (auto value = ParseValue(node, *type)) {
      data.property = std::move(*value);
    }
    else {
      return tl::make_unexpected(value.error());
    }
  }
  else {
    return tl::make_unexpected(type.error());
  }

  return data;
}

}  // namespace

auto ParseProperties(const YAML::Node& node)
    -> tl::expected<std::vector<PropertyData>, ParseError>
{
  std::vector<PropertyData> properties;

  if (auto propertySeq = node["properties"]) {
    properties.reserve(propertySeq.size());

    for (const auto& propertyNode : propertySeq) {
      if (auto property = ParseProperty(propertyNode)) {
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
