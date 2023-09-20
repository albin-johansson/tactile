// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/context/attribute.hpp"

#include <type_traits>  // decay_t
#include <utility>      // to_underlying
#include <variant>      // visit

#include "tactile/core/debug/error.hpp"

namespace tactile {

void Attribute::reset(const AttributeType type)
{
  switch (type) {
    case AttributeType::Str:
      set(string_type {});
      break;

    case AttributeType::Int:
      set(int_type {});
      break;

    case AttributeType::Int2:
      set(int2_type {});
      break;

    case AttributeType::Int3:
      set(int3_type {});
      break;

    case AttributeType::Int4:
      set(int4_type {});
      break;

    case AttributeType::Float:
      set(float_type {});
      break;

    case AttributeType::Float2:
      set(float2_type {});
      break;

    case AttributeType::Float3:
      set(float3_type {});
      break;

    case AttributeType::Float4:
      set(float4_type {});
      break;

    case AttributeType::Bool:
      set(bool {});
      break;

    case AttributeType::Path:
      set(path_type {});
      break;

    case AttributeType::Color:
      set(color_type {});
      break;

    case AttributeType::Object:
      set(objref_type {});
      break;

    default:
      throw Error {"Invalid attribute type"};
  }
}

auto Attribute::is_vector() const -> bool
{
  switch (get_type()) {
    case AttributeType::Int2:
    case AttributeType::Int3:
    case AttributeType::Int4:
    case AttributeType::Float2:
    case AttributeType::Float3:
    case AttributeType::Float4:
      return true;

    case AttributeType::Str:
    case AttributeType::Int:
    case AttributeType::Float:
    case AttributeType::Bool:
    case AttributeType::Path:
    case AttributeType::Color:
    case AttributeType::Object:
    default:
      return false;
  }
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
    case kStringTypeIndex:
      return AttributeType::Str;

    case kIntTypeIndex:
      return AttributeType::Int;

    case kInt2TypeIndex:
      return AttributeType::Int2;

    case kInt3TypeIndex:
      return AttributeType::Int3;

    case kInt4TypeIndex:
      return AttributeType::Int4;

    case kFloatTypeIndex:
      return AttributeType::Float;

    case kFloat2TypeIndex:
      return AttributeType::Float2;

    case kFloat3TypeIndex:
      return AttributeType::Float3;

    case kFloat4TypeIndex:
      return AttributeType::Float4;

    case kBoolTypeIndex:
      return AttributeType::Bool;

    case kColorTypeIndex:
      return AttributeType::Color;

    case kPathTypeIndex:
      return AttributeType::Path;

    case kObjRefTypeIndex:
      return AttributeType::Object;

    default:
      throw Error {"Invalid property type"};
  }
}

auto operator<<(std::ostream& stream, const Attribute& attribute) -> std::ostream&
{
  switch (attribute.get_type()) {
    case AttributeType::Str:
      return stream << attribute.as_string();

    case AttributeType::Int:
      return stream << attribute.as_int();

    case AttributeType::Int2:
      return stream << attribute.as_int2();

    case AttributeType::Int3:
      return stream << attribute.as_int3();

    case AttributeType::Int4:
      return stream << attribute.as_int4();

    case AttributeType::Float:
      return stream << attribute.as_float();

    case AttributeType::Float2:
      return stream << attribute.as_float2();

    case AttributeType::Float3:
      return stream << attribute.as_float3();

    case AttributeType::Float4:
      return stream << attribute.as_float4();

    case AttributeType::Bool:
      return stream << attribute.as_bool();

    case AttributeType::Path:
      return stream << attribute.as_path();

    case AttributeType::Color:
      return stream << attribute.as_color();

    case AttributeType::Object:
      return stream << std::to_underlying(attribute.as_object());
  }

  return stream;
}

}  // namespace tactile
