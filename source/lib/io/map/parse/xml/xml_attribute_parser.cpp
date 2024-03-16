// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include <cstring>  // strcmp
#include <utility>  // move

#include "io/ir/map/map_ir.hpp"
#include "io/map/parse/xml/xml_parser.hpp"
#include "io/util/xml.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/core/debug/assert.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto parse_value(XmlNode node, const char* type)
    -> Expected<Attribute, ParseError>
{
  TACTILE_ASSERT(type);

  const auto attr_type = parse_attribute_type(type);
  if (!attr_type) {
    return unexpected(ParseError::UnsupportedPropertyType);
  }

  Attribute value;
  switch (*attr_type) {
    case AttributeType::kStr: {
      value = get_string_attr(node, "value").value();
      break;
    }
    case AttributeType::kInt: {
      value = get_int_attr(node, "value").value();
      break;
    }
    case AttributeType::kFloat: {
      value = get_float_attr(node, "value").value();
      break;
    }
    case AttributeType::kBool: {
      value = get_bool_attr(node, "value").value();
      break;
    }
    case AttributeType::kPath: {
      Path path = get_string_attr(node, "value").value();
      value = std::move(path);
      break;
    }
    case AttributeType::kColor: {
      const auto hex = get_string_attr(node, "value").value();

      // Empty color properties are not supported, so just assume the default color value
      if (hex.empty()) {
        value.reset(AttributeType::kColor);
      }
      else {
        const auto color = (hex.size() == 9) ? Color::parse_argb(hex)  //
                                             : Color::parse_rgb(hex);
        if (color) {
          value = *color;
        }
        else {
          return unexpected(ParseError::CorruptPropertyValue);
        }
      }

      break;
    }
    case AttributeType::kObject: {
      value = ObjectRef {get_int_attr(node, "value").value()};
      break;
    }
    case AttributeType::kFloat2:
    case AttributeType::kFloat3:
    case AttributeType::kFloat4:
    case AttributeType::kInt2:
    case AttributeType::kInt3: [[fallthrough]];
    case AttributeType::kInt4: return unexpected(ParseError::UnsupportedPropertyType);
  }

  return value;
}

[[nodiscard]] auto parse_property(XmlNode node) -> Expected<Attribute, ParseError>
{
  // String properties may exclude the type attribute
  const char* type = node.attribute("type").as_string("string");

  if (auto value = parse_value(node, type)) {
    return std::move(*value);
  }
  else {
    return propagate_unexpected(value);
  }
}

}  // namespace

auto parse_properties(XmlNode node) -> Expected<AttributeMap, ParseError>
{
  AttributeMap props;

  for (const auto property_node : node.child("properties").children("property")) {
    String property_name;
    if (auto name = get_string_attr(property_node, "name")) {
      property_name = std::move(*name);
    }
    else {
      return unexpected(ParseError::NoPropertyName);
    }

    if (auto value = parse_property(property_node)) {
      props[std::move(property_name)] = std::move(*value);
    }
    else {
      return propagate_unexpected(value);
    }
  }

  return props;
}

auto parse_context(XmlNode node) -> Expected<ContextIR, ParseError>
{
  ContextIR context;

  if (auto props = parse_properties(node)) {
    context.properties = std::move(*props);
  }
  else {
    return propagate_unexpected(props);
  }

  return context;
}

}  // namespace tactile
