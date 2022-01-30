#include "yaml_attributes.hpp"

#include <filesystem>   // path
#include <string>       // string
#include <string_view>  // string_view
#include <utility>      // move

#include <centurion.hpp>

namespace tactile::parsing {
namespace {

[[nodiscard]] auto _parse_attribute_type(const std::string_view type)
    -> Maybe<attribute_type>
{
  if (type == "string") {
    return attribute_type::string;
  }
  else if (type == "int") {
    return attribute_type::integer;
  }
  else if (type == "float") {
    return attribute_type::floating;
  }
  else if (type == "bool") {
    return attribute_type::boolean;
  }
  else if (type == "color") {
    return attribute_type::color;
  }
  else if (type == "object") {
    return attribute_type::object;
  }
  else if (type == "file") {
    return attribute_type::file;
  }
  else {
    return nothing;
  }
}

[[nodiscard]] auto _parse_attribute_value(const YAML::Node& value,
                                          const attribute_type type)
    -> Maybe<attribute_value>
{
  switch (type) {
    case attribute_type::string:
      return value.as<std::string>();

    case attribute_type::integer:
      return value.as<int32>();

    case attribute_type::floating:
      return value.as<float>();

    case attribute_type::boolean:
      return value.as<bool>();

    case attribute_type::file: {
      const std::filesystem::path file = value.as<std::string>();
      return file;
    }
    case attribute_type::color: {
      const auto hex = value.as<std::string>();
      if (const auto color = cen::color::from_rgba(hex)) {
        return *color;
      }
      else {
        return std::nullopt;
      }
    }
    case attribute_type::object:
      return object_t{value.as<int32>()};

    default:
      return nothing;
  }
}

[[nodiscard]] auto _parse_component_definition_attribute(const YAML::Node& node,
                                                         component_map& def)
    -> parse_error
{
  std::string name;
  if (auto attributeName = node["name"]) {
    name = attributeName.as<std::string>();
  }
  else {
    return parse_error::no_component_def_attribute_name;
  }

  attribute_type type{};
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
      case attribute_type::string:
        value = defaultValue.as<std::string>();
        break;

      case attribute_type::integer:
        value = defaultValue.as<int32>();
        break;

      case attribute_type::floating:
        value = defaultValue.as<float>();
        break;

      case attribute_type::boolean:
        value = defaultValue.as<bool>();
        break;

      case attribute_type::file: {
        std::filesystem::path path = defaultValue.as<std::string>();
        value = std::move(path);
        break;
      }
      case attribute_type::color: {
        if (auto color = cen::color::from_rgba(defaultValue.as<std::string>())) {
          value = *color;
        }
        else {
          return parse_error::unsupported_component_def_attribute_value;
        }
        break;
      }
      case attribute_type::object:
        value = object_t{defaultValue.as<int32>()};
        break;
    }
  }

  return parse_error::none;
}

[[nodiscard]] auto _parse_component_definition(const YAML::Node& node, map_data& data)
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
                                    const map_data& map,
                                    attribute_context_data& data) -> parse_error
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
        attributes[attr] = _parse_attribute_value(value, attrType).value();
      }
      else {
        return parse_error::no_component_attribute_value;
      }
    }
  }

  return parse_error::none;
}

}  // namespace

auto parse_component_definitions(const YAML::Node& node, map_data& data) -> parse_error
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
                      const map_data& map,
                      attribute_context_data& data) -> parse_error
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

auto parse_properties(const YAML::Node& node, attribute_context_data& data) -> parse_error
{
  if (auto sequence = node["properties"]) {
    for (const auto& propertyNode : sequence) {
      std::string propertyName;
      attribute_type propertyType{};

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
          return parse_error::could_not_parse_property;
        }
      }
      else {
        return parse_error::could_not_parse_property;  // TODO no_property_value
      }
    }
  }

  return parse_error::none;
}

}  // namespace tactile::parsing
