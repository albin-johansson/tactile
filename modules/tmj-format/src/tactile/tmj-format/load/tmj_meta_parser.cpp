// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tmj-format/load/tmj_meta_parser.hpp"

#include <utility>  // move

#include "tactile/core/container/string.hpp"
#include "tactile/core/context/attribute_type.hpp"
#include "tactile/core/context/color.hpp"
#include "tactile/core/debug/log/logger.hpp"
#include "tactile/core/io/save/save_format_error.hpp"

namespace tactile::tmj {
namespace {

[[nodiscard]]
auto _parse_string_value(const JSON& json, Attribute& value) -> Result<void>
{
  Attribute::string_type str {};
  return parse(json, "value", str).and_then([&] -> Result<void> {
    value = std::move(str);
    return kSuccess;
  });
}

[[nodiscard]]
auto _parse_int_value(const JSON& json, Attribute& value) -> Result<void>
{
  Attribute::int_type integer {};
  return parse(json, "value", integer).and_then([&] -> Result<void> {
    value = integer;
    return kSuccess;
  });
}

[[nodiscard]]
auto _parse_float_value(const JSON& json, Attribute& value) -> Result<void>
{
  Attribute::float_type real {};
  return parse(json, "value", real).and_then([&] -> Result<void> {
    value = real;
    return kSuccess;
  });
}

[[nodiscard]]
auto _parse_bool_value(const JSON& json, Attribute& value) -> Result<void>
{
  bool boolean {};
  return parse(json, "value", boolean).and_then([&] -> Result<void> {
    value = boolean;
    return kSuccess;
  });
}

[[nodiscard]]
auto _parse_path_value(const JSON& json, Attribute& value) -> Result<void>
{
  Attribute::string_type path {};
  return parse(json, "value", path).and_then([&] -> Result<void> {
    value = Attribute::path_type {std::move(path)};
    return kSuccess;
  });
}

[[nodiscard]]
auto _parse_color_value(const JSON& json, Attribute& value) -> Result<void>
{
  String color_code {};
  return parse(json, "value", color_code).and_then([&] -> Result<void> {
    // Empty color properties are not supported, so just assume the default color value.
    if (color_code.empty()) {
      TACTILE_LOG_WARN(
          "[TMJ] Encountered empty color property, assuming default color value");
      value.reset(AttributeType::kColor);
      return kSuccess;
    }

    const auto color = (color_code.size() == 9) ? to_color_argb(color_code)  //
                                                : to_color_rgb(color_code);
    if (color.has_value()) {
      value = *color;
      return kSuccess;
    }

    TACTILE_LOG_ERROR("[TMJ] Invalid color property '{}'", color_code);
    return error(SaveFormatError::kBadColorProperty);
  });
}

[[nodiscard]]
auto _parse_object_ref_value(const JSON& json, Attribute& value) -> Result<void>
{
  int32 object_ref {};
  return parse(json, "value", object_ref).and_then([&] -> Result<void> {
    value = ObjectRef {object_ref};
    return kSuccess;
  });
}

[[nodiscard]]
auto _parse_property_value(const JSON& json, const AttributeType type, Attribute& value)
    -> Result<void>
{
  switch (type) {
    case AttributeType::kStr: return _parse_string_value(json, value);
    case AttributeType::kInt: return _parse_int_value(json, value);
    case AttributeType::kFloat: return _parse_float_value(json, value);
    case AttributeType::kBool: return _parse_bool_value(json, value);
    case AttributeType::kPath: return _parse_path_value(json, value);
    case AttributeType::kColor: return _parse_color_value(json, value);
    case AttributeType::kObject: return _parse_object_ref_value(json, value);
    case AttributeType::kFloat2:
    case AttributeType::kFloat3:
    case AttributeType::kFloat4:
    case AttributeType::kInt2:
    case AttributeType::kInt3:
    case AttributeType::kInt4: return kSuccess;
  }

  return kSuccess;
}

[[nodiscard]]
auto _parse_property_type(const JSON& json, AttributeType& out_type) -> Result<void>
{
  String type_name {};
  return parse(json, "type", type_name).and_then([&] -> Result<void> {
    if (const auto attribute_type = parse_attribute_type(type_name)) {
      out_type = *attribute_type;
      return kSuccess;
    }
    else {
      return error(SaveFormatError::kUnsupportedPropertyType);
    }
  });
}

}  // namespace

auto parse_property(const JSON& json) -> Result<ir::NamedAttribute>
{
  ir::NamedAttribute property {};
  AttributeType type {};

  return parse(json, "name", property.name)
      .and_then([&] { return _parse_property_type(json, type); })
      .and_then([&] { return _parse_property_value(json, type, property.value); })
      .transform([&] { return std::move(property); });
}

auto parse_metadata(const JSON& json, ir::Metadata& meta) -> Result<void>
{
  // TODO parse name here?

  const auto properties_iter = json.find("properties");

  if (properties_iter != json.end()) {
    Vector<ir::NamedAttribute> properties {};
    properties.reserve(properties_iter->size());

    for (const auto& [_, property_json] : properties_iter->items()) {
      if (auto property = parse_property(property_json)) {
        properties.push_back(std::move(*property));
      }
      else {
        return propagate_unexpected(property);
      }
    }

    meta.properties = std::move(properties);
  }

  return kSuccess;
}

}  // namespace tactile
