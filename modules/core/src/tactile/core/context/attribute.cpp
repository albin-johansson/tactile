// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/context/attribute.hpp"

#include <type_traits>  // decay_t
#include <variant>      // visit

#include "tactile/core/debug/error.hpp"

namespace tactile {

void Attribute::reset(const AttributeType type)
{
  switch (type) {
    case AttributeType::kStr: set(string_type {}); return;
    case AttributeType::kInt: set(int_type {}); return;
    case AttributeType::kInt2: set(int2_type {}); return;
    case AttributeType::kInt3: set(int3_type {}); return;
    case AttributeType::kInt4: set(int4_type {}); return;
    case AttributeType::kFloat: set(float_type {}); return;
    case AttributeType::kFloat2: set(float2_type {}); return;
    case AttributeType::kFloat3: set(float3_type {}); return;
    case AttributeType::kFloat4: set(float4_type {}); return;
    case AttributeType::kBool: set(bool {}); return;
    case AttributeType::kPath: set(path_type {}); return;
    case AttributeType::kColor: set(color_type {}); return;
    case AttributeType::kObject: set(objref_type {}); return;
  }

  throw Error {"Invalid attribute type"};
}

auto Attribute::is_vector() const -> bool
{
  switch (get_type()) {
    case AttributeType::kInt2:
    case AttributeType::kInt3:
    case AttributeType::kInt4:
    case AttributeType::kFloat2:
    case AttributeType::kFloat3:
    case AttributeType::kFloat4: return true;

    case AttributeType::kStr:
    case AttributeType::kInt:
    case AttributeType::kFloat:
    case AttributeType::kBool:
    case AttributeType::kPath:
    case AttributeType::kColor:
    case AttributeType::kObject: return false;
  }

  return false;
}

auto Attribute::has_default_value() const -> bool
{
  auto visitor = [](const auto& value) -> bool {
    using T = std::decay_t<decltype(value)>;
    return value == T {};
  };

  return std::visit(visitor, mValue);
}

auto Attribute::get_type() const -> AttributeType
{
  switch (mValue.index()) {
    case kStringTypeIndex: return AttributeType::kStr;
    case kIntTypeIndex: return AttributeType::kInt;
    case kInt2TypeIndex: return AttributeType::kInt2;
    case kInt3TypeIndex: return AttributeType::kInt3;
    case kInt4TypeIndex: return AttributeType::kInt4;
    case kFloatTypeIndex: return AttributeType::kFloat;
    case kFloat2TypeIndex: return AttributeType::kFloat2;
    case kFloat3TypeIndex: return AttributeType::kFloat3;
    case kFloat4TypeIndex: return AttributeType::kFloat4;
    case kBoolTypeIndex: return AttributeType::kBool;
    case kColorTypeIndex: return AttributeType::kColor;
    case kPathTypeIndex: return AttributeType::kPath;
    case kObjRefTypeIndex: return AttributeType::kObject;
  }

  throw Error {"Invalid property type"};
}

auto operator<<(std::ostream& stream, const Attribute& attribute) -> std::ostream&
{
  switch (attribute.get_type()) {
    case AttributeType::kStr: return stream << attribute.as_string();
    case AttributeType::kInt: return stream << attribute.as_int();
    case AttributeType::kInt2: return stream << attribute.as_int2();
    case AttributeType::kInt3: return stream << attribute.as_int3();
    case AttributeType::kInt4: return stream << attribute.as_int4();
    case AttributeType::kFloat: return stream << attribute.as_float();
    case AttributeType::kFloat2: return stream << attribute.as_float2();
    case AttributeType::kFloat3: return stream << attribute.as_float3();
    case AttributeType::kFloat4: return stream << attribute.as_float4();
    case AttributeType::kBool: return stream << attribute.as_bool();
    case AttributeType::kPath: return stream << attribute.as_path();
    case AttributeType::kColor: return stream << attribute.as_color();
    case AttributeType::kObject: return stream << attribute.as_object().value;
  }

  return stream;
}

}  // namespace tactile
