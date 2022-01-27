#pragma once

#include <filesystem>  // path
#include <string>      // string
#include <utility>     // move
#include <variant>     // variant, get, get_if, holds_alternative

#include <centurion.hpp>
#include <tactile_def.hpp>
#include <tactile_stdlib.hpp>

#include "assert.hpp"
#include "throw.hpp"

namespace tactile {

/// \addtogroup core
/// \{

/**
 * \brief Strong type that represents object references.
 */
enum object_t : int32 {};

template <typename T>
concept is_attribute_type = std::same_as<T, std::string> ||  //
    std::same_as<T, int> ||                                  //
    std::same_as<T, float> ||                                //
    std::same_as<T, bool> ||                                 //
    std::same_as<T, cen::color> ||                           //
    std::same_as<T, std::filesystem::path> ||                //
    std::same_as<T, object_t>;

/**
 * \brief Represents an "attribute" value, used by both property and component facilities.
 */
class attribute_value final {
 public:
  using string_type = std::string;
  using integer_type = int;
  using float_type = float;
  using color_type = cen::color;
  using file_type = std::filesystem::path;

  using value_type = std::variant<string_type,
                                  integer_type,
                                  float_type,
                                  bool,
                                  color_type,
                                  file_type,
                                  object_t>;

  /// \name Construction
  /// \{

  /**
   * \brief Creates an empty string attribute.
   */
  attribute_value() = default;

  /**
   * \brief Creates a property.
   *
   * \tparam T the type of the property value.
   *
   * \param value the attribute value.
   */
  template <is_attribute_type T>
  /*implicit*/ attribute_value(T value)
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
  template <is_attribute_type T>
  void set_value(T value)
  {
    mValue.emplace<T>(std::move(value));
  }

  /**
   * \brief Resets the value of the attribute to the the default of a specific type.
   *
   * \param type the new value type.
   */
  void reset_to_default(const PropertyType type)
  {
    if (type == PropertyType::Integer) {
      set_value<integer_type>(0);
    }
    else if (type == PropertyType::Floating) {
      set_value<float_type>(0);
    }
    else if (type == PropertyType::Boolean) {
      set_value<bool>(false);
    }
    else if (type == PropertyType::String) {
      set_value<string_type>(string_type{});
    }
    else if (type == PropertyType::Color) {
      set_value<color_type>(cen::colors::black);
    }
    else if (type == PropertyType::Object) {
      set_value<object_t>(object_t{});
    }
    else if (type == PropertyType::File) {
      set_value<file_type>(file_type{});
    }
    else {
      ThrowTraced(TactileError{"Invalid attribute type!"});
    }
  }

  /**
   * \brief Indicates whether the stored value is the default one.
   *
   * \details Black is the default color.
   *
   * \return `true` if the attribute stores a default value; `false` otherwise.
   */
  [[nodiscard]] auto has_default_value() const -> bool
  {
    if (const auto* str = std::get_if<string_type>(&mValue)) {
      return str->empty();
    }
    else if (const auto* i = std::get_if<integer_type>(&mValue)) {
      return *i == 0;
    }
    else if (const auto* f = std::get_if<float_type>(&mValue)) {
      return *f == 0;
    }
    else if (const auto* b = std::get_if<bool>(&mValue)) {
      return !*b;
    }
    else if (const auto* path = std::get_if<file_type>(&mValue)) {
      return path->empty();
    }
    else if (const auto* obj = std::get_if<object_t>(&mValue)) {
      return *obj == object_t{};
    }
    else if (const auto* color = std::get_if<color_type>(&mValue)) {
      return *color == cen::colors::white;
    }
    else {
      ThrowTraced(TactileError{"Invalid property type!"});
    }
  }

  /// \} End of value functions

  /// \name Type checks
  /// \{

  /**
   * \brief Returns the type of the attribute's stored value.
   *
   * \return the attribute type.
   */
  [[nodiscard]] auto type() const noexcept -> PropertyType
  {
    if (holds<integer_type>()) {
      return PropertyType::Integer;
    }
    else if (holds<float_type>()) {
      return PropertyType::Floating;
    }
    else if (holds<bool>()) {
      return PropertyType::Boolean;
    }
    else if (holds<object_t>()) {
      return PropertyType::Object;
    }
    else if (holds<color_type>()) {
      return PropertyType::Color;
    }
    else if (holds<file_type>()) {
      return PropertyType::File;
    }
    else {
      TACTILE_ASSERT(holds<string_type>());
      return PropertyType::String;
    }
  }

  /**
   * \brief Indicates whether the attribute holds a string value.
   *
   * \return `true` if the attribute is a string; `false` otherwise.
   */
  [[nodiscard]] auto is_string() const noexcept -> bool
  {
    return type() == PropertyType::String;
  }

  /**
   * \brief Indicates whether the attribute holds an integer value.
   *
   * \return `true` if the attribute is an integer; `false` otherwise.
   */
  [[nodiscard]] auto is_int() const noexcept -> bool
  {
    return type() == PropertyType::Integer;
  }

