#ifndef RUNE_IO_INI_VALUE_HPP
#define RUNE_IO_INI_VALUE_HPP

#include <concepts>   // convertible_to, integral, floating_point, same_as
#include <nenya.hpp>  // strong_type
#include <ostream>    // ostream
#include <string>     // basic_string, to_string
#include <utility>    // move
#include <variant>    // variant, get, get_if, holds_alternative

#include "../aliases/integers.hpp"

namespace rune {

/// \addtogroup io
/// \{

/// \name Ini
/// \{

// clang-format off

/**
 * \brief Requires that a type is either a signed integer, unsigned integer,
 * floating-point number or a string.
 *
 * \tparam T the type that will be checked.
 * \tparam Char the character type.
 *
 * \since 0.1.0
 */
template <typename T, typename Char>
concept is_ini_value = std::integral<T> ||
                       std::floating_point<T> ||
                       std::constructible_from<std::basic_string<Char>, T>;

// clang-format on

/**
 * \brief Represents a value of an individual ini element.
 *
 * \tparam Char the character type that will be used.
 *
 * \see `ini_value`
 * \see `basic_ini`
 * \see `basic_ini_section`
 *
 * \since 0.1.0
 */
template <typename Char>
class basic_ini_value final
{
 public:
  using char_type = Char;
  using string_type = std::basic_string<char_type>;
  using int_type = int64;
  using uint_type = uint64;
  using float_type = double;
  using value_type = std::variant<string_type, bool, int_type, uint_type, float_type>;

  /// \name Construction
  /// \{

  /**
   * \brief Creates a `basic_ini_value` with an empty string as its value.
   *
   * \since 0.1.0
   */
  basic_ini_value() = default;

  basic_ini_value(const basic_ini_value&) = default;
  basic_ini_value(basic_ini_value&&) noexcept = default;

  basic_ini_value& operator=(const basic_ini_value&) = default;
  basic_ini_value& operator=(basic_ini_value&&) noexcept = default;

  /**
   * \brief Creates a `basic_ini_value` instance.
   *
   * \tparam T the type of the value.
   *
   * \param value the value that will be stored.
   *
   * \since 0.1.0
   */
  template <is_ini_value<char_type> T>
  /*implicit*/ basic_ini_value(T value)  // NOLINT
  {
    assign(std::move(value));
  }

  /**
   * \brief Assigns a new value to the instance.
   *
   * \tparam T the type of the new value.
   *
   * \param value the new value of the `basic_ini_value`.
   *
   * \return the `basic_ini_value` instance.
   *
   * \since 0.1.0
   */
  template <is_ini_value<char_type> T>
  auto operator=(T value) -> basic_ini_value&
  {
    assign(std::move(value));
    return *this;
  }

  /// \} End of construction

  /**
   * \brief Outputs the value to an output stream.
   *
   * \param stream the output stream that will be used.
   *
   * \since 0.1.0
   */
  void dump(std::ostream& stream) const
  {
    if (const auto* str = try_get_string())
    {
      stream << *str;
    }
    else if (const auto* i = try_get_int())
    {
      stream << std::to_string(*i);
    }
    else if (const auto* u = try_get_uint())
    {
      stream << std::to_string(*u) << 'u';
    }
    else if (const auto* f = try_get_float())
    {
      stream << std::to_string(*f);
    }
    else if (const auto* b = try_get_bool())
    {
      stream << ((*b) ? "true" : "false");
    }
  }

