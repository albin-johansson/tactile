// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include <concepts>
#include <utility>

#include "tactile/core/container/path.hpp"
#include "tactile/core/container/string.hpp"
#include "tactile/core/container/variant.hpp"
#include "tactile/core/meta/color.hpp"
#include "tactile/core/primitives.hpp"

namespace tactile {

/// Represents the supported attribute kinds.
enum class AttributeKind : uint8
{
  kInt,
  kFloat,
  kBool,
  kString,
  kPath,
  kColor,
};

/// Represents an attribute value.
class Attribute final
{
 public:
  using int_type = int64;
  using float_type = float64;
  using string_type = String;
  using path_type = Path;
  using color_type = Color;

  /// Resets the attribute value.
  void reset(AttributeKind kind);

  /// Returns a pointer to the integer value, or null if there is none.
  auto as_int() -> int_type*;
  auto as_int() const -> const int_type*;

  /// Returns a pointer to the floating-point value, or null if there is none.
  auto as_float() -> float_type*;
  auto as_float() const -> const float_type*;

  /// Returns a pointer to the boolean value, or null if there is none.
  auto as_bool() -> bool*;
  auto as_bool() const -> const bool*;

  /// Returns a pointer to the string value, or null if there is none.
  auto as_string() -> string_type*;
  auto as_string() const -> const string_type*;

  /// Returns a pointer to the path value, or null if there is none.
  auto as_path() -> path_type*;
  auto as_path() const -> const path_type*;

  /// Returns a pointer to the color value, or null if there is none.
  auto as_color() -> color_type*;
  auto as_color() const -> const color_type*;

  /// Indicates whether the attribute has the default value of the underlying type.
  auto has_default_value() const -> bool;

  /// Returns the kind of the attribute.
  auto get_kind() const -> AttributeKind;

  /// Updates the value of the attribute.
  template <typename T, typename... Args>
    requires std::constructible_from<T, Args...>
  auto emplace(Args&&... args) -> T&
  {
    return m_value.emplace<T>(std::forward<Args>(args)...);
  }

 private:
  constexpr static usize kIntValueIndex = 0;
  constexpr static usize kFloatValueIndex = 1;
  constexpr static usize kBoolValueIndex = 2;
  constexpr static usize kStringValueIndex = 3;
  constexpr static usize kPathValueIndex = 4;
  constexpr static usize kColorValueIndex = 5;

  using value_type =
      Variant<int_type, float_type, bool, string_type, path_type, color_type>;

  value_type m_value {};
};

}  // namespace tactile
