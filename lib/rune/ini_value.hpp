#ifndef RUNE_IO_INI_VALUE_HPP
#define RUNE_IO_INI_VALUE_HPP

#include <concepts>     // convertible_to, integral, floating_point, same_as
#include <nenya.hpp>    // strong_type
#include <ostream>      // ostream
#include <string>       // basic_string, to_string
#include <string_view>  // basic_string_view
#include <utility>      // move
#include <variant>      // variant, get, get_if, holds_alternative

#include "czstring.hpp"
#include "integers.hpp"

namespace rune {

/// \addtogroup io
/// \{

/// \name Ini
/// \{

// clang-format off

template <typename T, typename Char>
concept is_ini_value = std::integral<T> ||
                       std::floating_point<T> ||
                       std::constructible_from<std::basic_string<Char>, T>;

// clang-format on

template <typename Char>
class basic_ini_value final
{
 public:
  using char_type = Char;
  using string_type = std::basic_string<char_type>;
  using string_view_type = std::basic_string_view<char_type>;
  using value_type = std::variant<string_type, bool, int64, uint64, double>;

  basic_ini_value() = default;

  basic_ini_value(const basic_ini_value&) = default;
  basic_ini_value(basic_ini_value&&) noexcept = default;

  basic_ini_value& operator=(const basic_ini_value&) = default;
  basic_ini_value& operator=(basic_ini_value&&) noexcept = default;

  template <typename T>
  /*implicit*/ basic_ini_value(T value) requires is_ini_value<T, char_type>  // NOLINT
  {
    assign(std::move(value));
  }

  template <typename T>
  basic_ini_value& operator=(T value) requires is_ini_value<T, char_type>
  {
    assign(std::move(value));
    return *this;
  }

  // clang-format off

  template <typename T> requires is_ini_value<T, char_type>
  [[nodiscard]] auto get() const -> const T&
  {
    static_assert(std::convertible_to<T, string_type> ||
                  std::signed_integral<T> ||
                  std::unsigned_integral<T> ||
                  std::floating_point<T> ||
                  std::same_as<T, bool>,
                  "Invalid template type parameter to basic_ini_value::get!");
    // clang-format on

    if constexpr (std::same_as<T, bool>)
    {
      return std::get<bool>(m_value);
    }
    else if constexpr (std::signed_integral<T>)
    {
      return std::get<int64>(m_value);
    }
    else if constexpr (std::unsigned_integral<T>)
    {
      return std::get<uint64>(m_value);
    }
    else if constexpr (std::floating_point<T>)
    {
      return std::get<double>(m_value);
    }
    else /*if constexpr (std::convertible_to<T, string_type>)*/
    {
      return std::get<string_type>(m_value);
    }
  }

  void get_to(string_type& value) const
  {
    value = std::get<string_type>(m_value);
  }

  void get_to(bool& value) const
  {
    value = std::get<bool>(m_value);
  }

  void get_to(int8& value) const
  {
    value = static_cast<int8>(std::get<int64>(m_value));
  }

  void get_to(int16& value) const
  {
    value = static_cast<int16>(std::get<int64>(m_value));
  }

  void get_to(int32& value) const
  {
    value = static_cast<int32>(std::get<int64>(m_value));
  }

  void get_to(int64& value) const
  {
    value = std::get<int64>(m_value);
  }

  void get_to(uint8& value) const
  {
    value = static_cast<uint8>(std::get<uint64>(m_value));
  }

  void get_to(uint16& value) const
  {
    value = static_cast<uint16>(std::get<uint64>(m_value));
  }

  void get_to(uint32& value) const
  {
    value = static_cast<uint32>(std::get<uint64>(m_value));
  }

  void get_to(uint64& value) const
  {
    value = std::get<uint64>(m_value);
  }

  void get_to(float& value) const
  {
    value = static_cast<float>(std::get<double>(m_value));
  }

  void get_to(double& value) const
  {
    value = std::get<double>(m_value);
  }

  // clang-format off

