// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/meta/attribute_type.hpp"

#include "tactile/core/debug/exception.hpp"

namespace tactile::core {

auto parse_attribute_type(std::string_view name) -> std::expected<AttributeType, ErrorCode>
{
  if (name == "string") {
    return AttributeType::kStr;
  }
  else if (name == "int") {
    return AttributeType::kInt;
  }
  else if (name == "int2") {
    return AttributeType::kInt2;
  }
  else if (name == "int3") {
    return AttributeType::kInt3;
  }
  else if (name == "int4") {
    return AttributeType::kInt4;
  }
  else if (name == "float") {
    return AttributeType::kFloat;
  }
  else if (name == "float2") {
    return AttributeType::kFloat2;
  }
  else if (name == "float3") {
    return AttributeType::kFloat3;
  }
  else if (name == "float4") {
    return AttributeType::kFloat4;
  }
  else if (name == "bool") {
    return AttributeType::kBool;
  }
  else if (name == "file" || name == "path") {
    return AttributeType::kPath;
  }
  else if (name == "color") {
    return AttributeType::kColor;
  }
  else if (name == "object") {
    return AttributeType::kObject;
  }

  return std::unexpected {ErrorCode::kBadParam};
}

auto serialize(const AttributeType type) -> std::string_view
{
  switch (type) {
    case AttributeType::kStr:    return "string";
    case AttributeType::kInt:    return "int";
    case AttributeType::kInt2:   return "int2";
    case AttributeType::kInt3:   return "int3";
    case AttributeType::kInt4:   return "int4";
    case AttributeType::kFloat:  return "float";
    case AttributeType::kFloat2: return "float2";
    case AttributeType::kFloat3: return "float3";
    case AttributeType::kFloat4: return "float4";
    case AttributeType::kBool:   return "bool";
    case AttributeType::kPath:   return "path";
    case AttributeType::kColor:  return "color";
    case AttributeType::kObject: return "object";
    default:                     throw Exception {"bad attribute type"};
  }
}

auto operator<<(std::ostream& stream, const AttributeType type) -> std::ostream&
{
  return stream << serialize(type);
}

}  // namespace tactile::core
