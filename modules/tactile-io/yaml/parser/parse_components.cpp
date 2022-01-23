#include "parse_components.hpp"

#include <string>  // string

#include <yaml-cpp/yaml.h>

#include "common_parsing.hpp"

namespace tactile::IO {
namespace {

template <typename T>
[[nodiscard]] auto ParseComponent(const Map& map, const YAML::Node& node, T& target)
    -> ParseError
{
  std::string type;
  if (auto typeName = node["type"]) {
    type = typeName.as<std::string>();
  }
  else {
    return ParseError::ComponentMissingType;
  }

  const auto& def = GetComponentDef(map, type.c_str());
  auto& component = AddComponent(target, def);

  if (auto sequence = node["values"]) {
    for (auto valueNode : sequence) {
      std::string attr;
      if (auto name = valueNode["name"]) {
        attr = name.as<std::string>();
      }
      else {
        return ParseError::ComponentAttributeMissingName;
      }

      if (auto value = valueNode["value"]) {
        switch (GetAttributeType(def, attr.c_str())) {
          case PropertyType::String:
            SetAttributeString(component, attr.c_str(), value.as<std::string>().c_str());
            break;

          case PropertyType::Integer:
            SetAttributeInt(component, attr.c_str(), value.as<int32>());
            break;

          case PropertyType::Floating:
            SetAttributeFloat(component, attr.c_str(), value.as<float>());
            break;

          case PropertyType::Boolean:
            SetAttributeBool(component, attr.c_str(), value.as<bool>());
            break;

          case PropertyType::File: {
            const std::filesystem::path file = value.as<std::string>();
            SetAttributeFile(component, attr.c_str(), file.c_str());
            break;
          }
          case PropertyType::Color: {
            const auto hex = value.as<std::string>();
            if (const auto color = ParseColorRGBA(hex)) {
              SetAttributeColor(component, attr.c_str(), *color);
            }
            else {
              return ParseError::CouldNotParseComponentAttributeValue;
            }
            break;
          }
          case PropertyType::Object:
            SetAttributeObject(component, attr.c_str(), value.as<int32>());
            break;
        }
      }
      else {
        return ParseError::ComponentAttributeMissingValue;
      }
    }
  }
  else {
    if (GetAttributeCount(def) != 0u) {
      return ParseError::ComponentMissingValues;
    }
  }

  return ParseError::None;
}

template <typename T>
auto ParseComponentsImpl(const Map& map, const YAML::Node& node, T& target) -> ParseError
{
  if (auto sequence = node["components"]) {
    ReserveComponents(target, sequence.size());

    for (const auto& componentNode : sequence) {
      if (const auto err = ParseComponent(map, componentNode, target);
          err != ParseError::None) {
        return err;
      }
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto ParseComponentDefAttribute(const YAML::Node& node, ComponentDef& def)
    -> ParseError
{
  std::string name;
  if (auto attributeName = node["name"]) {
    name = attributeName.as<std::string>();
  }
  else {
    return ParseError::ComponentDefAttributeMissingName;
  }

  PropertyType type{};
  if (auto attributeType = node["type"]) {
    if (const auto parsedType = ParseAttributeType(attributeType.as<std::string>())) {
      type = *parsedType;
    }
    else {
      return ParseError::ComponentDefAttributeInvalidType;
    }
  }
  else {
    return ParseError::ComponentDefAttributeMissingType;
  }

  DefineAttribute(def, name.c_str(), type);

  if (auto defaultValue = node["default"]) {
    switch (type) {
      case PropertyType::String:
        SetAttributeDefaultString(def,
                                  name.c_str(),
                                  defaultValue.as<std::string>().c_str());
        break;

      case PropertyType::Integer:
        SetAttributeDefaultInt(def, name.c_str(), defaultValue.as<int32>());
        break;

      case PropertyType::Floating:
        SetAttributeDefaultFloat(def, name.c_str(), defaultValue.as<float>());
        break;

      case PropertyType::Boolean:
        SetAttributeDefaultBool(def, name.c_str(), defaultValue.as<bool>());
        break;

      case PropertyType::File: {
        std::filesystem::path path = defaultValue.as<std::string>();
        SetAttributeDefaultFile(def, name.c_str(), path.c_str());
        break;
      }

      case PropertyType::Color: {
        if (auto color = ParseColorRGBA(defaultValue.as<std::string>())) {
          SetAttributeDefaultColor(def, name.c_str(), *color);
        }
        else {
          return ParseError::ComponentDefAttributeInvalidValue;
        }
        break;
      }

      case PropertyType::Object:
        SetAttributeDefaultObject(def, name.c_str(), defaultValue.as<int32>());
        break;
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto ParseComponentDef(const YAML::Node& node, Map& map) -> ParseError
{
  std::string type;
  if (auto name = node["name"]) {
    type = name.as<std::string>();
  }
  else {
    return ParseError::ComponentDefMissingName;
  }

  auto& def = DefineComponent(map, type.c_str());

  if (auto attributeSeq = node["attributes"]) {
    for (auto attributeNode : attributeSeq) {
      if (const auto err = ParseComponentDefAttribute(attributeNode, def);
          err != ParseError::None) {
        return err;
      }
    }
  }

  return ParseError::None;
}

}  // namespace

auto ParseComponentDefinitions(const YAML::Node& node, Map& map) -> ParseError
{
  if (auto sequence = node["component-definitions"]) {
    ReserveComponentDefinitions(map, sequence.size());

    for (const auto& defNode : sequence) {
      if (const auto err = ParseComponentDef(defNode, map); err != ParseError::None) {
        return err;
      }
    }
  }

  return ParseError::None;
}

auto ParseComponents(const YAML::Node& node, Map& map) -> ParseError
{
  return ParseComponentsImpl(map, node, map);
}

auto ParseComponents(const Map& map, const YAML::Node& node, Tileset& tileset)
    -> ParseError
{
  return ParseComponentsImpl(map, node, tileset);
}

auto ParseComponents(const Map& map, const YAML::Node& node, Tile& tile) -> ParseError
{
  return ParseComponentsImpl(map, node, tile);
}

auto ParseComponents(const Map& map, const YAML::Node& node, Layer& layer) -> ParseError
{
  return ParseComponentsImpl(map, node, layer);
}

auto ParseComponents(const Map& map, const YAML::Node& node, Object& object) -> ParseError
{
  return ParseComponentsImpl(map, node, object);
}

}  // namespace tactile::IO