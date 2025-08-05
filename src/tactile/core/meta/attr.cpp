// Copyright (C) 2025 Albin Johansson

module tactile.core.meta;

import tactile.core.ext.std;

namespace tactile {

Attr::Attr(const AttrKind kind)
{
  reset(kind);
}

void Attr::reset(const AttrKind kind)
{
  switch (kind) {
    case AttrKind::kInt:
      emplace<int_type>();
      break;
    case AttrKind::kInt2:
      emplace<int2_type>();
      break;
    case AttrKind::kInt3:
      emplace<int3_type>();
      break;
    case AttrKind::kInt4:
      emplace<int4_type>();
      break;
    case AttrKind::kFloat:
      emplace<float_type>();
      break;
    case AttrKind::kFloat2:
      emplace<float2_type>();
      break;
    case AttrKind::kFloat3:
      emplace<float3_type>();
      break;
    case AttrKind::kFloat4:
      emplace<float4_type>();
      break;
    case AttrKind::kBool:
      emplace<bool>();
      break;
    case AttrKind::kString:
      emplace<string_type>();
      break;
    case AttrKind::kPath:
      emplace<path_type>();
      break;
    case AttrKind::kColor:
      emplace<color_type>();
      break;
    default:
      throw std::invalid_argument {"bad attribute kind"};
  }
}

auto Attr::as_int() -> int_type*
{
  return std::get_if<int_type>(&m_value);
}

auto Attr::as_int() const -> const int_type*
{
  return std::get_if<int_type>(&m_value);
}

auto Attr::as_int2() -> int2_type*
{
  return std::get_if<int2_type>(&m_value);
}

auto Attr::as_int2() const -> const int2_type*
{
  return std::get_if<int2_type>(&m_value);
}

auto Attr::as_int3() -> int3_type*
{
  return std::get_if<int3_type>(&m_value);
}

auto Attr::as_int3() const -> const int3_type*
{
  return std::get_if<int3_type>(&m_value);
}

auto Attr::as_int4() -> int4_type*
{
  return std::get_if<int4_type>(&m_value);
}

auto Attr::as_int4() const -> const int4_type*
{
  return std::get_if<int4_type>(&m_value);
}

auto Attr::as_float() -> float_type*
{
  return std::get_if<float_type>(&m_value);
}

auto Attr::as_float() const -> const float_type*
{
  return std::get_if<float_type>(&m_value);
}

auto Attr::as_float2() -> float2_type*
{
  return std::get_if<float2_type>(&m_value);
}

auto Attr::as_float2() const -> const float2_type*
{
  return std::get_if<float2_type>(&m_value);
}

auto Attr::as_float3() -> float3_type*
{
  return std::get_if<float3_type>(&m_value);
}

auto Attr::as_float3() const -> const float3_type*
{
  return std::get_if<float3_type>(&m_value);
}

auto Attr::as_float4() -> float4_type*
{
  return std::get_if<float4_type>(&m_value);
}

auto Attr::as_float4() const -> const float4_type*
{
  return std::get_if<float4_type>(&m_value);
}

auto Attr::as_bool() -> bool*
{
  return std::get_if<bool>(&m_value);
}

auto Attr::as_bool() const -> const bool*
{
  return std::get_if<bool>(&m_value);
}

auto Attr::as_string() -> string_type*
{
  return std::get_if<string_type>(&m_value);
}

auto Attr::as_string() const -> const string_type*
{
  return std::get_if<string_type>(&m_value);
}

auto Attr::as_path() -> path_type*
{
  return std::get_if<path_type>(&m_value);
}

auto Attr::as_path() const -> const path_type*
{
  return std::get_if<path_type>(&m_value);
}

auto Attr::as_color() -> color_type*
{
  return std::get_if<color_type>(&m_value);
}

auto Attr::as_color() const -> const color_type*
{
  return std::get_if<color_type>(&m_value);
}

auto Attr::kind() const -> AttrKind
{
  switch (m_value.index()) {
    case kIntTypeIndex:
      return AttrKind::kInt;

    case kInt2TypeIndex:
      return AttrKind::kInt2;

    case kInt3TypeIndex:
      return AttrKind::kInt3;

    case kInt4TypeIndex:
      return AttrKind::kInt4;

    case kFloatTypeIndex:
      return AttrKind::kFloat;

    case kFloat2TypeIndex:
      return AttrKind::kFloat2;

    case kFloat3TypeIndex:
      return AttrKind::kFloat3;

    case kFloat4TypeIndex:
      return AttrKind::kFloat4;

    case kBoolTypeIndex:
      return AttrKind::kBool;

    case kStringTypeIndex:
      return AttrKind::kString;

    case kPathTypeIndex:
      return AttrKind::kPath;

    case kColorTypeIndex:
      return AttrKind::kColor;
  }

  throw std::logic_error {"bad attribute value index"};
}

auto Attr::is_vector() const -> bool
{
  switch (kind()) {
    case AttrKind::kInt:
    case AttrKind::kFloat:
    case AttrKind::kBool:
    case AttrKind::kString:
    case AttrKind::kPath:
      [[fallthrough]];
    case AttrKind::kColor:
      return false;
    case AttrKind::kInt2:
    case AttrKind::kInt3:
    case AttrKind::kInt4:
    case AttrKind::kFloat2:
    case AttrKind::kFloat3:
      [[fallthrough]];
    case AttrKind::kFloat4:
      return true;
  }

  throw std::logic_error {"bad attribute kind"};
}

}  // namespace tactile
