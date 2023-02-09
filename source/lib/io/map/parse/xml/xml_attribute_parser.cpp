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

#include <cstring>  // strcmp
#include <utility>  // move

#include "common/debug/assert.hpp"
#include "common/type/string.hpp"
#include "io/ir/map_ir.hpp"
#include "io/map/parse/xml/xml_parser.hpp"
#include "io/util/xml.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto parse_value(XmlNode node, const char* type)
    -> Expected<Attribute, ParseError>
{
  TACTILE_ASSERT(type);

  const auto attr_type = parse_attr_type(type);
  if (!attr_type) {
    return error(ParseError::UnsupportedPropertyType);
  }

  Attribute value;
  switch (*attr_type) {
    case AttributeType::String: {
      value = get_string_attr(node, "value").value();
      break;
    }
    case AttributeType::Int: {
      value = get_int_attr(node, "value").value();
      break;
    }
    case AttributeType::Float: {
      value = get_float_attr(node, "value").value();
      break;
    }
    case AttributeType::Bool: {
      value = get_bool_attr(node, "value").value();
      break;
    }
    case AttributeType::Path: {
      Path path = get_string_attr(node, "value").value();
      value = std::move(path);
      break;
    }
    case AttributeType::Color: {
      const auto hex = get_string_attr(node, "value").value();

      // Empty color properties are not supported, so just assume the default color value
      if (hex.empty()) {
        value.reset_to_default(AttributeType::Color);
      }
      else {
        if (const auto color =
                (hex.size() == 9) ? Color::from_argb(hex) : Color::from_rgb(hex)) {
          value = *color;
        }
        else {
          return error(ParseError::CorruptPropertyValue);
        }
      }

      break;
    }
    case AttributeType::Object: {
      value = ObjectRef {get_int_attr(node, "value").value()};
      break;
    }
    case AttributeType::Float2:
    case AttributeType::Float3:
    case AttributeType::Float4:
    case AttributeType::Int2:
    case AttributeType::Int3:
      [[fallthrough]];
    case AttributeType::Int4:
      return error(ParseError::UnsupportedPropertyType);
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
    return pass_on_error(value);
  }
}

}  // namespace

auto parse_properties(XmlNode node) -> Expected<ir::AttributeMap, ParseError>
{
  ir::AttributeMap props;

  for (const auto property_node: node.child("properties").children("property")) {
    String property_name;
    if (auto name = get_string_attr(property_node, "name")) {
      property_name = std::move(*name);
    }
    else {
      return error(ParseError::NoPropertyName);
    }

    if (auto value = parse_property(property_node)) {
      props[std::move(property_name)] = std::move(*value);
    }
    else {
      return pass_on_error(value);
    }
  }

  return props;
}

auto parse_context(XmlNode node) -> Expected<ir::ContextData, ParseError>
{
  ir::ContextData context;

  if (auto props = parse_properties(node)) {
    context.properties = std::move(*props);
  }
  else {
    return pass_on_error(props);
  }

  return context;
}

}  // namespace tactile