  /**
   * \brief Indicates whether the attribute holds a float value.
   *
   * \return `true` if the attribute is a float; `false` otherwise.
   */
  [[nodiscard]] auto is_float() const noexcept -> bool
  {
    return type() == PropertyType::Floating;
  }

  /**
   * \brief Indicates whether the attribute holds a boolean value.
   *
   * \return `true` if the attribute is a boolean; `false` otherwise.
   */
  [[nodiscard]] auto is_bool() const noexcept -> bool
  {
    return type() == PropertyType::Boolean;
  }

  /**
   * \brief Indicates whether the attribute holds a file path value.
   *
   * \return `true` if the attribute is a file; `false` otherwise.
   */
  [[nodiscard]] auto is_file() const noexcept -> bool
  {
    return type() == PropertyType::File;
  }

  /**
   * \brief Indicates whether the attribute holds an object reference value.
   *
   * \return `true` if the attribute is an object reference; `false` otherwise.
   */
  [[nodiscard]] auto is_object() const noexcept -> bool
  {
    return type() == PropertyType::Object;
  }

  /**
   * \brief Indicates whether the attribute holds a color value.
   *
   * \return `true` if the attribute is a color; `false` otherwise.
   */
  [[nodiscard]] auto is_color() const noexcept -> bool
  {
    return type() == PropertyType::Color;
  }

  /// \} End of type checks

  /// \name Non-throwing value retrieval
  /// \{

  [[nodiscard]] auto try_as_string() const noexcept -> const string_type*
  {
    return get_if<string_type>();
  }

  [[nodiscard]] auto try_as_int() const noexcept -> const integer_type*
  {
    return get_if<integer_type>();
  }

  [[nodiscard]] auto try_as_float() const noexcept -> const float_type*
  {
    return get_if<float_type>();
  }

  [[nodiscard]] auto try_as_bool() const noexcept -> const bool*
  {
    return get_if<bool>();
  }

  [[nodiscard]] auto try_as_file() const noexcept -> const file_type*
  {
    return get_if<file_type>();
  }

  [[nodiscard]] auto try_as_object() const noexcept -> const object_t*
  {
    return get_if<object_t>();
  }

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
  [[nodiscard]] auto as_string() const -> const string_type&
  {
    if (const auto* str = get_if<string_type>()) {
      return *str;
    }
    else {
      ThrowTraced(TactileError{"Attribute was not a string!"});
    }
  }

  /**
   * \brief Returns the attribute's integer value.
   *
   * \return the stored value.
   *
   * \throws TactileError if the attribute isn't an integer.
   */
  [[nodiscard]] auto as_int() const -> integer_type
  {
    if (const auto* i = get_if<integer_type>()) {
      return *i;
    }
    else {
      ThrowTraced(TactileError{"Attribute was not an integer!"});
    }
  }

  /**
   * \brief Returns the attribute's float value.
   *
   * \return the stored value.
   *
   * \throws TactileError if the attribute isn't a float.
   */
  [[nodiscard]] auto as_float() const -> float_type
  {
    if (const auto* f = get_if<float_type>()) {
      return *f;
    }
    else {
      ThrowTraced(TactileError{"Attribute was not a float!"});
    }
  }

  /**
   * \brief Returns the attribute's boolean value.
   *
   * \return the stored value.
   *
   * \throws TactileError if the attribute isn't a boolean.
   */
  [[nodiscard]] auto as_bool() const -> bool
  {
    if (const auto* b = get_if<bool>()) {
      return *b;
    }
    else {
      ThrowTraced(TactileError{"Attribute was not a boolean!"});
    }
  }

  /**
   * \brief Returns the attribute's file value.
   *
   * \return a reference to the stored value.
   *
   * \throws TactileError if the attribute isn't a file.
   */
  [[nodiscard]] auto as_file() const -> const file_type&
  {
    if (const auto* file = get_if<file_type>()) {
      return *file;
    }
    else {
      ThrowTraced(TactileError{"Attribute was not a file!"});
    }
  }

  /**
   * \brief Returns the attribute's object reference value.
   *
   * \return the stored value.
   *
   * \throws TactileError if the attribute isn't an object reference.
   */
  [[nodiscard]] auto as_object() const -> object_t
  {
    if (const auto* obj = get_if<object_t>()) {
      return *obj;
    }
    else {
      ThrowTraced(TactileError{"Attribute was not an object reference!"});
    }
  }

  /**
   * \brief Returns the attribute's color value.
   *
   * \return the a reference to the stored value.
   *
   * \throws TactileError if the attribute isn't a color.
   */
  [[nodiscard]] auto as_color() const -> const color_type&
  {
    if (const auto* color = get_if<color_type>()) {
      return *color;
    }
    else {
      ThrowTraced(TactileError{"Attribute was not a color!"});
    }
  }

  /// \} End of checked value retrieval

 private:
  value_type mValue;

  template <is_attribute_type T>
  [[nodiscard]] auto holds() const noexcept -> bool
  {
    return std::holds_alternative<T>(mValue);
  }

  template <is_attribute_type T>
  [[nodiscard]] auto get_if() const noexcept -> const T*
  {
    return std::get_if<T>(&mValue);
  }
};

/// \} End of group core

}  // namespace tactile