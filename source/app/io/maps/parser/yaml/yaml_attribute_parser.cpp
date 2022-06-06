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
#include <string>       // string
#include <string_view>  // string_view
#include <utility>      // move

#include <centurion/color.hpp>

#include "core/common/maybe.hpp"
#include "io/maps/ir.hpp"
#include "io/maps/yaml_utils.hpp"

namespace tactile::io {
namespace {

[[nodiscard]] auto _parse_attribute_type(const std::string_view type)
    -> Maybe<AttributeType>
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
    return nothing;
  }
}

[[nodiscard]] auto _parse_attribute_value(const YAML::Node& value,
                                          const AttributeType type) -> Maybe<Attribute>
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
        return nothing;
      }
    }
    case AttributeType::Object:
      return object_t{value.as<int32>()};

    default:
      return nothing;
  }
}

[[nodiscard]] auto _parse_component_definition_attribute(const YAML::Node& node,
                                                         ir::ComponentMap& def)
    -> ParseError
{
  std::string name;
  if (!read_attribute(node, "name", name)) {
    return ParseError::NoComponentDefAttributeName;
  }

  std::string typeName;
  if (!read_attribute(node, "type", typeName)) {
    return ParseError::NoComponentDefAttributeType;
  }

  AttributeType type{};
  if (const auto parsedType = _parse_attribute_type(typeName)) {
    type = *parsedType;
  }
  else {
    return ParseError::UnsupportedComponentDefAttributeType;
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
          return ParseError::CorruptComponentDefAttributeValue;
        }
        break;
      }
      case AttributeType::Object:
        value = object_t{defaultValue.as<int32>()};
        break;
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto _parse_component_definition(const YAML::Node& node, ir::MapData& data)
    -> ParseError
{
  std::string type;
  if (!read_attribute(node, "name", type)) {
    return ParseError::NoComponentDefName;
  }

  auto& def = data.component_definitions[type];

  if (auto attributeSeq = node["attributes"]) {
    for (auto attributeNode : attributeSeq) {
      if (const auto err = _parse_component_definition_attribute(attributeNode, def);
          err != ParseError::None) {
        return err;
      }
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto _parse_component(const YAML::Node& node,
                                    const ir::MapData& map,
                                    ir::ContextData& data) -> ParseError
{
  std::string type;
  if (!read_attribute(node, "type", type)) {
    return ParseError::NoComponentType;
  }

  // TODO invalid component type check, e.g. ParseError::InvalidComponentType
  const auto& prototype = map.component_definitions.at(type);
  auto& attributes = data.components[type];

  if (auto sequence = node["values"]) {
    for (const auto& valueNode : sequence) {
      std::string attrName;
      if (!read_attribute(valueNode, "name", attrName)) {
        return ParseError::NoComponentAttributeName;
      }

      if (auto value = valueNode["value"]) {
        const auto attrType = prototype.at(attrName).type();
        if (auto attributeValue = _parse_attribute_value(value, attrType)) {
          attributes[attrName] = std::move(*attributeValue);
        }
        else {
          return ParseError::CorruptComponentAttributeValue;
        }
      }
      else {
        return ParseError::NoComponentAttributeValue;
      }
    }
  }

  return ParseError::None;
}

}  // namespace

auto parse_component_definitions(const YAML::Node& node, ir::MapData& data) -> ParseError
{
  if (auto sequence = node["component-definitions"]) {
    for (const auto& defNode : sequence) {
      if (const auto err = _parse_component_definition(defNode, data);
          err != ParseError::None) {
        return err;
      }
    }
  }

  return ParseError::None;
}

auto parse_components(const YAML::Node& node,
                      const ir::MapData& map,
                      ir::ContextData& data) -> ParseError
{
  if (auto sequence = node["components"]) {
    for (const auto& componentNode : sequence) {
      if (const auto err = _parse_component(componentNode, map, data);
          err != ParseError::None) {
        return err;
      }
    }
  }

  return ParseError::None;
}

auto parse_properties(const YAML::Node& node, ir::ContextData& data) -> ParseError
{
  if (auto sequence = node["properties"]) {
    for (const auto& propertyNode : sequence) {
      std::string propertyName;
      if (!read_attribute(propertyNode, "name", propertyName)) {
        return ParseError::NoPropertyName;
      }

      std::string type;
      if (!read_attribute(propertyNode, "type", type)) {
        return ParseError::NoPropertyType;
      }

      AttributeType propertyType{};
      if (auto parsedType = _parse_attribute_type(type)) {
        propertyType = *parsedType;
      }
      else {
        return ParseError::UnsupportedPropertyType;
      }

      if (auto valueAttr = propertyNode["value"]) {
        if (auto value = _parse_attribute_value(valueAttr, propertyType)) {
          data.properties[std::move(propertyName)] = std::move(*value);
        }
        else {
          return ParseError::CorruptPropertyValue;
        }
      }
      else {
        return ParseError::CorruptPropertyValue;  // TODO no_property_value
      }
    }
  }

  return ParseError::None;
}

}  // namespace tactile::io
