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
#include <ostream>   // ostream
#include <string>    // string
#include <utility>   // move
#include <variant>   // variant, get, get_if, holds_alternative

#include <centurion/color.hpp>

#include "core/common/filesystem.hpp"
#include "core/common/ints.hpp"

namespace tactile {

/// Represents the different possible attributes types.
enum class AttributeType
{
  String,  /// A string property.
  Int,     /// An integer property.
  Float,   /// A floating-point property.
  Bool,    /// A boolean property.
  Path,    /// A file path property.
  Color,   /// A color property.
  Object   /// An integer ID property, that refers to a map object.
};

/**
 * Returns the name of an attribute type for use in save files.
 *
 * \param type the type to query.
 *
 * \return a type name.
 *
 * \throws TactileError if the type is invalid.
 */
[[nodiscard]] auto stringify(AttributeType type) -> const char*;

/**
 * Outputs an attribute type using an output stream.
 *
 * This function simply forwards the result from `stringify(AttributeType)` to a
 * stream.
 *
 * \param stream the output stream.
 * \param type the attribute type to output.
 *
 * \return the used stream.
 */
auto operator<<(std::ostream& stream, AttributeType type) -> std::ostream&;

/// Strong type that represents object references.
enum object_t : int32
{};

template <typename T>
concept CAttributeType = std::same_as<T, std::string> ||  //
                         std::same_as<T, int32> ||        //
                         std::same_as<T, float> ||        //
                         std::same_as<T, bool> ||         //
                         std::same_as<T, cen::color> ||   //
                         std::same_as<T, fs::path> ||     //
                         std::same_as<T, object_t>;

/// Represents an "attribute" value, used by both property and component facilities.
class Attribute final
{
 public:
  using string_type = std::string;
  using integer_type = int32;
  using float_type = float;
  using color_type = cen::color;
  using path_type = fs::path;

  using value_type = std::variant<string_type,
                                  integer_type,
                                  float_type,
                                  bool,
                                  color_type,
                                  path_type,
                                  object_t>;

  /// Creates an empty string attribute.
  Attribute() = default;

  explicit Attribute(const AttributeType type)
  {
    reset_to_default(type);
  }

  /**
   * Creates a property.
   *
   * \tparam T the type of the property value.
   *
   * \param value the attribute value.
   */
  template <CAttributeType T>
  /*implicit*/ Attribute(T value)
  {
    mValue.emplace<T>(std::move(value));
  }

  /**
   * Updates the attribute value.
   *
   * \tparam T the value type.
   *
   * \param value the new attribute value.
   */
  template <CAttributeType T>
  void set_value(T value)
  {
    mValue.emplace<T>(std::move(value));
  }

  /**
   * Resets the value of the attribute to the the default of a specific type.
   *
   * \param type the new value type.
   */
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

  /// Indicates whether the attribute holds a float value.
  [[nodiscard]] auto is_float() const noexcept -> bool;

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

  /// Attempts to return the attribute value as a float.
  [[nodiscard]] auto try_as_float() const noexcept -> const float_type*
  {
    return get_if<float_type>();
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

  /// Returns the attribute's float value.
  [[nodiscard]] auto as_float() const -> float_type;

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

  template <CAttributeType T>
  [[nodiscard]] auto holds() const noexcept -> bool
  {
    return std::holds_alternative<T>(mValue);
  }

  template <CAttributeType T>
  [[nodiscard]] auto get_if() const noexcept -> const T*
  {
    return std::get_if<T>(&mValue);
  }
};

auto operator<<(std::ostream& stream, const Attribute& value) -> std::ostream&;

}  // namespace tactile
