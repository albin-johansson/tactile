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

#include <concepts>    // same_as
#include <filesystem>  // path
#include <ostream>     // ostream
#include <string>      // string
#include <utility>     // move
#include <variant>     // variant, get, get_if, holds_alternative

#include <centurion/color.hpp>

#include "core/common/ints.hpp"

namespace tactile {

/// \addtogroup core
/// \{

/**
 * \brief Represents the different possible attributes types.
 */
enum class AttributeType
{
  String,  ///< A string property.
  Int,     ///< An integer property.
  Float,   ///< A floating-point property.
  Bool,    ///< A boolean property.
  Path,    ///< A file path property.
  Color,   ///< A color property.
  Object   ///< An integer ID property, that refers to a map object.
};

/**
 * \brief Returns the name of an attribute type for use in save files.
 *
 * \param type the type to query.
 *
 * \return the type name.
 *
 * \throws TactileError if the type is invalid.
 */
[[nodiscard]] auto stringify(AttributeType type) -> const char*;

/**
 * \brief Outputs an attribute type using an output stream.
 *
 * \details This function simply forwards the result from `stringify(AttributeType)` to a
 * stream.
 *
 * \param stream the output stream.
 * \param type the attribute type to output.
 *
 * \return the used stream.
 */
auto operator<<(std::ostream& stream, AttributeType type) -> std::ostream&;

/**
 * \brief Strong type that represents object references.
 */
enum object_t : int32
{};

template <typename T>
concept CAttributeType = std::same_as<T, std::string> ||            //
                         std::same_as<T, int32> ||                  //
                         std::same_as<T, float> ||                  //
                         std::same_as<T, bool> ||                   //
                         std::same_as<T, cen::color> ||             //
                         std::same_as<T, std::filesystem::path> ||  //
                         std::same_as<T, object_t>;

/**
 * \brief Represents an "attribute" value, used by both property and component facilities.
 */
class Attribute final
{
 public:
  using string_type = std::string;
  using integer_type = int32;
  using float_type = float;
  using color_type = cen::color;
  using path_type = std::filesystem::path;

  using value_type = std::variant<string_type,
                                  integer_type,
                                  float_type,
                                  bool,
                                  color_type,
                                  path_type,
                                  object_t>;

  /// \name Construction
  /// \{

  /**
   * \brief Creates an empty string attribute.
   */
  Attribute() = default;

  /**
   * \brief Creates a property.
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

  /// \} End of construction

  /// \name Value functions
  /// \{

  /**
   * \brief Updates the attribute value.
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
   * \brief Resets the value of the attribute to the the default of a specific type.
   *
   * \param type the new value type.
   */
  void reset_to_default(AttributeType type);

  /**
   * \brief Indicates whether the stored value is the default one.
   *
   * \details Black is the default color.
   *
   * \return `true` if the attribute stores a default value; `false` otherwise.
   */
  [[nodiscard]] auto has_default_value() const -> bool;

  /// \} End of value functions

  /// \name Type checks
  /// \{

  /**
   * \brief Returns the type of the attribute's stored value.
   *
   * \return the attribute type.
   */
  [[nodiscard]] auto type() const noexcept -> AttributeType;

  /**
   * \brief Indicates whether the attribute holds a string value.
   *
   * \return `true` if the attribute is a string; `false` otherwise.
   */
  [[nodiscard]] auto is_string() const noexcept -> bool;

  /**
   * \brief Indicates whether the attribute holds an integer value.
   *
   * \return `true` if the attribute is an integer; `false` otherwise.
   */
  [[nodiscard]] auto is_int() const noexcept -> bool;

  /**
   * \brief Indicates whether the attribute holds a float value.
   *
   * \return `true` if the attribute is a float; `false` otherwise.
   */
  [[nodiscard]] auto is_float() const noexcept -> bool;

  /**
   * \brief Indicates whether the attribute holds a boolean value.
   *
   * \return `true` if the attribute is a boolean; `false` otherwise.
   */
  [[nodiscard]] auto is_bool() const noexcept -> bool;

  /**
   * \brief Indicates whether the attribute holds a file path value.
   *
   * \return `true` if the attribute is a file; `false` otherwise.
   */
  [[nodiscard]] auto is_path() const noexcept -> bool;

