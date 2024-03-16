// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include <utility>  // move

#include "io/ir/map/map_ir.hpp"
#include "io/map/parse/json/json_parser.hpp"
#include "tactile/base/container/string.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto parse_value(const JSON& json, StringView type, Attribute& value)
    -> ParseError
{
  const auto attr_type = parse_attribute_type(type);
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
        value.reset(AttributeType::kColor);
      }
      else {
        if (const auto color =
                (hex.size() == 9) ? Color::parse_argb(hex) : Color::parse_rgb(hex)) {
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
    case AttributeType::kInt3: [[fallthrough]];
    case AttributeType::kInt4: return ParseError::UnsupportedPropertyType;
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
    for (const auto& [_, value] : it->items()) {
      if (const auto err = parse_property(value, context_data); err != ParseError::None) {
        return err;
      }
    }
  }

  return ParseError::None;
}

}  // namespace tactile