  template <typename T, typename Tag, nenya::conversion Conv> requires is_ini_value<T, char_type>
  void get_to(nenya::strong_type<T, Tag, Conv>& value) const
  {
    using strong_type = nenya::strong_type<T, Tag, Conv>;

    if constexpr (std::same_as<T, bool>)
    {
      value = strong_type{static_cast<T>(std::get<bool>(m_value))};
    }
    else if constexpr (std::signed_integral<T>)
    {
      value = strong_type{static_cast<T>(std::get<int64>(m_value))};
    }
    else if constexpr (std::unsigned_integral<T>)
    {
      value = strong_type{static_cast<T>(std::get<uint64>(m_value))};
    }
    else if constexpr (std::floating_point<T>)
    {
      value = strong_type{static_cast<T>(std::get<double>(m_value))};
    }
    else /*if constexpr (std::convertible_to<T, string_type>)*/
    {
      value = strong_type{static_cast<T>(std::get<string_type>(m_value))};
    }
  }

  // clang-format on

  [[nodiscard]] auto get() const -> const value_type&
  {
    return m_value;
  }

  [[nodiscard]] auto try_get_string() const noexcept -> const string_type*
  {
    return std::get_if<string_type>(&m_value);
  }

  [[nodiscard]] auto try_get_int() const noexcept -> const int64*
  {
    return std::get_if<int64>(&m_value);
  }

  [[nodiscard]] auto try_get_uint() const noexcept -> const uint64*
  {
    return std::get_if<uint64>(&m_value);
  }

  [[nodiscard]] auto try_get_float() const noexcept -> const double*
  {
    return std::get_if<double>(&m_value);
  }

  [[nodiscard]] auto try_get_bool() const noexcept -> const bool*
  {
    return std::get_if<bool>(&m_value);
  }

  [[nodiscard]] auto is_string() const noexcept -> bool
  {
    return std::holds_alternative<string_type>(m_value);
  }

  [[nodiscard]] auto is_int() const noexcept -> bool
  {
    return std::holds_alternative<int64>(m_value);
  }

  [[nodiscard]] auto is_uint() const noexcept -> bool
  {
    return std::holds_alternative<uint64>(m_value);
  }

  [[nodiscard]] auto is_float() const noexcept -> bool
  {
    return std::holds_alternative<double>(m_value);
  }

  [[nodiscard]] auto is_bool() const noexcept -> bool
  {
    return std::holds_alternative<bool>(m_value);
  }

  [[nodiscard]] bool operator==(const basic_ini_value&) const = default;

 private:
  value_type m_value;

  template <typename T>
  void assign(T value) requires is_ini_value<T, char_type>
  {
    if constexpr (std::same_as<T, bool>)
    {
      m_value.template emplace<bool>(std::move(value));
    }
    else if constexpr (std::signed_integral<T>)
    {
      m_value.template emplace<int64>(std::move(value));
    }
    else if constexpr (std::unsigned_integral<T>)
    {
      m_value.template emplace<uint64>(std::move(value));
    }
    else if constexpr (std::floating_point<T>)
    {
      m_value.template emplace<double>(std::move(value));
    }
    else /*if constexpr (std::convertible_to<T, string_type>)*/
    {
      m_value.template emplace<string_type>(std::move(value));
    }
  }
};

using ini_value = basic_ini_value<char>;

template <typename Char>
auto operator<<(std::ostream& stream, const basic_ini_value<Char>& value) -> std::ostream&
{
  if (const auto* str = value.try_get_string())
  {
    stream << *str;
  }
  else if (const auto* i = value.try_get_int())
  {
    stream << std::to_string(*i);
  }
  else if (const auto* u = value.try_get_uint())
  {
    stream << std::to_string(*u);
  }
  else if (const auto* f = value.try_get_float())
  {
    stream << std::to_string(*f);
  }
  else if (const auto* b = value.try_get_bool())
  {
    stream << ((*b) ? "true" : "false");
  }

  return stream;
}

/// \} End of ini

/// \} End of group io

}  // namespace rune

#endif  // RUNE_IO_INI_VALUE_HPP