  /**
   * \brief Indicates whether the attribute holds an object reference value.
   *
   * \return `true` if the attribute is an object reference; `false` otherwise.
   */
  [[nodiscard]] auto is_object() const noexcept -> bool;

  /**
   * \brief Indicates whether the attribute holds a color value.
   *
   * \return `true` if the attribute is a color; `false` otherwise.
   */
  [[nodiscard]] auto is_color() const noexcept -> bool;

  /// \} End of type checks

  /// \name Non-throwing value retrieval
  /// \{

  /**
   * \brief Attempts to return the attribute value as a string.
   *
   * \return a pointer to the stored value;
   *         a null pointer if the internal value is of another type.
   */
  [[nodiscard]] auto try_as_string() const noexcept -> const string_type*
  {
    return get_if<string_type>();
  }

  /**
   * \brief Attempts to return the attribute value as an integer.
   *
   * \return a pointer to the stored value;
   *         a null pointer if the internal value is of another type.
   */
  [[nodiscard]] auto try_as_int() const noexcept -> const integer_type*
  {
    return get_if<integer_type>();
  }

  /**
   * \brief Attempts to return the attribute value as a float.
   *
   * \return a pointer to the stored value;
   *         a null pointer if the internal value is of another type.
   */
  [[nodiscard]] auto try_as_float() const noexcept -> const float_type*
  {
    return get_if<float_type>();
  }

  /**
   * \brief Attempts to return the attribute value as a boolean.
   *
   * \return a pointer to the stored value;
   *         a null pointer if the internal value is of another type.
   */
  [[nodiscard]] auto try_as_bool() const noexcept -> const bool*
  {
    return get_if<bool>();
  }

  /**
   * \brief Attempts to return the attribute value as a file path.
   *
   * \return a pointer to the stored value;
   *         a null pointer if the internal value is of another type.
   */
  [[nodiscard]] auto try_as_path() const noexcept -> const path_type*
  {
    return get_if<path_type>();
  }

  /**
   * \brief Attempts to return the attribute value as an object reference.
   *
   * \return a pointer to the stored value;
   *         a null pointer if the internal value is of another type.
   */
  [[nodiscard]] auto try_as_object() const noexcept -> const object_t*
  {
    return get_if<object_t>();
  }

  /**
   * \brief Attempts to return the attribute value as a color.
   *
   * \return a pointer to the stored value;
   *         a null pointer if the internal value is of another type.
   */
  [[nodiscard]] auto try_as_color() const noexcept -> const color_type*
  {
    return get_if<color_type>();
  }

  /// \} End of non-throwing value retrieval

  /// \name Checked value retrieval
  /// \{

  /**
   * \brief Returns the attribute's string value.
   *
   * \return a reference to the stored value.
   *
   * \throws TactileError if the attribute isn't a string.
   */
  [[nodiscard]] auto as_string() const -> const string_type&;

  /**
   * \brief Returns the attribute's integer value.
   *
   * \return the stored value.
   *
   * \throws TactileError if the attribute isn't an integer.
   */
  [[nodiscard]] auto as_int() const -> integer_type;

  /**
   * \brief Returns the attribute's float value.
   *
   * \return the stored value.
   *
   * \throws TactileError if the attribute isn't a float.
   */
  [[nodiscard]] auto as_float() const -> float_type;

  /**
   * \brief Returns the attribute's boolean value.
   *
   * \return the stored value.
   *
   * \throws TactileError if the attribute isn't a boolean.
   */
  [[nodiscard]] auto as_bool() const -> bool;

  /**
   * \brief Returns the attribute's file value.
   *
   * \return a reference to the stored value.
   *
   * \throws TactileError if the attribute isn't a file.
   */
  [[nodiscard]] auto as_path() const -> const path_type&;

  /**
   * \brief Returns the attribute's object reference value.
   *
   * \return the stored value.
   *
   * \throws TactileError if the attribute isn't an object reference.
   */
  [[nodiscard]] auto as_object() const -> object_t;

  /**
   * \brief Returns the attribute's color value.
   *
   * \return the a reference to the stored value.
   *
   * \throws TactileError if the attribute isn't a color.
   */
  [[nodiscard]] auto as_color() const -> const color_type&;

  /// \} End of checked value retrieval

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

/// \} End of group core

}  // namespace tactile
