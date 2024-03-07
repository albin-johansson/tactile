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

#include "common/util/str.hpp"
#include "io/ir/map/map_ir.hpp"
#include "io/map/parse/yaml/yaml_parser.hpp"
#include "io/util/yaml.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/string.hpp"

namespace tactile {
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
    case AttributeType::kStr:
      return value.as<String>();

    case AttributeType::kInt:
      return value.as<int32>();

    case AttributeType::kInt2:
      return parse_vector<Int2>(value);

    case AttributeType::kInt3:
      return parse_vector<Int3>(value);

    case AttributeType::kInt4:
      return parse_vector<Int4>(value);

    case AttributeType::kFloat:
      return value.as<float>();

    case AttributeType::kFloat2:
      return parse_vector<Float2>(value);

    case AttributeType::kFloat3:
      return parse_vector<Float3>(value);

    case AttributeType::kFloat4:
      return parse_vector<Float4>(value);

    case AttributeType::kBool:
      return value.as<bool>();

    case AttributeType::kPath:
      return Path {value.as<String>()};

    case AttributeType::kColor: {
      const auto hex = value.as<String>();
      if (const auto color = Color::from_rgba(hex)) {
        return *color;
      }
      else {
        return nothing;
      }
    }
    case AttributeType::kObject:
      return ObjectRef {value.as<int32>()};

    default:
      return nothing;
  }
}

[[nodiscard]] auto parse_component_definition_attribute(const YAML::Node& node)
    -> Expected<Attribute, ParseError>
{
  String name;
  if (!read_attr(node, "name", name)) {
    return unexpected(ParseError::NoComponentDefAttributeName);
  }

  String type_name;
  if (!read_attr(node, "type", type_name)) {
    return unexpected(ParseError::NoComponentDefAttributeType);
  }

  AttributeType type {};
  if (const auto parsed_type = parse_attr_type(type_name)) {
    type = *parsed_type;
  }
  else {
    return unexpected(ParseError::UnsupportedComponentDefAttributeType);
  }

  Attribute value;
  value.reset_to_default(type);

  if (auto default_value = node["default"]) {
    switch (type) {
      case AttributeType::kStr:
        value = default_value.as<String>();
        break;

      case AttributeType::kInt:
        value = default_value.as<int32>();
        break;

      case AttributeType::kInt2:
        if (const auto vec = parse_vector<Int2>(default_value)) {
          value = *vec;
        }
        else {
          return unexpected(ParseError::CorruptPropertyValue);
        }
        break;

      case AttributeType::kInt3:
        if (const auto vec = parse_vector<Int3>(default_value)) {
          value = *vec;
        }
        else {
          return unexpected(ParseError::CorruptPropertyValue);
        }
        break;

      case AttributeType::kInt4:
        if (const auto vec = parse_vector<Int4>(default_value)) {
          value = *vec;
        }
        else {
          return unexpected(ParseError::CorruptPropertyValue);
        }
        break;

      case AttributeType::kFloat:
        value = default_value.as<float>();
        break;

      case AttributeType::kFloat2:
        if (const auto vec = parse_vector<Float2>(default_value)) {
          value = *vec;
        }
        else {
          return unexpected(ParseError::CorruptPropertyValue);
        }
        break;

      case AttributeType::kFloat3:
        if (const auto vec = parse_vector<Float3>(default_value)) {
          value = *vec;
        }
        else {
          return unexpected(ParseError::CorruptPropertyValue);
        }
        break;

      case AttributeType::kFloat4:
        if (const auto vec = parse_vector<Float4>(default_value)) {
          value = *vec;
        }
        else {
          return unexpected(ParseError::CorruptPropertyValue);
        }
        break;

      case AttributeType::kBool:
        value = default_value.as<bool>();
        break;

      case AttributeType::kPath: {
        Path path = default_value.as<String>();
        value = std::move(path);
        break;
      }
      case AttributeType::kColor: {
        if (const auto color = Color::from_rgba(default_value.as<String>())) {
          value = *color;
        }
        else {
          return unexpected(ParseError::CorruptComponentDefAttributeValue);
        }
        break;
      }
      case AttributeType::kObject:
        value = ObjectRef {default_value.as<int32>()};
        break;
    }
  }

  return value;
}

