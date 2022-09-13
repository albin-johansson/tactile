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
#include <string>   // string
#include <utility>  // move

#include <centurion/color.hpp>

#include "core/common/fs.hpp"
#include "io/map/ir/ir.hpp"
#include "io/map/parse/xml/xml_parser.hpp"
#include "io/util/xml.hpp"
#include "misc/assert.hpp"

namespace tactile::io {
namespace {

[[nodiscard]] auto parse_value(XMLNode node, const char* type, Attribute& value)
    -> ParseError
{
  TACTILE_ASSERT(type);

  const auto attr_type = parse_attr_type(type);
  if (!attr_type) {
    return ParseError::UnsupportedPropertyType;
  }

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
      fs::path path = as_string(node, "value").value();
      value = std::move(path);
      break;
    }
    case AttributeType::Color: {
      const auto hex = as_string(node, "value").value();

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
          return ParseError::CorruptPropertyValue;
        }
      }

      break;
    }
    case AttributeType::Object: {
      value = object_t {as_int(node, "value").value()};
      break;
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto parse_property(XMLNode node, ir::ContextData& context) -> ParseError
{
  std::string property_name;

  if (auto name = as_string(node, "name")) {
    property_name = std::move(*name);
  }
  else {
    return ParseError::NoPropertyName;
  }

  auto& value = context.properties[std::move(property_name)];

  // String properties may exclude the type attribute
  const char* type = node.attribute("type").as_string("string");

  if (const auto err = parse_value(node, type, value); err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

}  // namespace

auto parse_properties(XMLNode node, ir::ContextData& context) -> ParseError
{
  for (const auto property_node : node.child("properties").children("property")) {
    if (const auto err = parse_property(property_node, context);
        err != ParseError::None) {
      return err;
    }
  }

  return ParseError::None;
}

}  // namespace tactile::io