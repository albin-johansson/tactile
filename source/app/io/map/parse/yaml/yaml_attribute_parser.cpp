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

#include <string>       // string
#include <string_view>  // string_view
#include <utility>      // move

#include <centurion/color.hpp>

#include "core/common/filesystem.hpp"
#include "core/common/maybe.hpp"
#include "io/map/ir/ir.hpp"
#include "io/map/parse/yaml/yaml_parser.hpp"
#include "io/util/yaml.hpp"

namespace tactile::io {
namespace {

[[nodiscard]] auto parse_attribute_type(const std::string_view type)
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

[[nodiscard]] auto parse_attribute_value(const YAML::Node&   value,
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
      const fs::path file = value.as<std::string>();
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
      return object_t {value.as<int32>()};

    default:
      return nothing;
  }
}

[[nodiscard]] auto parse_component_definition_attribute(const YAML::Node& node,
                                                        ir::ComponentMap& def)
    -> ParseError
{
  std::string name;
  if (!read_attribute(node, "name", name)) {
    return ParseError::NoComponentDefAttributeName;
  }

  std::string type_name;
  if (!read_attribute(node, "type", type_name)) {
    return ParseError::NoComponentDefAttributeType;
  }

  AttributeType type {};
  if (const auto parsed_type = parse_attribute_type(type_name)) {
    type = *parsed_type;
  }
  else {
    return ParseError::UnsupportedComponentDefAttributeType;
  }

  auto& value = def[name];
  value.reset_to_default(type);

  if (auto default_value = node["default"]) {
    switch (type) {
      case AttributeType::String:
        value = default_value.as<std::string>();
        break;

      case AttributeType::Int:
        value = default_value.as<int32>();
        break;

      case AttributeType::Float:
        value = default_value.as<float>();
        break;

      case AttributeType::Bool:
        value = default_value.as<bool>();
        break;

      case AttributeType::Path: {
        fs::path path = default_value.as<std::string>();
        value = std::move(path);
        break;
      }
      case AttributeType::Color: {
        if (auto color = cen::color::from_rgba(default_value.as<std::string>())) {
          value = *color;
        }
        else {
          return ParseError::CorruptComponentDefAttributeValue;
        }
        break;
      }
      case AttributeType::Object:
        value = object_t {default_value.as<int32>()};
        break;
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto parse_component_definition(const YAML::Node& node, ir::MapData& data)
    -> ParseError
{
  std::string type;
  if (!read_attribute(node, "name", type)) {
    return ParseError::NoComponentDefName;
  }

  auto& def = data.component_definitions[type];

  if (auto attribute_seq = node["attributes"]) {
    for (auto attribute_node : attribute_seq) {
      if (const auto err = parse_component_definition_attribute(attribute_node, def);
          err != ParseError::None) {
        return err;
      }
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto parse_component(const YAML::Node&  node,
                                   const ir::MapData& map,
                                   ir::ContextData&   data) -> ParseError
{
  std::string type;
  if (!read_attribute(node, "type", type)) {
    return ParseError::NoComponentType;
  }

  // TODO invalid component type check, e.g. ParseError::InvalidComponentType
  const auto& prototype = map.component_definitions.at(type);
  auto&       attributes = data.components[type];

  if (auto sequence = node["values"]) {
    for (const auto& value_node : sequence) {
      std::string attr_name;
      if (!read_attribute(value_node, "name", attr_name)) {
        return ParseError::NoComponentAttributeName;
      }

      if (auto value = value_node["value"]) {
        const auto attr_type = prototype.at(attr_name).type();
        if (auto attribute_value = parse_attribute_value(value, attr_type)) {
          attributes[attr_name] = std::move(*attribute_value);
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
    for (const auto& def_node : sequence) {
      if (const auto err = parse_component_definition(def_node, data);
          err != ParseError::None) {
        return err;
      }
    }
  }

  return ParseError::None;
}

auto parse_components(const YAML::Node&  node,
                      const ir::MapData& map,
                      ir::ContextData&   data) -> ParseError
{
  if (auto sequence = node["components"]) {
    for (const auto& component_node : sequence) {
      if (const auto err = parse_component(component_node, map, data);
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
    for (const auto& property_node : sequence) {
      std::string property_name;
      if (!read_attribute(property_node, "name", property_name)) {
        return ParseError::NoPropertyName;
      }

      std::string type;
      if (!read_attribute(property_node, "type", type)) {
        return ParseError::NoPropertyType;
      }

      AttributeType property_type {};
      if (auto parsed_type = parse_attribute_type(type)) {
        property_type = *parsed_type;
      }
      else {
        return ParseError::UnsupportedPropertyType;
      }

      if (auto value_attr = property_node["value"]) {
        if (auto value = parse_attribute_value(value_attr, property_type)) {
          data.properties[std::move(property_name)] = std::move(*value);
        }
        else {
          return ParseError::CorruptPropertyValue;
        }
      }
      else {
        // TODO make it so that values can be omitted if using default value?
        return ParseError::CorruptPropertyValue;
      }
    }
  }

  return ParseError::None;
}

}  // namespace tactile::io
