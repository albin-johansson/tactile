// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj_format/tmj_format_attribute_parser.hpp"

#include <utility>  // move

#include "tactile/base/io/color_parser.hpp"
#include "tactile/base/numeric/saturate_cast.hpp"
#include "tactile/runtime/logging.hpp"

namespace tactile {
namespace tmj_format_attribute_parser {

[[nodiscard]]
auto parse_type(const StringView type) -> Expected<AttributeType, SaveFormatParseError>
{
  if (type == "string") {
    return AttributeType::kStr;
  }

  if (type == "int") {
    return AttributeType::kInt;
  }

  if (type == "float") {
    return AttributeType::kFloat;
  }

  if (type == "bool") {
    return AttributeType::kBool;
  }

  if (type == "file") {
    return AttributeType::kPath;
  }

  if (type == "color") {
    return AttributeType::kColor;
  }

  if (type == "object") {
    return AttributeType::kObject;
  }

  log(LogLevel::kError, "Invalid TMJ property type: {}", type);
  return unexpected(SaveFormatParseError::kBadPropertyType);
}

[[nodiscard]]
auto parse_value(const nlohmann::json& value_json,
                 const AttributeType type) -> Expected<Attribute, SaveFormatParseError>
{
  switch (type) {
    case AttributeType::kStr: {
      if (const auto* value = value_json.get_ptr<const nlohmann::json::string_t*>()) {
        return Attribute {Attribute::string_type {*value}};
      }

      break;
    }
    case AttributeType::kInt: {
      if (const auto* value = value_json.get_ptr<const nlohmann::json::number_integer_t*>()) {
        return Attribute {saturate_cast<Attribute::int_type>(*value)};
      }

      break;
    }
    case AttributeType::kFloat: {
      if (const auto* value = value_json.get_ptr<const nlohmann::json::number_float_t*>()) {
        return Attribute {static_cast<Attribute::float_type>(*value)};
      }

      break;
    }
    case AttributeType::kBool: {
      if (const auto* value = value_json.get_ptr<const bool*>()) {
        return Attribute {*value};
      }

      break;
    }
    case AttributeType::kPath: {
      if (const auto* value = value_json.get_ptr<const nlohmann::json::string_t*>()) {
        return Attribute {Attribute::path_type {*value}};
      }

      break;
    }
    case AttributeType::kColor: {
      if (const auto* value = value_json.get_ptr<const nlohmann::json::string_t*>()) {
        Attribute attribute {AttributeType::kColor};

        const auto length = value->size();
        if (length != 0 && length != 7 && length != 9) {
          log(LogLevel::kError, "Invalid TMJ property color length: {}", length);
          return unexpected(SaveFormatParseError::kBadPropertyValue);
        }

        if (!value->empty()) {
          const auto color =
              (length == 9) ? parse_color_argb(*value) : parse_color_rgb(*value);
          if (color.has_value()) {
            attribute = Attribute {*color};
          }
          else {
            log(LogLevel::kError, "Invalid color property value: {}", *value);
            return unexpected(SaveFormatParseError::kBadPropertyValue);
          }
        }

        return attribute;
      }

      break;
    }
    case AttributeType::kObject: {
      if (const auto* value = value_json.get_ptr<const nlohmann::json::number_integer_t*>()) {
        return Attribute {
          Attribute::objref_type {saturate_cast<Attribute::objref_type::value_type>(*value)}};
      }

      break;
    }
    default: return unexpected(SaveFormatParseError::kBadPropertyType);
  }

  return unexpected(SaveFormatParseError::kBadPropertyValue);
}

}  // namespace tmj_format_attribute_parser

auto parse_tiled_tmj_property(const nlohmann::json& property_json)
    -> Expected<ir::NamedAttribute, SaveFormatParseError>
{
  ir::NamedAttribute named_attribute {};

  const auto name_iter = property_json.find("name");
  if (name_iter == property_json.end()) {
    return unexpected(SaveFormatParseError::kNoPropertyName);
  }

  const auto type_iter = property_json.find("type");
  if (type_iter == property_json.end()) {
    return unexpected(SaveFormatParseError::kNoPropertyType);
  }

  const auto value_iter = property_json.find("value");
  if (value_iter == property_json.end()) {
    return unexpected(SaveFormatParseError::kNoPropertyValue);
  }

  name_iter->get_to(named_attribute.name);

  const auto property_type = tmj_format_attribute_parser::parse_type(*type_iter);
  if (!property_type.has_value()) {
    return propagate_unexpected(property_type);
  }

  auto property_value = tmj_format_attribute_parser::parse_value(*value_iter, *property_type);
  if (!property_value.has_value()) {
    return propagate_unexpected(property_value);
  }

  named_attribute.value = std::move(*property_value);

  return named_attribute;
}

auto parse_tiled_tmj_metadata(const nlohmann::json& root_json)
    -> Expected<ir::Metadata, SaveFormatParseError>
{
  ir::Metadata metadata {};

  if (const auto name_iter = root_json.find("name"); name_iter != root_json.end()) {
    name_iter->get_to(metadata.name);
  }

  if (const auto properties_iter = root_json.find("properties");
      properties_iter != root_json.end() && properties_iter->is_array()) {
    metadata.properties.reserve(properties_iter->size());

    for (const auto& [_, property_json] : properties_iter->items()) {
      if (auto property = parse_tiled_tmj_property(property_json)) {
        metadata.properties.push_back(std::move(*property));
      }
      else {
        return propagate_unexpected(property);
      }
    }
  }

  return metadata;
}

}  // namespace tactile
