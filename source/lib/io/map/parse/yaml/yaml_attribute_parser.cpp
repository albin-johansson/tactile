/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include <concepts>  // same_as
#include <utility>   // move

#include "common/type/maybe.hpp"
#include "common/type/string.hpp"
#include "common/util/str.hpp"
#include "io/ir/ir.hpp"
#include "io/map/parse/yaml/yaml_parser.hpp"
#include "io/util/yaml.hpp"

namespace tactile::io {
namespace {

template <typename T>
[[nodiscard]] auto parse_vector(const YAML::Node& value) -> Maybe<T>
{
  using ScalarType = typename T::value_type;

  const auto raw_value = value.as<String>();
  const auto components = split(raw_value, ';');

  T vec {};
  if (components.size() != vec.length()) {
    return nothing;
  }

  int index = 0;
  for (const auto& component_str: components) {
    Maybe<ScalarType> component_value;

    if constexpr (std::same_as<ScalarType, float>) {
      component_value = parse_f32(component_str);
    }
    else {
      component_value = parse_i32(component_str);
    }

    if (component_value.has_value()) {
      vec[index] = *component_value;
    }
    else {
      return nothing;
    }

    ++index;
  }

  return vec;
}

[[nodiscard]] auto parse_attribute_value(const YAML::Node& value,
                                         const AttributeType type) -> Maybe<Attribute>
{
  switch (type) {
    case AttributeType::String:
      return value.as<String>();

    case AttributeType::Int:
      return value.as<int32>();

    case AttributeType::Int2:
      return parse_vector<Int2>(value);

    case AttributeType::Int3:
      return parse_vector<Int3>(value);

    case AttributeType::Int4:
      return parse_vector<Int4>(value);

    case AttributeType::Float:
      return value.as<float>();

    case AttributeType::Float2:
      return parse_vector<Float2>(value);

    case AttributeType::Float3:
      return parse_vector<Float3>(value);

    case AttributeType::Float4:
      return parse_vector<Float4>(value);

    case AttributeType::Bool:
      return value.as<bool>();

    case AttributeType::Path:
      return Path {value.as<String>()};

    case AttributeType::Color: {
      const auto hex = value.as<String>();
      if (const auto color = Color::from_rgba(hex)) {
        return *color;
      }
      else {
        return nothing;
      }
    }
    case AttributeType::Object:
      return ObjectRef {value.as<int32>()};

    default:
      return nothing;
  }
}

[[nodiscard]] auto parse_component_definition_attribute(const YAML::Node& node)
    -> Expected<Attribute, ParseError>
{
  String name;
  if (!read_attribute(node, "name", name)) {
    return error(ParseError::NoComponentDefAttributeName);
  }

  String type_name;
  if (!read_attribute(node, "type", type_name)) {
    return error(ParseError::NoComponentDefAttributeType);
  }

  AttributeType type {};
  if (const auto parsed_type = parse_attr_type(type_name)) {
    type = *parsed_type;
  }
  else {
    return error(ParseError::UnsupportedComponentDefAttributeType);
  }

  Attribute value;
  value.reset_to_default(type);

  if (auto default_value = node["default"]) {
    switch (type) {
      case AttributeType::String:
        value = default_value.as<String>();
        break;

      case AttributeType::Int:
        value = default_value.as<int32>();
        break;

      case AttributeType::Int2:
        if (const auto vec = parse_vector<Int2>(default_value)) {
          value = *vec;
        }
        else {
          return error(ParseError::CorruptPropertyValue);
        }
        break;

      case AttributeType::Int3:
        if (const auto vec = parse_vector<Int3>(default_value)) {
          value = *vec;
        }
        else {
          return error(ParseError::CorruptPropertyValue);
        }
        break;

      case AttributeType::Int4:
        if (const auto vec = parse_vector<Int4>(default_value)) {
          value = *vec;
        }
        else {
          return error(ParseError::CorruptPropertyValue);
        }
        break;

      case AttributeType::Float:
        value = default_value.as<float>();
        break;

      case AttributeType::Float2:
        if (const auto vec = parse_vector<Float2>(default_value)) {
          value = *vec;
        }
        else {
          return error(ParseError::CorruptPropertyValue);
        }
        break;

      case AttributeType::Float3:
        if (const auto vec = parse_vector<Float3>(default_value)) {
          value = *vec;
        }
        else {
          return error(ParseError::CorruptPropertyValue);
        }
        break;

      case AttributeType::Float4:
        if (const auto vec = parse_vector<Float4>(default_value)) {
          value = *vec;
        }
        else {
          return error(ParseError::CorruptPropertyValue);
        }
        break;

      case AttributeType::Bool:
        value = default_value.as<bool>();
        break;

      case AttributeType::Path: {
        Path path = default_value.as<String>();
        value = std::move(path);
        break;
      }
      case AttributeType::Color: {
        if (const auto color = Color::from_rgba(default_value.as<String>())) {
          value = *color;
        }
        else {
          return error(ParseError::CorruptComponentDefAttributeValue);
        }
        break;
      }
      case AttributeType::Object:
        value = ObjectRef {default_value.as<int32>()};
        break;
    }
  }

  return value;
}

[[nodiscard]] auto parse_component_definition(const YAML::Node& node)
    -> Expected<ir::AttributeMap, ParseError>
{
  ir::AttributeMap def;

  if (auto attribute_seq = node["attributes"]) {
    for (auto attribute_node: attribute_seq) {
      String attribute_name;
      if (!read_attribute(attribute_node, "name", attribute_name)) {
        return error(ParseError::NoComponentDefAttributeName);
      }

      if (auto attr = parse_component_definition_attribute(attribute_node)) {
        def[std::move(attribute_name)] = std::move(*attr);
      }
      else {
        return pass_on_error(attr);
      }
    }
  }

  return def;
}

[[nodiscard]] auto parse_component(const YAML::Node& node,
                                   const ir::MapData& map,
                                   const String& type)
    -> Expected<ir::AttributeMap, ParseError>
{
  // TODO invalid component type check, e.g. ParseError::InvalidComponentType
  const auto& prototype = map.component_definitions.at(type);
  ir::AttributeMap comp;

  if (auto sequence = node["values"]) {
    for (const auto& value_node: sequence) {
      String attr_name;
      if (!read_attribute(value_node, "name", attr_name)) {
        return error(ParseError::NoComponentAttributeName);
      }

      if (auto value = value_node["value"]) {
        const auto attr_type = prototype.at(attr_name).get_type();
        if (auto attribute_value = parse_attribute_value(value, attr_type)) {
          comp[attr_name] = std::move(*attribute_value);
        }
        else {
          return error(ParseError::CorruptComponentAttributeValue);
        }
      }
      else {
        return error(ParseError::NoComponentAttributeValue);
      }
    }
  }

  return comp;
}

}  // namespace

auto parse_component_definitions(const YAML::Node& node)
    -> Expected<ir::ComponentMap, ParseError>
{
  ir::ComponentMap defs;

  if (auto sequence = node["component-definitions"]) {
    for (const auto& def_node: sequence) {
      String type;
      if (!read_attribute(def_node, "name", type)) {
        return error(ParseError::NoComponentDefName);
      }

      if (auto def = parse_component_definition(def_node)) {
        defs[std::move(type)] = std::move(*def);
      }
      else {
        return pass_on_error(def);
      }
    }
  }

  return defs;
}

auto parse_components(const YAML::Node& node, const ir::MapData& map)
    -> Expected<ir::ComponentMap, ParseError>
{
  ir::ComponentMap comps;

  if (auto sequence = node["components"]) {
    for (const auto& component_node: sequence) {
      String type;
      if (!read_attribute(component_node, "type", type)) {
        return error(ParseError::NoComponentType);
      }

      if (auto comp = parse_component(component_node, map, type)) {
        comps[std::move(type)] = std::move(*comp);
      }
      else {
        return pass_on_error(comp);
      }
    }
  }

  return comps;
}

auto parse_properties(const YAML::Node& node) -> Expected<ir::AttributeMap, ParseError>
{
  ir::AttributeMap props;

  if (auto sequence = node["properties"]) {
    for (const auto& property_node: sequence) {
      String property_name;
      if (!read_attribute(property_node, "name", property_name)) {
        return error(ParseError::NoPropertyName);
      }

      String type;
      if (!read_attribute(property_node, "type", type)) {
        return error(ParseError::NoPropertyType);
      }

      AttributeType property_type {};
      if (auto parsed_type = parse_attr_type(type)) {
        property_type = *parsed_type;
      }
      else {
        return error(ParseError::UnsupportedPropertyType);
      }

      if (auto value_attr = property_node["value"]) {
        if (auto value = parse_attribute_value(value_attr, property_type)) {
          props[std::move(property_name)] = std::move(*value);
        }
        else {
          return error(ParseError::CorruptPropertyValue);
        }
      }
      else {
        // TODO make it so that values can be omitted if using default value?
        return error(ParseError::CorruptPropertyValue);
      }
    }
  }

  return props;
}

auto parse_context(const YAML::Node& node, const ir::MapData& map)
    -> Expected<ir::ContextData, ParseError>
{
  ir::ContextData context;

  if (auto props = parse_properties(node)) {
    context.properties = std::move(*props);
  }
  else {
    return pass_on_error(props);
  }

  if (auto comps = parse_components(node, map)) {
    context.components = std::move(*comps);
  }
  else {
    return pass_on_error(comps);
  }

  return context;
}

}  // namespace tactile::io
