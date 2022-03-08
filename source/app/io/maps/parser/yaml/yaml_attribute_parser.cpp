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

#include <centurion.hpp>

namespace tactile::parsing {
namespace {

[[nodiscard]] auto _parse_attribute_type(const std::string_view type)
    -> maybe<AttributeType>
{
  if (type == "string") {
    return AttributeType::string;
  }
  else if (type == "int") {
    return AttributeType::integer;
  }
  else if (type == "float") {
    return AttributeType::floating;
  }
  else if (type == "bool") {
    return AttributeType::boolean;
  }
  else if (type == "color") {
    return AttributeType::color;
  }
  else if (type == "object") {
    return AttributeType::object;
  }
  else if (type == "file") {
    return AttributeType::file;
  }
  else {
    return nothing;
  }
}

[[nodiscard]] auto _parse_attribute_value(const YAML::Node& value,
                                          const AttributeType type) -> maybe<Attribute>
{
  switch (type) {
    case AttributeType::string:
      return value.as<std::string>();

    case AttributeType::integer:
      return value.as<int32>();

    case AttributeType::floating:
      return value.as<float>();

    case AttributeType::boolean:
      return value.as<bool>();

    case AttributeType::file: {
      const std::filesystem::path file = value.as<std::string>();
      return file;
    }
    case AttributeType::color: {
      const auto hex = value.as<std::string>();
      if (const auto color = cen::color::from_rgba(hex)) {
        return *color;
      }
      else {
        return std::nullopt;
      }
    }
    case AttributeType::object:
      return object_t{value.as<int32>()};

    default:
      return nothing;
  }
}

[[nodiscard]] auto _parse_component_definition_attribute(const YAML::Node& node,
                                                         ir::component_map& def)
    -> parse_error
{
  std::string name;
  if (auto attributeName = node["name"]) {
    name = attributeName.as<std::string>();
  }
  else {
    return parse_error::no_component_def_attribute_name;
  }

  AttributeType type{};
  if (auto attributeType = node["type"]) {
    if (const auto parsedType = _parse_attribute_type(attributeType.as<std::string>())) {
      type = *parsedType;
    }
    else {
      return parse_error::unsupported_component_def_attribute_type;
    }
  }
  else {
    return parse_error::no_component_def_attribute_type;
  }

  auto& value = def[name];
  value.reset_to_default(type);

  if (auto defaultValue = node["default"]) {
    switch (type) {
      case AttributeType::string:
        value = defaultValue.as<std::string>();
        break;

      case AttributeType::integer:
        value = defaultValue.as<int32>();
        break;

      case AttributeType::floating:
        value = defaultValue.as<float>();
        break;

      case AttributeType::boolean:
        value = defaultValue.as<bool>();
        break;

      case AttributeType::file: {
        std::filesystem::path path = defaultValue.as<std::string>();
        value = std::move(path);
        break;
      }
      case AttributeType::color: {
        if (auto color = cen::color::from_rgba(defaultValue.as<std::string>())) {
          value = *color;
        }
        else {
          return parse_error::corrupt_component_def_attribute_value;
        }
        break;
      }
      case AttributeType::object:
        value = object_t{defaultValue.as<int32>()};
        break;
    }
  }

  return parse_error::none;
}

[[nodiscard]] auto _parse_component_definition(const YAML::Node& node, ir::map_data& data)
    -> parse_error
{
  std::string type;
  if (auto name = node["name"]) {
    type = name.as<std::string>();
  }
  else {
    return parse_error::no_component_def_name;
  }

  auto& def = data.component_definitions[type];

  if (auto attributeSeq = node["attributes"]) {
    for (auto attributeNode : attributeSeq) {
      if (const auto err = _parse_component_definition_attribute(attributeNode, def);
          err != parse_error::none) {
        return err;
      }
    }
  }

  return parse_error::none;
}

[[nodiscard]] auto _parse_component(const YAML::Node& node,
                                    const ir::map_data& map,
                                    ir::attribute_context_data& data) -> parse_error
{
  std::string type;
  if (auto typeName = node["type"]) {
    type = typeName.as<std::string>();
  }
  else {
    return parse_error::no_component_type;
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
        return parse_error::no_component_attribute_name;
      }

      if (auto value = valueNode["value"]) {
        const auto attrType = prototype.at(attr).type();
        if (auto attributeValue = _parse_attribute_value(value, attrType)) {
          attributes[attr] = std::move(*attributeValue);
        }
        else {
          return parse_error::corrupt_component_attribute_value;
        }
      }
      else {
        return parse_error::no_component_attribute_value;
      }
    }
  }

  return parse_error::none;
}

}  // namespace

auto parse_component_definitions(const YAML::Node& node, ir::map_data& data)
    -> parse_error
{
  if (auto sequence = node["component-definitions"]) {
    for (const auto& defNode : sequence) {
      if (const auto err = _parse_component_definition(defNode, data);
          err != parse_error::none) {
        return err;
      }
    }
  }

  return parse_error::none;
}

auto parse_components(const YAML::Node& node,
                      const ir::map_data& map,
                      ir::attribute_context_data& data) -> parse_error
{
  if (auto sequence = node["components"]) {
    for (const auto& componentNode : sequence) {
      if (const auto err = _parse_component(componentNode, map, data);
          err != parse_error::none) {
        return err;
      }
    }
  }

  return parse_error::none;
}

auto parse_properties(const YAML::Node& node, ir::attribute_context_data& data)
    -> parse_error
{
  if (auto sequence = node["properties"]) {
    for (const auto& propertyNode : sequence) {
      std::string propertyName;
      AttributeType propertyType{};

      if (auto name = propertyNode["name"]) {
        propertyName = name.as<std::string>();
      }
      else {
        return parse_error::no_property_name;
      }

      if (auto type = propertyNode["type"]) {
        if (auto parsedType = _parse_attribute_type(type.as<std::string>())) {
          propertyType = *parsedType;
        }
        else {
          return parse_error::unsupported_property_type;
        }
      }
      else {
        return parse_error::no_property_type;
      }

      if (auto valueAttr = propertyNode["value"]) {
        if (auto value = _parse_attribute_value(valueAttr, propertyType)) {
          data.properties[std::move(propertyName)] = std::move(*value);
        }
        else {
          return parse_error::corrupt_property_value;
        }
      }
      else {
        return parse_error::corrupt_property_value;  // TODO no_property_value
      }
    }
  }

  return parse_error::none;
}

}  // namespace tactile::parsing
