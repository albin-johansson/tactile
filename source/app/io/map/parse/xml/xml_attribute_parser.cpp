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

#include <cstring>  // strcmp
#include <utility>  // move

#include <centurion/color.hpp>

#include "core/type/string.hpp"
#include "core/util/str.hpp"
#include "io/map/ir/ir.hpp"
#include "io/map/parse/xml/xml_parser.hpp"
#include "io/util/xml.hpp"
#include "misc/assert.hpp"

namespace tactile::io {
namespace {

[[nodiscard]] auto parse_value(XMLNode node, const char* type)
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
      value = as_string(node, "value").value();
      break;
    }
    case AttributeType::Int: {
      value = as_int(node, "value").value();
      break;
    }
    case AttributeType::Float: {
      value = as_float(node, "value").value();
      break;
    }
    case AttributeType::Bool: {
      value = as_bool(node, "value").value();
      break;
    }
    case AttributeType::Path: {
      Path path = to_std(as_string(node, "value").value());
      value = std::move(path);
      break;
    }
    case AttributeType::Color: {
      const auto hex = to_std(as_string(node, "value").value());

      // Empty color properties are not supported, so just assume the default color value
      if (hex.empty()) {
        value.reset_to_default(AttributeType::Color);
      }
      else {
        if (const auto color = (hex.size() == 9) ? cen::color::from_argb(hex)
                                                 : cen::color::from_rgb(hex)) {
          value = *color;
        }
        else {
          return error(ParseError::CorruptPropertyValue);
        }
      }

      break;
    }
    case AttributeType::Object: {
      value = object_t {as_int(node, "value").value()};
      break;
    }
  }

  return value;
}

[[nodiscard]] auto parse_property(XMLNode node) -> Expected<Attribute, ParseError>
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

auto parse_properties(XMLNode node) -> Expected<ir::AttributeMap, ParseError>
{
  ir::AttributeMap props;

  for (const auto property_node: node.child("properties").children("property")) {
    String property_name;
    if (auto name = as_string(property_node, "name")) {
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

auto parse_context(XMLNode node) -> Expected<ir::ContextData, ParseError>
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

}  // namespace tactile::io