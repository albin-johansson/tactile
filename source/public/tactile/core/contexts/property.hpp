/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <filesystem>  // path
#include <string>      // string
#include <utility>     // move
#include <variant>     // variant, holds_alternative, get_if

#include "tactile/core/common/prelude.hpp"
#include "tactile/core/contexts/color.hpp"
#include "tactile/core/contexts/property_type.hpp"
#include "tactile/core/core.hpp"
#include "tactile/core/math/vector.hpp"

namespace tactile {

/** Represents a value of one of several possible types. */
class TACTILE_CORE_API Property final {
  // These are indices into the value type variant types
  inline constexpr static usize kStringTypeIndex = 0;
  inline constexpr static usize kIntTypeIndex = 1;
  inline constexpr static usize kInt2TypeIndex = 2;
  inline constexpr static usize kInt3TypeIndex = 3;
  inline constexpr static usize kInt4TypeIndex = 4;
  inline constexpr static usize kFloatTypeIndex = 5;
  inline constexpr static usize kFloat2TypeIndex = 6;
  inline constexpr static usize kFloat3TypeIndex = 7;
  inline constexpr static usize kFloat4TypeIndex = 8;
  inline constexpr static usize kBoolTypeIndex = 9;
  inline constexpr static usize kColorTypeIndex = 10;
  inline constexpr static usize kPathTypeIndex = 11;
  inline constexpr static usize kObjRefTypeIndex = 12;

 public:
  using string_type = std::string;
  using int_type = int32;
  using int2_type = Int2;
  using int3_type = Int3;
  using int4_type = Int4;
  using float_type = float32;
  using float2_type = Float2;
  using float3_type = Float3;
  using float4_type = Float4;
  using color_type = UColor;
  using path_type = std::filesystem::path;
  using objref_type = ObjectRef;

  // Remember to update the type indices if the order of the type arguments change here.
  using value_type = std::variant<string_type,
                                  int_type,
                                  int2_type,
                                  int3_type,
                                  int4_type,
                                  float_type,
                                  float2_type,
                                  float3_type,
                                  float4_type,
                                  bool,
                                  color_type,
                                  path_type,
                                  objref_type>;

  /** Creates an empty string property. */
  Property() = default;

  /** Creates a property with the specified type. */
  explicit Property(PropertyType type);

  Property(const char* value) { mValue.emplace<string_type>(value); }

  template <typename T>
  Property(T value)
  {
    mValue.emplace<T>(std::move(value));
  }

  void set(const char* value) { set(string_type {value}); }

  template <typename T>
  void set(T value)
  {
    mValue.emplace<T>(std::move(value));
  }

  /** Resets the property to the initial value associated with the specified type. */
  void reset(PropertyType type);

  [[nodiscard]] auto as_string() const -> const string_type&;

  [[nodiscard]] auto as_int() const -> int_type;

  [[nodiscard]] auto as_int2() const -> const int2_type&;

  [[nodiscard]] auto as_int3() const -> const int3_type&;

  [[nodiscard]] auto as_int4() const -> const int4_type&;

  [[nodiscard]] auto as_float() const -> float_type;

  [[nodiscard]] auto as_float2() const -> const float2_type&;

  [[nodiscard]] auto as_float3() const -> const float3_type&;

  [[nodiscard]] auto as_float4() const -> const float4_type&;

  [[nodiscard]] auto as_bool() const -> bool;

  [[nodiscard]] auto as_color() const -> const color_type&;

  [[nodiscard]] auto as_path() const -> const path_type&;

  [[nodiscard]] auto as_object() const -> objref_type;

  [[nodiscard]] auto get_type() const -> PropertyType;

  [[nodiscard]] auto is_vector() const -> bool;

  [[nodiscard]] auto has_initial_value() const -> bool;

  [[nodiscard]] auto operator==(const Property& other) const -> bool = default;

 private:
  value_type mValue;

  template <typename T>
  [[nodiscard]] auto _holds() const noexcept -> bool
  {
    return std::holds_alternative<T>(mValue);
  }

  template <typename T>
  [[nodiscard]] auto _get_if() const noexcept -> const T*
  {
    return std::get_if<T>(&mValue);
  }
};

}  // namespace tactile
