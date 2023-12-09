// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/meta/attribute_type.hpp"

namespace tactile {

auto parse_attribute_type(const StringView name) -> Maybe<AttributeType>
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
  else {
    return kNone;
  }
}

}  // namespace tactile