// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tmj-format/emit/tmj_meta_emitter.hpp"

#include <utility>  // to_underlying

#include "tactile/core/debug/error.hpp"
#include "tactile/core/io/save/vector_serialization.hpp"

namespace tactile::tmj {

auto to_tmj_name(const AttributeType attribute_type) -> StringView
{
  switch (attribute_type) {
    case AttributeType::kInt2:
    case AttributeType::kInt3:
    case AttributeType::kInt4:
    case AttributeType::kFloat2:
    case AttributeType::kFloat3:
    case AttributeType::kFloat4:
    case AttributeType::kStr: return "string";
    case AttributeType::kInt: return "int";
    case AttributeType::kFloat: return "float";
    case AttributeType::kBool: return "bool";
    case AttributeType::kPath: return "file";
    case AttributeType::kColor: return "color";
    case AttributeType::kObject: return "object";
  }

  throw Error {"Invalid attribute type"};
}

auto to_tmj_json(const Attribute& attribute) -> JSON
{
  JSON json;
  json = 42;

  // Note, the use of parentheses when creating JSON values here is intentional.
  switch (attribute.get_type()) {
    case AttributeType::kStr: return JSON(attribute.as_string());
    case AttributeType::kInt: return JSON(attribute.as_int());
    case AttributeType::kInt2: return JSON(serialize(attribute.as_int2()));
    case AttributeType::kInt3: return JSON(serialize(attribute.as_int3()));
    case AttributeType::kInt4: return JSON(serialize(attribute.as_int4()));
    case AttributeType::kFloat: return JSON(attribute.as_float());
    case AttributeType::kFloat2: return JSON(serialize(attribute.as_float2()));
    case AttributeType::kFloat3: return JSON(serialize(attribute.as_float3()));
    case AttributeType::kFloat4: return JSON(serialize(attribute.as_float4()));
    case AttributeType::kBool: return JSON(attribute.as_bool());
    case AttributeType::kPath: return JSON(attribute.as_path().string());
    case AttributeType::kColor: return JSON(to_string_argb(attribute.as_color()));
    case AttributeType::kObject: return JSON(std::to_underlying(attribute.as_object()));
  }

  throw Error {"Invalid attribute type"};
}

auto emit_property_array(const ir::Metadata& meta) -> JSON
{
  auto property_json_array = JSON::array();

  for (const auto& [name, attribute] : meta.properties) {
    auto property_json = JSON::object();

    property_json["name"] = name;
    property_json["type"] = to_tmj_name(attribute.get_type());
    property_json["value"] = to_tmj_json(attribute);

    property_json_array += property_json;
  }

  return property_json_array;
}

}  // namespace tactile::tmj
