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

#include <utility>  // move

#include "io/ir/map/map_ir.hpp"
#include "io/map/parse/json/json_parser.hpp"
#include "tactile/base/container/string.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto parse_value(const JSON& json, StringView type, Attribute& value)
    -> ParseError
{
  const auto attr_type = parse_attr_type(type);
  if (!attr_type) {
    return ParseError::UnsupportedPropertyType;
  }

  switch (*attr_type) {
    case AttributeType::kStr: {
      value = as_string(json, "value").value();
      break;
    }
    case AttributeType::kInt: {
      value = as_int(json, "value").value();
      break;
    }
    case AttributeType::kFloat: {
      value = as_float(json, "value").value();
      break;
    }
    case AttributeType::kBool: {
      value = as_bool(json, "value").value();
      break;
    }
    case AttributeType::kPath: {
      Path path = as_string(json, "value").value();
      value = std::move(path);
      break;
    }
    case AttributeType::kColor: {
      const auto hex = as_string(json, "value").value();

      // Empty color properties are not supported, so just assume the default color value
      if (hex.empty()) {
        value.reset_to_default(AttributeType::kColor);
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
    case AttributeType::kObject: {
      value = ObjectRef {as_int(json, "value").value()};
      break;
    }
    case AttributeType::kFloat2:
    case AttributeType::kFloat3:
    case AttributeType::kFloat4:
    case AttributeType::kInt2:
    case AttributeType::kInt3:
      [[fallthrough]];
    case AttributeType::kInt4:
      return ParseError::UnsupportedPropertyType;
  }

  return ParseError::None;
}

[[nodiscard]] auto parse_property(const JSON& json, ContextIR& context_data) -> ParseError
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

auto parse_properties(const JSON& json, ContextIR& context_data) -> ParseError
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

}  // namespace tactile
