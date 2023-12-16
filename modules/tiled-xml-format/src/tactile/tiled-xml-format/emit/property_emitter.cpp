// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled-xml-format/emit/property_emitter.hpp"

#include "tactile/foundation/debug/exception.hpp"
#include "tactile/foundation/io/filesystem.hpp"
#include "tactile/foundation/io/save/save_format_error.hpp"
#include "tactile/foundation/io/save/vector_serialization.hpp"

namespace tactile::tiled::tmx {

auto to_tmx_name(const AttributeType type) -> const char*
{
  switch (type) {
    case AttributeType::kInt2:
    case AttributeType::kInt3:
    case AttributeType::kInt4:
    case AttributeType::kFloat2:
    case AttributeType::kFloat3:
    case AttributeType::kFloat4: [[fallthrough]];
    case AttributeType::kStr: return "string";
    case AttributeType::kInt: return "int";
    case AttributeType::kFloat: return "float";
    case AttributeType::kBool: return "bool";
    case AttributeType::kPath: return "file";
    case AttributeType::kColor: return "color";
    case AttributeType::kObject: return "object";
  }

  throw Exception {"Invalid attribute type"};
}

void append_property_node(pugi::xml_node properties_node,
                          const ir::NamedAttribute& property)
{
  const auto attribute_type = property.value.get_type();

  auto property_node = properties_node.append_child("property");
  property_node.append_attribute("name").set_value(property.name.c_str());

  // Properties with no type attribute are assumed to be string properties.
  if (attribute_type != AttributeType::kStr) {
    property_node.append_attribute("type").set_value(to_tmx_name(attribute_type));
  }

  auto value_attr = property_node.append_attribute("value");

  switch (attribute_type) {
    case AttributeType::kStr: {
      value_attr.set_value(property.value.as_string().c_str());
      break;
    }
    case AttributeType::kInt: {
      value_attr.set_value(property.value.as_int());
      break;
    }
    case AttributeType::kInt2: {
      const auto vec_str = serialize(property.value.as_int2());
      value_attr.set_value(vec_str.c_str());
      break;
    }
    case AttributeType::kInt3: {
      const auto vec_str = serialize(property.value.as_int3());
      value_attr.set_value(vec_str.c_str());
      break;
    }
    case AttributeType::kInt4: {
      const auto vec_str = serialize(property.value.as_int4());
      value_attr.set_value(vec_str.c_str());
      break;
    }
    case AttributeType::kFloat: {
      value_attr.set_value(property.value.as_float());
      break;
    }
    case AttributeType::kFloat2: {
      const auto vec_str = serialize(property.value.as_float2());
      value_attr.set_value(vec_str.c_str());
      break;
    }
    case AttributeType::kFloat3: {
      const auto vec_str = serialize(property.value.as_float3());
      value_attr.set_value(vec_str.c_str());
      break;
    }
    case AttributeType::kFloat4: {
      const auto vec_str = serialize(property.value.as_float4());
      value_attr.set_value(vec_str.c_str());
      break;
    }
    case AttributeType::kBool: {
      value_attr.set_value(property.value.as_bool());
      break;
    }
    case AttributeType::kPath: {
      const auto path_str = normalize_path(property.value.as_path());
      value_attr.set_value(path_str.c_str());
      break;
    }
    case AttributeType::kColor: {
      const auto argb_str = to_string_argb(property.value.as_color());
      value_attr.set_value(argb_str.c_str());
      break;
    }
    case AttributeType::kObject: {
      value_attr.set_value(property.value.as_object().value);
      break;
    }
  }
}

void append_properties_node(pugi::xml_node context_node,
                            const Vector<ir::NamedAttribute>& properties)
{
  if (!properties.empty()) {
    auto properties_node = context_node.append_child("properties");

    for (const auto& property : properties) {
      append_property_node(properties_node, property);
    }
  }
}

}  // namespace tactile::tiled::tmx