  /**
   * \brief Returns the underlying representation.
   *
   * \return the underlying variant value.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto get() const -> const value_type&
  {
    return m_value;
  }

  /// \name Checked getters
  /// \{

  /**
   * \brief Returns the underlying value as the specified type.
   *
   * \details This function can be used to safely cast the underlying value to the
   * specified type, as long as the types are in the same "family". In other words, where
   * the only difference between the types is the representation range. For example, if
   * the underlying value is a 64-bit signed integer, it is perfectly valid to cast it to
   * an `int`, since they are both signed integer types.
   *
   * \code{cpp}
   * rune::ini_value value = 42;  // Signed integer value (stored as 64-bit integer)
   *
   * auto i = value.as<int>();  // Fine, underlying type is a signed integer
   *
   * auto u = value.as<unsigned>();  // Error, no implicit signed to unsigned conversion
   * auto f = value.as<float>();     // Error, no implicit integer to float conversion
   * auto s = value.as<std::string>();  // Error, completely unrelated types
   * \endcode
   *
   * \tparam T the type of the returned value.
   *
   * \return the internal value casted to the specified type.
   *
   * \throws bad_variant_access if the underlying and specified types aren't related.
   *
   * \since 0.1.0
   */
  template <is_ini_value<char_type> T>
  [[nodiscard]] auto as() const -> T
  {
    if constexpr (std::same_as<T, bool>)
    {
      return std::get<bool>(m_value);
    }
    else if constexpr (std::signed_integral<T>)
    {
      return static_cast<T>(std::get<int64>(m_value));
    }
    else if constexpr (std::unsigned_integral<T>)
    {
      return static_cast<T>(std::get<uint64>(m_value));
    }
    else if constexpr (std::floating_point<T>)
    {
      return static_cast<T>(std::get<double>(m_value));
    }
    else
    {
      return std::get<string_type>(m_value);
    }
  }

  /**
   * \brief Assigns a value with the underlying value.
   *
   * \details The value conversion behavior of this function is identical to those of
   * the `as()` function.
   *
   * \code{cpp}
   * rune::ini_value value = "foo";
   *
   * std::string str;
   * value.get_to(str);  // Fine, the underlying type is a string
   *
   * int i{};
   * value.get_to(i);  // Error, cannot assign integer with string
   * \endcode
   *
   * \param[out] value a reference to the value that will be assigned.
   *
   * \throws bad_variant_access if there is a type mismatch.
   *
   * \since 0.1.0
   */
  void get_to(string_type& value) const
  {
    value = std::get<string_type>(m_value);
  }

  /// \copydoc get_to()
  void get_to(bool& value) const
  {
    value = std::get<bool>(m_value);
  }

  /// \copydoc get_to()
  void get_to(int8& value) const
  {
    value = static_cast<int8>(std::get<int_type>(m_value));
  }

  /// \copydoc get_to()
  void get_to(int16& value) const
  {
    value = static_cast<int16>(std::get<int_type>(m_value));
  }

  /// \copydoc get_to()
  void get_to(int32& value) const
  {
    value = static_cast<int32>(std::get<int_type>(m_value));
  }

  /// \copydoc get_to()
  void get_to(int64& value) const
  {
    value = std::get<int64>(m_value);
  }

  /// \copydoc get_to()
  void get_to(uint8& value) const
  {
    value = static_cast<uint8>(std::get<uint_type>(m_value));
  }

  /// \copydoc get_to()
  void get_to(uint16& value) const
  {
    value = static_cast<uint16>(std::get<uint_type>(m_value));
  }

  /// \copydoc get_to()
  void get_to(uint32& value) const
  {
    value = static_cast<uint32>(std::get<uint_type>(m_value));
  }

  /// \copydoc get_to()
  void get_to(uint64& value) const
  {
    value = static_cast<uint64>(std::get<uint_type>(m_value));
  }

  /// \copydoc get_to()
  void get_to(float& value) const
  {
    value = static_cast<float>(std::get<float_type>(m_value));
  }

  /// \copydoc get_to()
  void get_to(double& value) const
  {
    value = static_cast<double>(std::get<float_type>(m_value));
  }

  /// \copydoc get_to()
  void get_to(long double& value) const
  {
    value = static_cast<long double>(std::get<float_type>(m_value));
  }

  /// \copydoc get_to()
  template <is_ini_value<char_type> T, typename Tag, nenya::conversion Conv>
  void get_to(nenya::strong_type<T, Tag, Conv>& value) const
  {
    using strong_type = nenya::strong_type<T, Tag, Conv>;

    if constexpr (std::same_as<T, bool>)
    {
      value = strong_type{static_cast<T>(std::get<bool>(m_value))};
    }
    else if constexpr (std::signed_integral<T>)
    {
      value = strong_type{static_cast<T>(std::get<int_type>(m_value))};
    }
    else if constexpr (std::unsigned_integral<T>)
    {
      value = strong_type{static_cast<T>(std::get<uint_type>(m_value))};
    }
    else if constexpr (std::floating_point<T>)
    {
      value = strong_type{static_cast<T>(std::get<float_type>(m_value))};
    }
    else /*if constexpr (std::convertible_to<T, string_type>)*/
    {
      value = strong_type{static_cast<T>(std::get<string_type>(m_value))};
    }
  }

