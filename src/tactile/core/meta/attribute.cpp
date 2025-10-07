// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/core/meta/attribute.hpp"

#include <stdexcept>

namespace tactile {

void Attribute::reset(const AttributeKind kind)
{
  switch (kind) {
    case AttributeKind::kInt:    (void) emplace<int_type>(); break;
    case AttributeKind::kFloat:  (void) emplace<float_type>(); break;
    case AttributeKind::kBool:   (void) emplace<bool>(); break;
    case AttributeKind::kString: (void) emplace<string_type>(); break;
    case AttributeKind::kPath:   (void) emplace<path_type>(); break;
    case AttributeKind::kColor:  (void) emplace<color_type>(); break;
    default:                     throw std::invalid_argument {"[Attribute::reset]: bad attribute kind"};
  }
}

auto Attribute::as_int() -> int_type*
{
  return get_if<int_type>(&m_value);
}

auto Attribute::as_int() const -> const int_type*
{
  return get_if<int_type>(&m_value);
}

auto Attribute::as_float() -> float_type*
{
  return get_if<float_type>(&m_value);
}

auto Attribute::as_float() const -> const float_type*
{
  return get_if<float_type>(&m_value);
}

auto Attribute::as_bool() -> bool*
{
  return get_if<bool>(&m_value);
}

auto Attribute::as_bool() const -> const bool*
{
  return get_if<bool>(&m_value);
}

auto Attribute::as_string() -> string_type*
{
  return get_if<string_type>(&m_value);
}

auto Attribute::as_string() const -> const string_type*
{
  return get_if<string_type>(&m_value);
}

auto Attribute::as_path() -> path_type*
{
  return get_if<path_type>(&m_value);
}

auto Attribute::as_path() const -> const path_type*
{
  return get_if<path_type>(&m_value);
}

auto Attribute::as_color() -> color_type*
{
  return get_if<color_type>(&m_value);
}

auto Attribute::as_color() const -> const color_type*
{
  return get_if<color_type>(&m_value);
}

auto Attribute::has_default_value() const -> bool
{
  const auto visitor = []<typename T>(const T& value) -> bool { return value == T {}; };
  return std::visit(visitor, m_value);
}

auto Attribute::get_kind() const -> AttributeKind
{
  switch (m_value.index()) {
    case kIntValueIndex:    return AttributeKind::kInt;
    case kFloatValueIndex:  return AttributeKind::kFloat;
    case kBoolValueIndex:   return AttributeKind::kBool;
    case kStringValueIndex: return AttributeKind::kString;
    case kPathValueIndex:   return AttributeKind::kPath;
    case kColorValueIndex:  return AttributeKind::kColor;
    default:                throw std::runtime_error {"[Attribute::get_kind]: bad variant"};
  }
}

}  // namespace tactile
