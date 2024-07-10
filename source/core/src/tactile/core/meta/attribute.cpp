// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/meta/attribute.hpp"

#include <type_traits>  // decay_t
#include <variant>      // visit

#include "tactile/core/debug/assert.hpp"
#include "tactile/core/numeric/vec_stream.hpp"

namespace tactile {

Attribute::Attribute(const AttributeType type)
{
  reset(type);
}

Attribute::Attribute(const char* str)
{
  TACTILE_ASSERT(str);
  mValue.emplace<string_type>(str);
}

void Attribute::reset(const AttributeType type)
{
  switch (type) {
    case AttributeType::kStr:    set(string_type {}); return;
    case AttributeType::kInt:    set(int_type {}); return;
    case AttributeType::kInt2:   set(int2_type {}); return;
    case AttributeType::kInt3:   set(int3_type {}); return;
    case AttributeType::kInt4:   set(int4_type {}); return;
    case AttributeType::kFloat:  set(float_type {}); return;
    case AttributeType::kFloat2: set(float2_type {}); return;
    case AttributeType::kFloat3: set(float3_type {}); return;
    case AttributeType::kFloat4: set(float4_type {}); return;
    case AttributeType::kBool:   set(bool {}); return;
    case AttributeType::kPath:   set(path_type {}); return;
    case AttributeType::kColor:  set(color_type {}); return;
    case AttributeType::kObject: set(objref_type {}); return;
  }

  throw Exception {"bad attribute type"};
}

void Attribute::set(const char* str)
{
  TACTILE_ASSERT(str);
  mValue.emplace<string_type>(str);
}

auto Attribute::as_string() const -> const string_type&
{
  return _get<string_type>();
}

auto Attribute::as_int() const -> int_type
{
  return _get<int_type>();
}

auto Attribute::as_int2() const -> const int2_type&
{
  return _get<int2_type>();
}

auto Attribute::as_int3() const -> const int3_type&
{
  return _get<int3_type>();
}

auto Attribute::as_int4() const -> const int4_type&
{
  return _get<int4_type>();
}

auto Attribute::as_float() const -> float_type
{
  return _get<float_type>();
}

auto Attribute::as_float2() const -> const float2_type&
{
  return _get<float2_type>();
}

auto Attribute::as_float3() const -> const float3_type&
{
  return _get<float3_type>();
}

auto Attribute::as_float4() const -> const float4_type&
{
  return _get<float4_type>();
}

auto Attribute::as_bool() const -> bool
{
  return _get<bool>();
}

auto Attribute::as_color() const -> const color_type&
{
  return _get<color_type>();
}

auto Attribute::as_path() const -> const path_type&
{
  return _get<path_type>();
}

auto Attribute::as_object() const -> objref_type
{
  return _get<objref_type>();
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
    case kIntTypeIndex:    return AttributeType::kInt;
    case kInt2TypeIndex:   return AttributeType::kInt2;
    case kInt3TypeIndex:   return AttributeType::kInt3;
    case kInt4TypeIndex:   return AttributeType::kInt4;
    case kFloatTypeIndex:  return AttributeType::kFloat;
    case kFloat2TypeIndex: return AttributeType::kFloat2;
    case kFloat3TypeIndex: return AttributeType::kFloat3;
    case kFloat4TypeIndex: return AttributeType::kFloat4;
    case kBoolTypeIndex:   return AttributeType::kBool;
    case kColorTypeIndex:  return AttributeType::kColor;
    case kPathTypeIndex:   return AttributeType::kPath;
    case kObjRefTypeIndex: return AttributeType::kObject;
  }

  throw Exception {"bad attribute type"};
}

}  // namespace tactile