  /// \} End of checked getters

  /// \name Unchecked getters
  /// \{

  /**
   * \brief Returns a pointer to the underlying value.
   *
   * \return a pointer to the underlying value; null if the underlying value isn't a
   * string.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto try_get_string() const noexcept -> const string_type*
  {
    return std::get_if<string_type>(&m_value);
  }

  /**
   * \brief Returns a pointer to the underlying value.
   *
   * \return a pointer to the underlying value; null if the underlying value isn't an
   * integer.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto try_get_int() const noexcept -> const int_type*
  {
    return std::get_if<int_type>(&m_value);
  }

  /**
   * \brief Returns a pointer to the underlying value.
   *
   * \return a pointer to the underlying value; null if the underlying value isn't an
   * unsigned integer.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto try_get_uint() const noexcept -> const uint_type*
  {
    return std::get_if<uint_type>(&m_value);
  }

  /**
   * \brief Returns a pointer to the underlying value.
   *
   * \return a pointer to the underlying value; null if the underlying value isn't a
   * floating-point number.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto try_get_float() const noexcept -> const float_type*
  {
    return std::get_if<float_type>(&m_value);
  }

  /**
   * \brief Returns a pointer to the underlying value.
   *
   * \return a pointer to the underlying boolean value; null if the underlying value isn't
   * a boolean.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto try_get_bool() const noexcept -> const bool*
  {
    return std::get_if<bool>(&m_value);
  }

  /// \} End of unchecked getters

  /// \name Type indicators
  /// \{

  /**
   * \brief Indicates whether or not the value is a string.
   *
   * \return `true` if the value is a string; `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto is_string() const noexcept -> bool
  {
    return std::holds_alternative<string_type>(m_value);
  }

  /**
   * \brief Indicates whether or not the value is a signed integer.
   *
   * \return `true` if the value is a signed integer; `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto is_int() const noexcept -> bool
  {
    return std::holds_alternative<int_type>(m_value);
  }

  /**
   * \brief Indicates whether or not the value is an unsigned integer.
   *
   * \return `true` if the value is an unsigned integer; `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto is_uint() const noexcept -> bool
  {
    return std::holds_alternative<uint_type>(m_value);
  }

  /**
   * \brief Indicates whether or not the value is a floating-point number.
   *
   * \return `true` if the value is a floating-point number; `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto is_float() const noexcept -> bool
  {
    return std::holds_alternative<float_type>(m_value);
  }

  /**
   * \brief Indicates whether or not the value is a boolean.
   *
   * \return `true` if the value is a boolean; `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto is_bool() const noexcept -> bool
  {
    return std::holds_alternative<bool>(m_value);
  }

  /// \} End of type indicators

  [[nodiscard]] bool operator==(const basic_ini_value&) const = default;

 private:
  value_type m_value;

  template <is_ini_value<char_type> T>
  void assign(T value)
  {
    if constexpr (std::same_as<T, bool>)
    {
      m_value.template emplace<bool>(std::move(value));
    }
    else if constexpr (std::signed_integral<T>)
    {
      m_value.template emplace<int_type>(std::move(value));
    }
    else if constexpr (std::unsigned_integral<T>)
    {
      m_value.template emplace<uint_type>(std::move(value));
    }
    else if constexpr (std::floating_point<T>)
    {
      m_value.template emplace<float_type>(std::move(value));
    }
    else
    {
      m_value.template emplace<string_type>(std::move(value));
    }
  }
};

/**
 * \brief Alias for the most common use case of `basic_ini_value`.
 *
 * \since 0.1.0
 */
using ini_value = basic_ini_value<char>;

/**
 * \brief Prints a textual representation of an ini value.
 *
 * \tparam Char the used character type.
 *
 * \param stream the output stream that will be used.
 * \param value the ini value that will be printed.
 *
 * \return the used stream.
 *
 * \since 0.1.0
 */
template <typename Char>
auto operator<<(std::ostream& stream, const basic_ini_value<Char>& value) -> std::ostream&
{
  value.dump(stream);
  return stream;
}

/// \} End of ini

/// \} End of group io

}  // namespace rune

#endif  // RUNE_IO_INI_VALUE_HPP
