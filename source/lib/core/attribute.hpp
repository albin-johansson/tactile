/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include <concepts>  // same_as
#include <utility>   // move

#include "common/numeric.hpp"
#include "common/type/math.hpp"
#include "common/type/maybe.hpp"
#include "common/type/ostream.hpp"
#include "common/type/path.hpp"
#include "common/type/string.hpp"
#include "common/type/variant.hpp"
#include "core/color.hpp"

namespace tactile {

/// Represents the different possible attributes types.
enum class AttributeType {
  String,  ///< Arbitrary string.
  Int,     ///< Plain int.
  Int2,    ///< 2D int vector.
  Int3,    ///< 3D int vector.
  Int4,    ///< 4D int vector.
  Float,   ///< Plain float.
  Float2,  ///< 2D float vector.
  Float3,  ///< 3D float vector.
  Float4,  ///< 4D float vector.
  Bool,    ///< Boolean value.
  Path,    ///< File path.
  Color,   ///< Color value.
  Object,  ///< Integer object ID, references some map object.
};

/// Returns the name of an attribute type for use in save files.
[[nodiscard]] auto stringify(AttributeType type) -> const char*;

/// Parses an attribute type from a type name used in save files.
[[nodiscard]] auto parse_attr_type(StringView name) -> Maybe<AttributeType>;

/// Outputs the result of calling `stringify` with the type to a stream.
auto operator<<(OStream& stream, AttributeType type) -> OStream&;

/// Strong type that represents object references.
enum object_t : int32 {
};

template <typename T>
concept AnAttributeType = std::same_as<T, String> ||  //
                          std::same_as<T, int32> ||   //
                          std::same_as<T, Int2> ||    //
                          std::same_as<T, Int3> ||    //
                          std::same_as<T, Int4> ||    //
                          std::same_as<T, float> ||   //
                          std::same_as<T, Float2> ||  //
                          std::same_as<T, Float3> ||  //
                          std::same_as<T, Float4> ||  //
                          std::same_as<T, bool> ||    //
                          std::same_as<T, Color> ||   //
                          std::same_as<T, Path> ||    //
                          std::same_as<T, object_t>;

/// Represents an "attribute" value, used by both property and component facilities.
class Attribute final {
 public:
  using string_type = String;
  using integer_type = int32;
  using int2_type = Int2;
  using int3_type = Int3;
  using int4_type = Int4;
  using float_type = float;
  using float2_type = Float2;
  using float3_type = Float3;
  using float4_type = Float4;
  using color_type = Color;
  using path_type = Path;

  using value_type = Variant<string_type,
                             integer_type,
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
                             object_t>;

  /// Creates an empty string attribute.
  Attribute() = default;

  explicit Attribute(const AttributeType type) { reset_to_default(type); }

  /// Creates an attribute with an initial value.
  template <AnAttributeType T>
  /* implicit */ Attribute(T value)
  {
    mValue.emplace<T>(std::move(value));
  }

  /// Updates the stored value of the attribute.
  template <AnAttributeType T>
  void set_value(T value)
  {
    mValue.emplace<T>(std::move(value));
  }

  /// Resets the attribute to use the default value of the specified type.
  void reset_to_default(AttributeType type);

  /// Indicates whether the stored value is the default one.
  /// Note, black is used as the default color.
  [[nodiscard]] auto has_default_value() const -> bool;

  /// Returns the type of the attribute's stored value.
  [[nodiscard]] auto type() const noexcept -> AttributeType;

  /// Indicates whether the attribute holds a string value.
  [[nodiscard]] auto is_string() const noexcept -> bool;

  /// Indicates whether the attribute holds an integer value.
  [[nodiscard]] auto is_int() const noexcept -> bool;

  /// Indicates whether the attribute holds a 2D int vector.
  [[nodiscard]] auto is_int2() const noexcept -> bool;

  /// Indicates whether the attribute holds a 3D int vector.
  [[nodiscard]] auto is_int3() const noexcept -> bool;

  /// Indicates whether the attribute holds a 4D int vector.
  [[nodiscard]] auto is_int4() const noexcept -> bool;

  /// Indicates whether the attribute holds a float value.
  [[nodiscard]] auto is_float() const noexcept -> bool;

  /// Indicates whether the attribute holds a 2D float vector.
  [[nodiscard]] auto is_float2() const noexcept -> bool;

  /// Indicates whether the attribute holds a 3D float vector.
  [[nodiscard]] auto is_float3() const noexcept -> bool;

  /// Indicates whether the attribute holds a 4D float vector.
  [[nodiscard]] auto is_float4() const noexcept -> bool;

  /// Indicates whether the attribute holds a boolean value.
  [[nodiscard]] auto is_bool() const noexcept -> bool;

