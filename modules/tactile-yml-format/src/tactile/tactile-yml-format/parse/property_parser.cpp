// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tactile-yml-format/parse/property_parser.hpp"

#include <utility>  // move

#include "tactile/foundation/debug/generic_error.hpp"
#include "tactile/foundation/io/save/vector_deserialization.hpp"
#include "tactile/tactile-yml-format/parse/common.hpp"

namespace tactile::yml_format {

auto parse_property_type(StringView type_name) -> Result<AttributeType>
{
  if (const auto parsed_type = parse_attribute_type(type_name)) {
    return *parsed_type;
  }

  return unexpected(make_save_format_error(SaveFormatError::kUnsupportedPropertyType));
}

auto parse_property_value(const YAML::Node& node, const AttributeType attr_type)
    -> Result<Attribute>
{
  switch (attr_type) {
    case AttributeType::kStr: return Attribute {node.as<String>()};
    case AttributeType::kInt: return Attribute {node.as<int>()};
    case AttributeType::kFloat: return Attribute {node.as<float>()};
    case AttributeType::kBool: return Attribute {node.as<bool>()};
    case AttributeType::kPath:
      return Attribute {Attribute::path_type {node.as<String>()}};
    case AttributeType::kObject:
      return Attribute {Attribute::objref_type {node.as<int>()}};

    case AttributeType::kInt2: {
      if (const auto vec = deserialize_int2(node.as<String>())) {
        return Attribute {*vec};
      }

      return unexpected(make_save_format_error(SaveFormatError::kBadPropertyValue));
    }

    case AttributeType::kInt3: {
      if (const auto vec = deserialize_int3(node.as<String>())) {
        return Attribute {*vec};
      }

      return unexpected(make_save_format_error(SaveFormatError::kBadPropertyValue));
    }

    case AttributeType::kInt4: {
      if (const auto vec = deserialize_int4(node.as<String>())) {
        return Attribute {*vec};
      }

      return unexpected(make_save_format_error(SaveFormatError::kBadPropertyValue));
    }

    case AttributeType::kFloat2: {
      if (const auto vec = deserialize_float2(node.as<String>())) {
        return Attribute {*vec};
      }

      return unexpected(make_save_format_error(SaveFormatError::kBadPropertyValue));
    }

    case AttributeType::kFloat3: {
      if (const auto vec = deserialize_float3(node.as<String>())) {
        return Attribute {*vec};
      }

      return unexpected(make_save_format_error(SaveFormatError::kBadPropertyValue));
    }

    case AttributeType::kFloat4: {
      if (const auto vec = deserialize_float4(node.as<String>())) {
        return Attribute {*vec};
      }

      return unexpected(make_save_format_error(SaveFormatError::kBadPropertyValue));
    }

    case AttributeType::kColor: {
      const auto color_code = node.as<String>();
      if (const auto color = to_color_rgba(color_code)) {
        return Attribute {*color};
      }

      return unexpected(make_save_format_error(SaveFormatError::kBadPropertyValue));
    }
  }

  return unexpected(make_save_format_error(SaveFormatError::kUnsupportedPropertyType));
}

auto parse_property(const YAML::Node& node) -> Result<ir::NamedAttribute>
{
  ir::NamedAttribute named_attribute {};

  return parse_to(node, "name", named_attribute.name)
      .and_then([&] { return parse<String>(node, "type"); })
      .and_then([&](const String& type_name) { return parse_property_type(type_name); })
      .and_then([&](const AttributeType value_type) -> Result<Attribute> {
        if (const auto value_node = node["value"]) {
          return parse_property_value(value_node, value_type);
        }

        // TODO
        //  if (!options.strict_mode) {
        //    return Attribute {value_type};
        //  }

        return unexpected(make_save_format_error(SaveFormatError::kPropertyWithoutValue));
      })
      .transform([&](Attribute&& value) {
        named_attribute.value = std::move(value);
        return std::move(named_attribute);
      });
}

auto parse_properties(const YAML::Node& node) -> Result<Vector<ir::NamedAttribute>>
{
  Vector<ir::NamedAttribute> properties;

  const auto property_seq = node["properties"];
  if (!property_seq) {
    return properties;
  }

  properties.reserve(property_seq.size());

  for (const auto& property_node : property_seq) {
    if (auto property = parse_property(property_node)) {
      properties.push_back(std::move(*property));
    }
    else {
      return propagate_unexpected(property);
    }
  }

  return properties;
}

}  // namespace tactile::yml_format