[[nodiscard]] auto parse_component_definition(const YAML::Node& node)
    -> Expected<AttributeMap, ParseError>
{
  AttributeMap def;

  if (auto attribute_seq = node["attributes"]) {
    for (auto attribute_node: attribute_seq) {
      String attribute_name;
      if (!read_attr(attribute_node, "name", attribute_name)) {
        return unexpected(ParseError::NoComponentDefAttributeName);
      }

      if (auto attr = parse_component_definition_attribute(attribute_node)) {
        def[std::move(attribute_name)] = std::move(*attr);
      }
      else {
        return propagate_unexpected(attr);
      }
    }
  }

  return def;
}

[[nodiscard]] auto parse_component(const YAML::Node& node,
                                   const MapIR& map,
                                   const String& type)
    -> Expected<AttributeMap, ParseError>
{
  // TODO invalid component type check, e.g. ParseError::InvalidComponentType
  const auto& prototype = map.component_definitions.at(type);
  AttributeMap comp;

  if (auto sequence = node["values"]) {
    for (const auto& value_node: sequence) {
      String attr_name;
      if (!read_attr(value_node, "name", attr_name)) {
        return unexpected(ParseError::NoComponentAttributeName);
      }

      if (auto value = value_node["value"]) {
        const auto attr_type = prototype.at(attr_name).get_type();
        if (auto attribute_value = parse_attribute_value(value, attr_type)) {
          comp[attr_name] = std::move(*attribute_value);
        }
        else {
          return unexpected(ParseError::CorruptComponentAttributeValue);
        }
      }
      else {
        return unexpected(ParseError::NoComponentAttributeValue);
      }
    }
  }

  return comp;
}

}  // namespace

auto parse_component_definitions(const YAML::Node& node)
    -> Expected<ComponentMap, ParseError>
{
  ComponentMap defs;

  if (auto sequence = node["component-definitions"]) {
    for (const auto& def_node: sequence) {
      String type;
      if (!read_attr(def_node, "name", type)) {
        return unexpected(ParseError::NoComponentDefName);
      }

      if (auto def = parse_component_definition(def_node)) {
        defs[std::move(type)] = std::move(*def);
      }
      else {
        return propagate_unexpected(def);
      }
    }
  }

  return defs;
}

auto parse_components(const YAML::Node& node, const MapIR& map)
    -> Expected<ComponentMap, ParseError>
{
  ComponentMap comps;

  if (auto sequence = node["components"]) {
    for (const auto& component_node: sequence) {
      String type;
      if (!read_attr(component_node, "type", type)) {
        return unexpected(ParseError::NoComponentType);
      }

      if (auto comp = parse_component(component_node, map, type)) {
        comps[std::move(type)] = std::move(*comp);
      }
      else {
        return propagate_unexpected(comp);
      }
    }
  }

  return comps;
}

auto parse_properties(const YAML::Node& node) -> Expected<AttributeMap, ParseError>
{
  AttributeMap props;

  if (auto sequence = node["properties"]) {
    for (const auto& property_node: sequence) {
      String property_name;
      if (!read_attr(property_node, "name", property_name)) {
        return unexpected(ParseError::NoPropertyName);
      }

      String type;
      if (!read_attr(property_node, "type", type)) {
        return unexpected(ParseError::NoPropertyType);
      }

      AttributeType property_type {};
      if (auto parsed_type = parse_attr_type(type)) {
        property_type = *parsed_type;
      }
      else {
        return unexpected(ParseError::UnsupportedPropertyType);
      }

      if (auto value_attr = property_node["value"]) {
        if (auto value = parse_attribute_value(value_attr, property_type)) {
          props[std::move(property_name)] = std::move(*value);
        }
        else {
          return unexpected(ParseError::CorruptPropertyValue);
        }
      }
      else {
        // TODO make it so that values can be omitted if using default value?
        return unexpected(ParseError::CorruptPropertyValue);
      }
    }
  }

  return props;
}

auto parse_context(const YAML::Node& node, const MapIR& map)
    -> Expected<ContextIR, ParseError>
{
  ContextIR context;

  if (auto props = parse_properties(node)) {
    context.properties = std::move(*props);
  }
  else {
    return propagate_unexpected(props);
  }

  if (auto comps = parse_components(node, map)) {
    context.components = std::move(*comps);
  }
  else {
    return propagate_unexpected(comps);
  }

  return context;
}

}  // namespace tactile
