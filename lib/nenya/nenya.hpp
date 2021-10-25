#pragma once

#include <concepts>     // ...
#include <functional>   // hash
#include <type_traits>  // enable_if_t
#include <utility>      // move

/**
 * \namespace nenya
 *
 * \brief Contains a small library for easily creating strong types.
 *
 * \headerfile nenya.hpp
 */
namespace nenya {

// MSVC has better support for conditional noexcept specifier
#ifdef _MSC_VER
#define NENYA_NOEXCEPT_AS(expr) noexcept(noexcept((expr)))
#else
#define NENYA_NOEXCEPT_AS(expr)
#endif  // _MSC_VER

// clang-format off

template <typename T>
concept pre_increment = requires(T t) { { ++t } -> std::convertible_to<T>; };

template <typename T>
concept post_increment = requires(T t) { { t++ } -> std::convertible_to<T>; };

template <typename T>
concept pre_decrement = requires(T t) { { --t } -> std::convertible_to<T>; };

template <typename T>
concept post_decrement = requires(T t) { { t-- } -> std::convertible_to<T>; };

template <typename T>
concept addition = requires(T t) { { t + t } -> std::convertible_to<T>; };

template <typename T>
concept addition_assignment = requires(T t) { { t += t }; };

template <typename T>
concept unary_plus = requires(T t) { { +t } -> std::convertible_to<T>; };

template <typename T>
concept subtraction = requires(T t) { { t - t } -> std::convertible_to<T>; };

template <typename T>
concept subtraction_assignment = requires(T t) { { t -= t }; };

template <typename T>
concept unary_minus = requires(T t) { { -t } -> std::convertible_to<T>; };

template <typename T>
concept multiplication = requires(T t) { { t * t } -> std::convertible_to<T>; };

template <typename T>
concept multiplication_assignment = requires(T t) { { t *= t }; };

template <typename T>
concept division = requires(T t) { { t / t } -> std::convertible_to<T>; };

template <typename T>
concept division_assignment = requires(T t) { { t /= t }; };

template <typename T>
concept modulo = requires(T t) { { t % t } -> std::convertible_to<T>; };

template <typename T>
concept modulo_assignment = requires(T t) { { t %= t }; };

template <typename T>
concept bit_not = requires(T t) { { ~t } -> std::convertible_to<T>; };

template <typename T>
concept bit_and = requires(T t) { { t & t } -> std::convertible_to<T>; };

template <typename T>
concept bit_and_assignment = requires(T t) { { t &= t }; };

template <typename T>
concept bit_or = requires(T t) { { t | t } -> std::convertible_to<T>; };

template <typename T>
concept bit_or_assignment = requires(T t) { { t |= t }; };

template <typename T>
concept bit_xor = requires(T t) { { t ^ t } -> std::convertible_to<T>; };

template <typename T>
concept bit_xor_assignment = requires(T t) { { t ^= t }; };

template <typename T>
concept left_shift = requires(T t) { { t << t } -> std::convertible_to<T>; };

template <typename T>
concept left_shift_assignment = requires(T t) { { t <<= t }; };

template <typename T>
concept right_shift = requires(T t) { { t >> t } -> std::convertible_to<T>; };

template <typename T>
concept right_shift_assignment = requires(T t) { { t >>= t }; };

template <typename T>
concept bool_convertible = requires(T t) { { static_cast<bool>(t) }; };

template <typename T>
concept less = requires(T t) { { t < t } -> std::convertible_to<bool>; };

template <typename T>
concept less_equal = requires(T t) { { t <= t } -> std::convertible_to<bool>; };

template <typename T>
concept greater = requires(T t) { { t > t } -> std::convertible_to<bool>; };

template <typename T>
concept greater_equal = requires(T t) { { t >= t } -> std::convertible_to<bool>; };

template <typename T>
concept negation = requires(T t) { { !t } -> std::convertible_to<bool>; };

template <typename T, typename Index>
concept subscript = requires(T t, Index i) { { t[i] }; };

template<typename T>
concept hashable = requires(T a)
{
  { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
  noexcept(std::hash<T>{}(a));
};

// clang-format on

enum conversion
{
  implicit_conversion,
  explicit_conversion
};

/**
 * \class strong_type
 *
 * \brief A strong type that automatically inherits all of the properties of the
 * representation type.
 *
 * \details This class will mirror adopt all available operators except for
 * `operator*`, `operator&&` and `operator||`. The dereference operator is
 * considered too error-prone for a strong type, it's easy to misuse.
 * Furthermore, the logical AND and OR operators lose their short-circuit
 * semantics when overloaded.
 *
 * \details This class is hashable if `Rep` provides a specialization of
 * `std::hash`. It works out-of-the-box with common types such as `int` or
 * `std::string`.
 *
 * \details Furthermore, this class is `constexpr` and `noexcept` (on MSVC) aware and will
 * respect the properties of the underlying type.
 *
 * \tparam Rep the representation type, e.g `int` or `std::string`.
 * \tparam Tag the tag type that uniquely identifies the type.
 * \tparam Conversion `explicit_conversion` if the conversion operator to the underlying
 * type should be explicit; `implicit_conversion` otherwise.
 *
 * \headerfile nenya.hpp
 */
template <typename Rep, typename Tag, conversion Conversion = implicit_conversion>
class strong_type
{
  // clang-format off

  inline constexpr static bool nothrowCopy = std::is_nothrow_copy_constructible_v<Rep>;
  inline constexpr static bool nothrowMove = std::is_nothrow_copy_constructible_v<Rep>;

 public:
  using value_type = Rep;

  constexpr strong_type() = default;

  constexpr explicit strong_type(const value_type& value) noexcept(nothrowCopy)
      : m_value{value}
  {}

  constexpr explicit strong_type(value_type&& value) noexcept(nothrowMove)
      : m_value{std::move(value)}
  {}

  /// \name Unary operators
  /// \{

  constexpr strong_type& operator++() NENYA_NOEXCEPT_AS(++m_value)
      requires pre_increment<value_type>
  {
    ++m_value;
    return *this;
  }

  constexpr strong_type operator++(int) NENYA_NOEXCEPT_AS(m_value++)
      requires post_increment<value_type>
  {
    return strong_type{m_value++};
  }

  constexpr strong_type& operator--() NENYA_NOEXCEPT_AS(--m_value)
      requires pre_decrement<value_type>
  {
    --m_value;
    return *this;
  }

  constexpr strong_type operator--(int) NENYA_NOEXCEPT_AS(m_value--)
      requires post_decrement<value_type>
  {
    return strong_type{m_value--};
  }

  [[nodiscard]]
  constexpr auto operator+() const NENYA_NOEXCEPT_AS(+m_value)
      requires unary_plus<value_type>
  {
    return strong_type{+m_value};
  }

  [[nodiscard]]
  constexpr auto operator-() const NENYA_NOEXCEPT_AS(-m_value)
      requires unary_minus<value_type>
  {
    return strong_type{-m_value};
  }

  [[nodiscard]]
  constexpr auto operator~() const NENYA_NOEXCEPT_AS(~m_value)
      requires bit_not<value_type>
  {
    return strong_type{~m_value};
  }

  [[nodiscard]]
  constexpr auto operator!() const NENYA_NOEXCEPT_AS(!m_value)
      requires negation<value_type>
  {
    return !m_value;
  }

  template <typename Index> requires subscript<value_type, Index>
  constexpr decltype(auto) operator[](const Index& key) NENYA_NOEXCEPT_AS(m_value[key])
  {
    return m_value[key];
  }

  template <typename Index> requires subscript<value_type, Index>
  constexpr decltype(auto) operator[](const Index& key) const NENYA_NOEXCEPT_AS(m_value[key])
  {
    return m_value[key];
  }

  /// \} // end of unary operators

  /// \name Arithmetic operators
  /// \{

  [[nodiscard]]
  constexpr auto operator+(const strong_type& rhs) const NENYA_NOEXCEPT_AS(m_value + rhs.m_value)
      requires addition<value_type>
  {
    return strong_type{m_value + rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator-(const strong_type& rhs) const NENYA_NOEXCEPT_AS(m_value - rhs.m_value)
      requires subtraction<value_type>
  {
    return strong_type{m_value - rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator/(const strong_type& rhs) const NENYA_NOEXCEPT_AS(m_value / rhs.m_value)
      requires division<value_type>
  {
    return strong_type{m_value / rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator*(const strong_type& rhs) const NENYA_NOEXCEPT_AS(m_value * rhs.m_value)
      requires multiplication<value_type>
  {
    return strong_type{m_value * rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator%(const strong_type& rhs) const NENYA_NOEXCEPT_AS(m_value % rhs.m_value)
      requires modulo<value_type>
  {
    return strong_type{m_value % rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator&(const strong_type& rhs) const NENYA_NOEXCEPT_AS(m_value & rhs.m_value)
      requires bit_and<value_type>
  {
    return strong_type{m_value & rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator|(const strong_type& rhs) const NENYA_NOEXCEPT_AS(m_value | rhs.m_value)
      requires bit_or<value_type>
  {
    return strong_type{m_value | rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator^(const strong_type& rhs) const NENYA_NOEXCEPT_AS(m_value ^ rhs.m_value)
      requires bit_xor<value_type>
  {
    return strong_type{m_value ^ rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator<<(const strong_type& rhs) const NENYA_NOEXCEPT_AS(m_value << rhs.m_value)
      requires left_shift<value_type>
  {
    return strong_type{m_value << rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator>>(const strong_type& rhs) const NENYA_NOEXCEPT_AS(m_value >> rhs.m_value)
      requires right_shift<value_type>
  {
    return strong_type{m_value >> rhs.m_value};
  }

  /// \} // end of arithmetic operators

  /// \name Assignment operators
  /// \{

  constexpr strong_type& operator+=(const strong_type& rhs) NENYA_NOEXCEPT_AS(m_value += rhs.m_value)
      requires addition_assignment<value_type>
  {
    m_value += rhs.m_value;
    return *this;
  }

  constexpr strong_type& operator-=(const strong_type& rhs) NENYA_NOEXCEPT_AS(m_value -= rhs.m_value)
      requires subtraction_assignment<value_type>
  {
    m_value -= rhs.m_value;
    return *this;
  }

  constexpr strong_type& operator/=(const strong_type& rhs) NENYA_NOEXCEPT_AS(m_value /= rhs.m_value)
      requires division_assignment<value_type>
  {
    m_value /= rhs.m_value;
    return *this;
  }

  constexpr strong_type& operator*=(const strong_type& rhs) NENYA_NOEXCEPT_AS(m_value *= rhs.m_value)
      requires multiplication_assignment<value_type>
  {
    m_value *= rhs.m_value;
    return *this;
  }

  constexpr strong_type& operator%=(const strong_type& rhs) NENYA_NOEXCEPT_AS(m_value %= rhs.m_value)
      requires modulo_assignment<value_type>
  {
    m_value %= rhs.m_value;
    return *this;
  }

  constexpr strong_type& operator&=(const strong_type& rhs) NENYA_NOEXCEPT_AS(m_value &= rhs.m_value)
      requires bit_and_assignment<value_type>
  {
    m_value &= rhs.m_value;
    return *this;
  }

  constexpr strong_type& operator|=(const strong_type& rhs) NENYA_NOEXCEPT_AS(m_value |= rhs.m_value)
      requires bit_or_assignment<value_type>
  {
    m_value |= rhs.m_value;
    return *this;
  }

  constexpr strong_type& operator^=(const strong_type& rhs) NENYA_NOEXCEPT_AS(m_value ^= rhs.m_value)
      requires bit_xor_assignment<value_type>
  {
    m_value ^= rhs.m_value;
    return *this;
  }

  constexpr strong_type& operator<<=(const strong_type& rhs) NENYA_NOEXCEPT_AS(m_value <<= rhs.m_value)
      requires left_shift_assignment<value_type>
  {
    m_value <<= rhs.m_value;
    return *this;
  }

  constexpr strong_type& operator>>=(const strong_type& rhs) NENYA_NOEXCEPT_AS(m_value >>= rhs.m_value)
      requires right_shift_assignment<value_type>
  {
    m_value >>= rhs.m_value;
    return *this;
  }

  /// \} // end of assignment operators

  /// \name Comparison operators
  /// \{

  [[nodiscard]]
  constexpr auto operator==(const strong_type& rhs) const NENYA_NOEXCEPT_AS(m_value == rhs.m_value)
      requires std::equality_comparable<value_type>
  {
    return m_value == rhs.m_value;
  }

  [[nodiscard]]
  constexpr auto operator!=(const strong_type& rhs) const NENYA_NOEXCEPT_AS(m_value != rhs.m_value)
      requires std::equality_comparable<value_type>
  {
    return m_value != rhs.m_value;
  }

  [[nodiscard]]
  constexpr auto operator<(const strong_type& rhs) const NENYA_NOEXCEPT_AS(m_value < rhs.m_value)
      requires less<value_type>
  {
    return m_value < rhs.m_value;
  }

  [[nodiscard]]
  constexpr auto operator<=(const strong_type& rhs) const NENYA_NOEXCEPT_AS(m_value <= rhs.m_value)
      requires less_equal<value_type>
  {
    return m_value <= rhs.m_value;
  }

  [[nodiscard]]
  constexpr auto operator>(const strong_type& rhs) const NENYA_NOEXCEPT_AS(m_value > rhs.m_value)
      requires greater<value_type>
  {
    return m_value > rhs.m_value;
  }

  [[nodiscard]]
  constexpr auto operator>=(const strong_type& rhs) const NENYA_NOEXCEPT_AS(m_value >= rhs.m_value)
      requires greater_equal<value_type>
  {
    return m_value >= rhs.m_value;
  }

  // clang-format on

  /// \} end of comparison operators

  [[nodiscard]] constexpr value_type& get() noexcept(nothrowCopy)
  {
    return m_value;
  }

  [[nodiscard]] constexpr const value_type& get() const noexcept(nothrowCopy)
  {
    return m_value;
  }

  const value_type* operator->() const noexcept
  {
    return &m_value;
  }

  value_type* operator->() noexcept
  {
    return &m_value;
  };

  // clang-format off

  constexpr explicit(Conversion == explicit_conversion) operator value_type() const
      noexcept(nothrowCopy)
  {
    return m_value;
  }

  constexpr explicit operator bool() const NENYA_NOEXCEPT_AS(static_cast<bool>(m_value))
      requires bool_convertible<value_type> && (!std::same_as<value_type, bool>)
  {
    return static_cast<bool>(m_value);
  }

  // clang-format on

  void serialize(auto& archive)
  {
    archive(m_value);
  }

 private:
  value_type m_value{};
};

}  // namespace nenya

// clang-format off

namespace std {

template <nenya::hashable Rep, typename Tag>
struct hash<nenya::strong_type<Rep, Tag>> final
{
  using strong_type = nenya::strong_type<Rep, Tag>;

  size_t operator()(const strong_type& key) const noexcept
  {
    return std::hash<Rep>{}(key.get());
  }
};

}  // namespace std

// clang-format on