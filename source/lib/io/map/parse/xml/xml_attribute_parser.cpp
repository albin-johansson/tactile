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

#include "io/ir/map/map_ir.hpp"
#include "io/map/parse/xml/xml_parser.hpp"
#include "io/save_formats.hpp"
#include "io/xml_utils.hpp"
#include "tactile/core/common/assert.hpp"
#include "tactile/core/containers/string.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto _parse_value(XmlNode node, const char* type) -> Parsed<Property>
{
  TACTILE_ASSERT(type);

  const auto attr_type = parse_attr_type(type);
  if (!attr_type) {
    return unexpected(ParseError::UnsupportedPropertyType);
  }

  Property value;
  switch (*attr_type) {
    case PropertyType::Str: {
      value = get_string_attr(node, "value").value();
      break;
    }
    case PropertyType::Int: {
      value = get_int_attr(node, "value").value();
      break;
    }
    case PropertyType::Float: {
      value = get_float_attr(node, "value").value();
      break;
    }
    case PropertyType::Bool: {
      value = get_bool_attr(node, "value").value();
      break;
    }
    case PropertyType::Path: {
      Path path = get_string_attr(node, "value").value();
      value = std::move(path);
      break;
    }
    case PropertyType::Color: {
      const auto hex = get_string_attr(node, "value").value();

      // Empty color properties are not supported, so just assume the default color value
      if (hex.empty()) {
        value.reset(PropertyType::Color);
      }
      else {
        if (const auto color =
                (hex.size() == 9) ? to_color_argb(hex) : to_color_rgb(hex)) {
          value = *color;
        }
        else {
          return unexpected(ParseError::CorruptPropertyValue);
        }
      }

      break;
    }
    case PropertyType::Object: {
      value = ObjectRef {get_int_attr(node, "value").value()};
      break;
    }
    case PropertyType::Float2:
    case PropertyType::Float3:
    case PropertyType::Float4:
    case PropertyType::Int2:
    case PropertyType::Int3:
      [[fallthrough]];
    case PropertyType::Int4:
      return unexpected(ParseError::UnsupportedPropertyType);
  }

  return value;
}

[[nodiscard]] auto _parse_property(XmlNode node) -> Parsed<Property>
{
  // String properties may exclude the type attribute
  const char* type = node.attribute("type").as_string("string");

  if (auto value = _parse_value(node, type)) {
    return std::move(*value);
  }
  else {
    return propagate_unexpected(value);
  }
}

}  // namespace

auto parse_properties(XmlNode node) -> Parsed<AttributeMap>
{
  AttributeMap props;

  for (const auto property_node: node.child("properties").children("property")) {
    String property_name;
    if (auto name = get_string_attr(property_node, "name")) {
      property_name = std::move(*name);
    }
    else {
      return unexpected(ParseError::NoPropertyName);
    }

    if (auto value = _parse_property(property_node)) {
      props[std::move(property_name)] = std::move(*value);
    }
    else {
      return propagate_unexpected(value);
    }
  }

  return props;
}

auto parse_context(XmlNode node) -> Parsed<ContextIR>
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