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

#include <utility>  // move

#include "common/type/string.hpp"
#include "io/map/ir/ir.hpp"
#include "io/map/parse/json/json_parser.hpp"

namespace tactile::io {
namespace {

[[nodiscard]] auto parse_value(const JSON& json, StringView type, Attribute& value)
    -> ParseError
{
  const auto attr_type = parse_attr_type(type);
  if (!attr_type) {
    return ParseError::UnsupportedPropertyType;
  }

  switch (*attr_type) {
    case AttributeType::String: {
      value = as_string(json, "value").value();
      break;
    }
    case AttributeType::Int: {
      value = as_int(json, "value").value();
      break;
    }
    case AttributeType::Float: {
      value = as_float(json, "value").value();
      break;
    }
    case AttributeType::Bool: {
      value = as_bool(json, "value").value();
      break;
    }
    case AttributeType::Path: {
      Path path = as_string(json, "value").value();
      value = std::move(path);
      break;
    }
    case AttributeType::Color: {
      const auto hex = as_string(json, "value").value();

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
          return ParseError::CorruptPropertyValue;
        }
      }

      break;
    }
    case AttributeType::Object: {
      value = object_t {as_int(json, "value").value()};
      break;
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto parse_property(const JSON& json, ir::ContextData& context_data)
    -> ParseError
{
  String property_name;

  if (auto name = as_string(json, "name")) {
    property_name = std::move(*name);
  }
  else {
    return ParseError::NoPropertyName;
  }

  auto& value = context_data.properties[std::move(property_name)];

  if (auto type = as_string(json, "type")) {
    if (const auto err = parse_value(json, *type, value); err != ParseError::None) {
      return err;
    }
  }
  else {
    return ParseError::NoPropertyType;
  }

  return ParseError::None;
}

}  // namespace

auto parse_properties(const JSON& json, ir::ContextData& context_data) -> ParseError
{
  if (const auto it = json.find("properties"); it != json.end()) {
    for (const auto& [_, value]: it->items()) {
      if (const auto err = parse_property(value, context_data); err != ParseError::None) {
        return err;
      }
    }
  }

  return ParseError::None;
}

}  // namespace tactile::io