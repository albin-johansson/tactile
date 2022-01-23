#include "parse_properties.hpp"

#include <filesystem>  // path
#include <string>      // string

#include <tactile_def.hpp>
#include <tactile_stdlib.hpp>
#include <yaml-cpp/yaml.h>

#include "common_parsing.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseType(const YAML::Node& node, PropertyType& type) -> ParseError
{
  if (auto propType = node["type"]) {
    if (const auto parsedType = ParseAttributeType(propType.as<std::string>())) {
      type = *parsedType;
    }
    else {
      return ParseError::PropertyUnknownType;
    }
  }
  else {
    return ParseError::PropertyMissingType;
  }

  return ParseError::None;
}

[[nodiscard]] auto ParseValue(const YAML::Node& node,
                              Property& property,
                              const PropertyType type) -> ParseError
{
  if (auto value = node["value"]) {
    try {
      switch (type) {
        case PropertyType::String:
          AssignString(property, value.as<std::string>().c_str());
          break;

        case PropertyType::Integer:
          AssignInt(property, value.as<int32>());
          break;

        case PropertyType::Floating:
          AssignFloat(property, value.as<float>());
          break;

        case PropertyType::Boolean:
          AssignBool(property, value.as<bool>());
          break;

        case PropertyType::File: {
          const std::filesystem::path file = value.as<std::string>();
          AssignFile(property, file.c_str());
          break;
        }

        case PropertyType::Color: {
          const auto hex = value.as<std::string>();
          if (const auto color = ParseColorRGBA(hex)) {
            AssignColor(property, *color);
          }
          else {
            return ParseError::CouldNotParseProperty;
          }
          break;
        }

        case PropertyType::Object:
          AssignObject(property, value.as<int32>());
          break;
      }
    }
    catch (...) {
      return ParseError::CouldNotParseProperty;
    }
  }
  else {
    return ParseError::CouldNotParseProperty;
  }

  return ParseError::None;
}

[[nodiscard]] auto ParseProperty(const YAML::Node& node, Property& property) -> ParseError
{
  if (auto name = node["name"]) {
    SetName(property, name.as<std::string>().c_str());
  }
  else {
    return ParseError::PropertyMissingName;
  }

  PropertyType type{};
  if (const auto err = ParseType(node, type); err != ParseError::None) {
    return err;
  }

  if (const auto err = ParseValue(node, property, type); err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

template <typename T>
auto ParsePropertiesImpl(const YAML::Node& node, T& obj) -> ParseError
{
  if (auto propertySeq = node["properties"]) {
    ReserveProperties(obj, propertySeq.size());

    for (const auto& propertyNode : propertySeq) {
      auto& property = AddProperty(obj);
      if (const auto err = ParseProperty(propertyNode, property);
          err != ParseError::None) {
        return err;
      }
    }
  }

  return ParseError::None;
}

}  // namespace

auto ParseProperties(const YAML::Node& node, Map& map) -> ParseError
{
  return ParsePropertiesImpl(node, map);
}

auto ParseProperties(const YAML::Node& node, Tileset& tileset) -> ParseError
{
  return ParsePropertiesImpl(node, tileset);
}

auto ParseProperties(const YAML::Node& node, Tile& tile) -> ParseError
{
  return ParsePropertiesImpl(node, tile);
}

auto ParseProperties(const YAML::Node& node, Layer& layer) -> ParseError
{
  return ParsePropertiesImpl(node, layer);
}

auto ParseProperties(const YAML::Node& node, Object& object) -> ParseError
{
  return ParsePropertiesImpl(node, object);
}

}  // namespace Tactile::IO
