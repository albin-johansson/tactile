// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled-xml-format/parse/property_parser.hpp"

#include <iterator>  // distance
#include <utility>   // move

#include "tactile/foundation/io/save/save_format_error.hpp"
#include "tactile/tiled-xml-format/common.hpp"

namespace tactile::tiled::tmx {
namespace {

auto parse_property_type(const StringView type_name) -> Result<AttributeType>
{
  if (const auto parsed_type = parse_attribute_type(type_name)) {
    return *parsed_type;
  }

  return unexpected(make_save_format_error(SaveFormatError::kUnsupportedPropertyType));
}

auto parse_property_value(pugi::xml_node property_node, const AttributeType type)
    -> Result<Attribute>
{
  switch (type) {
    case AttributeType::kStr: {
      if (auto value = parse_string(property_node, "value")) {
        return Attribute {std::move(*value)};
      }
      break;
    }
    case AttributeType::kInt: {
      if (const auto value = parse_int(property_node, "value")) {
        return Attribute {*value};
      }
      break;
    }
    case AttributeType::kFloat: {
      if (const auto value = parse_float(property_node, "value")) {
        return Attribute {*value};
      }
      break;
    }
    case AttributeType::kBool: {
      if (const auto value = parse_bool(property_node, "value")) {
        return Attribute {*value};
      }
      break;
    }
    case AttributeType::kPath: {
      if (const auto value = parse_string(property_node, "value")) {
        return Attribute {Attribute::path_type {*value}};
      }
      break;
    }
    case AttributeType::kColor: {
      if (const auto hex_code = parse_string(property_node, "value")) {
        // Empty color properties are not supported, so just assume the default color value.
        if (hex_code->empty()) {
          return Attribute {AttributeType::kColor};
        }

        const auto color =
            (hex_code->size() == 9) ? to_color_argb(*hex_code) : to_color_rgb(*hex_code);

        if (color.has_value()) {
          return Attribute {*color};
        }
      }

      break;
    }
    case AttributeType::kObject: {
      if (const auto value = parse_int(property_node, "value")) {
        return Attribute {Attribute::objref_type {*value}};
      }
      break;
    }

    case AttributeType::kInt2:
    case AttributeType::kInt3:
    case AttributeType::kInt4:
    case AttributeType::kFloat2:
    case AttributeType::kFloat3:
    case AttributeType::kFloat4:
      return unexpected(
          make_save_format_error(SaveFormatError::kUnsupportedPropertyType));
  }

  return unexpected(make_save_format_error(SaveFormatError::kBadPropertyValue));
}

}  // namespace

auto parse_property_node(pugi::xml_node property_node) -> Result<ir::NamedAttribute>
{
  ir::NamedAttribute property {};

  return parse_to(property_node, "name", property.name)
      .and_then([&] {
        // String properties may omit the type attribute.
        const auto* type_name = property_node.attribute("type").as_string("string");
        return parse_property_type(type_name);
      })
      .and_then([&](const AttributeType type) {
        return parse_property_value(property_node, type);
      })
      .and_then([&](Attribute&& value) {
        property.value = std::move(value);
        return kOK;
      })
      .transform([&] { return std::move(property); });
}

auto parse_properties_node(pugi::xml_node context_node)
    -> Result<Vector<ir::NamedAttribute>>
{
  Vector<ir::NamedAttribute> properties {};

  const auto property_range = context_node.child("properties").children("property");
  if (property_range.empty()) {
    return properties;
  }

  properties.reserve(std::distance(property_range.begin(), property_range.end()));

  for (const auto property_node : property_range) {
    if (auto property = parse_property_node(property_node)) {
      properties.push_back(std::move(*property));
    }
    else {
      return propagate_unexpected(property);
    }
  }

  return properties;
}

}  // namespace tactile::tiled::tmx