  /// Indicates whether the attribute holds a file path value.
  [[nodiscard]] auto is_path() const noexcept -> bool;

  /// Indicates whether the attribute holds an object reference value.
  [[nodiscard]] auto is_object() const noexcept -> bool;

  /// Indicates whether the attribute holds a color value.
  [[nodiscard]] auto is_color() const noexcept -> bool;

  /// Attempts to return the attribute value as a string.
  [[nodiscard]] auto try_as_string() const noexcept -> const string_type*
  {
    return get_if<string_type>();
  }

  /// Attempts to return the attribute value as an integer.
  [[nodiscard]] auto try_as_int() const noexcept -> const integer_type*
  {
    return get_if<integer_type>();
  }

  /// Attempts to return the attribute value as a 2D int vector.
  [[nodiscard]] auto try_as_int2() const noexcept -> const int2_type*
  {
    return get_if<int2_type>();
  }

  /// Attempts to return the attribute value as a 3D int vector.
  [[nodiscard]] auto try_as_int3() const noexcept -> const int3_type*
  {
    return get_if<int3_type>();
  }

  /// Attempts to return the attribute value as a 4D int vector.
  [[nodiscard]] auto try_as_int4() const noexcept -> const int4_type*
  {
    return get_if<int4_type>();
  }

  /// Attempts to return the attribute value as a float.
  [[nodiscard]] auto try_as_float() const noexcept -> const float_type*
  {
    return get_if<float_type>();
  }

  /// Attempts to return the attribute value as a 2D float vector.
  [[nodiscard]] auto try_as_float2() const noexcept -> const float2_type*
  {
    return get_if<float2_type>();
  }

  /// Attempts to return the attribute value as a 3D float vector.
  [[nodiscard]] auto try_as_float3() const noexcept -> const float3_type*
  {
    return get_if<float3_type>();
  }

  /// Attempts to return the attribute value as a 4D float vector.
  [[nodiscard]] auto try_as_float4() const noexcept -> const float4_type*
  {
    return get_if<float4_type>();
  }

  /// Attempts to return the attribute value as a boolean.
  [[nodiscard]] auto try_as_bool() const noexcept -> const bool*
  {
    return get_if<bool>();
  }

  /// Attempts to return the attribute value as a file path.
  [[nodiscard]] auto try_as_path() const noexcept -> const path_type*
  {
    return get_if<path_type>();
  }

  /// Attempts to return the attribute value as an object reference.
  [[nodiscard]] auto try_as_object() const noexcept -> const object_t*
  {
    return get_if<object_t>();
  }

  /// Attempts to return the attribute value as a color.
  [[nodiscard]] auto try_as_color() const noexcept -> const color_type*
  {
    return get_if<color_type>();
  }

  /// Returns the attribute's string value.
  [[nodiscard]] auto as_string() const -> const string_type&;

  /// Returns the attribute's integer value.
  [[nodiscard]] auto as_int() const -> integer_type;

  /// Returns the attribute's 2D int vector value.
  [[nodiscard]] auto as_int2() const -> int2_type;

  /// Returns the attribute's 3D int vector value.
  [[nodiscard]] auto as_int3() const -> int3_type;

  /// Returns the attribute's 4D int vector value.
  [[nodiscard]] auto as_int4() const -> int4_type;

  /// Returns the attribute's float value.
  [[nodiscard]] auto as_float() const -> float_type;

  /// Returns the attribute's 2D float vector value.
  [[nodiscard]] auto as_float2() const -> float2_type;

  /// Returns the attribute's 3D float vector value.
  [[nodiscard]] auto as_float3() const -> float3_type;

  /// Returns the attribute's 4D float vector value.
  [[nodiscard]] auto as_float4() const -> float4_type;

  /// Returns the attribute's boolean value.
  [[nodiscard]] auto as_bool() const -> bool;

  /// Returns the attribute's file value.
  [[nodiscard]] auto as_path() const -> const path_type&;

  /// Returns the attribute's object reference value.
  [[nodiscard]] auto as_object() const -> object_t;

  /// Returns the attribute's color value.
  [[nodiscard]] auto as_color() const -> const color_type&;

  [[nodiscard]] auto operator==(const Attribute&) const -> bool = default;

 private:
  value_type mValue;

  template <AnAttributeType T>
  [[nodiscard]] auto holds() const noexcept -> bool
  {
    return std::holds_alternative<T>(mValue);
  }

  template <AnAttributeType T>
  [[nodiscard]] auto get_if() const noexcept -> const T*
  {
    return std::get_if<T>(&mValue);
  }
};

/// Outputs an attribute to a stream for debugging purposes.
auto operator<<(OStream& stream, const Attribute& value) -> OStream&;

}  // namespace tactile
