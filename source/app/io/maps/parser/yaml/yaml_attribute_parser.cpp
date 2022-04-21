/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "yaml_attribute_parser.hpp"

#include <filesystem>   // path
#include <optional>     // optional
#include <string>       // string
#include <string_view>  // string_view
#include <utility>      // move

#include <centurion/color.hpp>

namespace tactile::parsing {
namespace {

[[nodiscard]] auto _parse_attribute_type(const std::string_view type)
    -> std::optional<AttributeType>
{
  if (type == "string") {
    return AttributeType::String;
  }
  else if (type == "int") {
    return AttributeType::Int;
  }
  else if (type == "float") {
    return AttributeType::Float;
  }
  else if (type == "bool") {
    return AttributeType::Bool;
  }
  else if (type == "color") {
    return AttributeType::Color;
  }
  else if (type == "object") {
    return AttributeType::Object;
  }
  else if (type == "file") {
    return AttributeType::Path;
  }
  else {
    return std::nullopt;
  }
}

[[nodiscard]] auto _parse_attribute_value(const YAML::Node& value,
                                          const AttributeType type)
    -> std::optional<Attribute>
{
  switch (type) {
    case AttributeType::String:
      return value.as<std::string>();

    case AttributeType::Int:
      return value.as<int32>();

    case AttributeType::Float:
      return value.as<float>();

    case AttributeType::Bool:
      return value.as<bool>();

    case AttributeType::Path: {
      const std::filesystem::path file = value.as<std::string>();
      return file;
    }
    case AttributeType::Color: {
      const auto hex = value.as<std::string>();
      if (const auto color = cen::color::from_rgba(hex)) {
        return *color;
      }
      else {
        return std::nullopt;
      }
    }
    case AttributeType::Object:
      return object_t{value.as<int32>()};

    default:
      return std::nullopt;
  }
}

[[nodiscard]] auto _parse_component_definition_attribute(const YAML::Node& node,
                                                         ir::ComponentMap& def)
    -> ParseError
{
  std::string name;
  if (auto attributeName = node["name"]) {
    name = attributeName.as<std::string>();
  }
  else {
    return ParseError::no_component_def_attribute_name;
  }

  AttributeType type{};
  if (auto attributeType = node["type"]) {
    if (const auto parsedType = _parse_attribute_type(attributeType.as<std::string>())) {
      type = *parsedType;
    }
    else {
      return ParseError::unsupported_component_def_attribute_type;
    }
  }
  else {
    return ParseError::no_component_def_attribute_type;
  }

  auto& value = def[name];
  value.reset_to_default(type);

  if (auto defaultValue = node["default"]) {
    switch (type) {
      case AttributeType::String:
        value = defaultValue.as<std::string>();
        break;

      case AttributeType::Int:
        value = defaultValue.as<int32>();
        break;

      case AttributeType::Float:
        value = defaultValue.as<float>();
        break;

      case AttributeType::Bool:
        value = defaultValue.as<bool>();
        break;

      case AttributeType::Path: {
        std::filesystem::path path = defaultValue.as<std::string>();
        value = std::move(path);
        break;
      }
      case AttributeType::Color: {
        if (auto color = cen::color::from_rgba(defaultValue.as<std::string>())) {
          value = *color;
        }
        else {
          return ParseError::corrupt_component_def_attribute_value;
        }
        break;
      }
      case AttributeType::Object:
        value = object_t{defaultValue.as<int32>()};
        break;
    }
  }

  return ParseError::none;
}

[[nodiscard]] auto _parse_component_definition(const YAML::Node& node, ir::MapData& data)
    -> ParseError
{
  std::string type;
  if (auto name = node["name"]) {
    type = name.as<std::string>();
  }
  else {
    return ParseError::no_component_def_name;
  }

  auto& def = data.component_definitions[type];

  if (auto attributeSeq = node["attributes"]) {
    for (auto attributeNode : attributeSeq) {
      if (const auto err = _parse_component_definition_attribute(attributeNode, def);
          err != ParseError::none) {
        return err;
      }
    }
  }

  return ParseError::none;
}

[[nodiscard]] auto _parse_component(const YAML::Node& node,
                                    const ir::MapData& map,
                                    ir::AttributeContextData& data) -> ParseError
{
  std::string type;
  if (auto typeName = node["type"]) {
    type = typeName.as<std::string>();
  }
  else {
    return ParseError::no_component_type;
  }

  // TODO invalid component type check, e.g. parse_error::invalid_component_type
  const auto& prototype = map.component_definitions.at(type);
  auto& attributes = data.components[type];

  if (auto sequence = node["values"]) {
    for (const auto& valueNode : sequence) {
      std::string attr;
      if (auto name = valueNode["name"]) {
        attr = name.as<std::string>();
      }
      else {
        return ParseError::no_component_attribute_name;
      }

      if (auto value = valueNode["value"]) {
        const auto attrType = prototype.at(attr).type();
        if (auto attributeValue = _parse_attribute_value(value, attrType)) {
          attributes[attr] = std::move(*attributeValue);
        }
        else {
          return ParseError::corrupt_component_attribute_value;
        }
      }
      else {
        return ParseError::no_component_attribute_value;
      }
    }
  }

  return ParseError::none;
}

}  // namespace

auto parse_component_definitions(const YAML::Node& node, ir::MapData& data) -> ParseError
{
  if (auto sequence = node["component-definitions"]) {
    for (const auto& defNode : sequence) {
      if (const auto err = _parse_component_definition(defNode, data);
          err != ParseError::none) {
        return err;
      }
    }
  }

  return ParseError::none;
}

auto parse_components(const YAML::Node& node,
                      const ir::MapData& map,
                      ir::AttributeContextData& data) -> ParseError
{
  if (auto sequence = node["components"]) {
    for (const auto& componentNode : sequence) {
      if (const auto err = _parse_component(componentNode, map, data);
          err != ParseError::none) {
        return err;
      }
    }
  }

  return ParseError::none;
}

auto parse_properties(const YAML::Node& node, ir::AttributeContextData& data)
    -> ParseError
{
  if (auto sequence = node["properties"]) {
    for (const auto& propertyNode : sequence) {
      std::string propertyName;
      AttributeType propertyType{};

      if (auto name = propertyNode["name"]) {
        propertyName = name.as<std::string>();
      }
      else {
        return ParseError::no_property_name;
      }

      if (auto type = propertyNode["type"]) {
        if (auto parsedType = _parse_attribute_type(type.as<std::string>())) {
          propertyType = *parsedType;
        }
        else {
          return ParseError::unsupported_property_type;
        }
      }
      else {
        return ParseError::no_property_type;
      }

      if (auto valueAttr = propertyNode["value"]) {
        if (auto value = _parse_attribute_value(valueAttr, propertyType)) {
          data.properties[std::move(propertyName)] = std::move(*value);
        }
        else {
          return ParseError::corrupt_property_value;
        }
      }
      else {
        return ParseError::corrupt_property_value;  // TODO no_property_value
      }
    }
  }

  return ParseError::none;
}

}  // namespace tactile::parsing
