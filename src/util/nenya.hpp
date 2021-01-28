#pragma once

#include <compare>
#include <concepts>    // convertible_to
#include <functional>  // hash
#include <utility>     // move

/**
 * \namespace nenya
 *
 * \brief Contains a small library for easily creating strong types.
 *
 * \headerfile nenya.hpp
 */
namespace nenya {

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
concept bit_and = requires(T t) { { t& t } -> std::convertible_to<T>; };

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
concept hashable = requires(T a) {
  { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
  noexcept(std::hash<T>{}(a));
};

// clang-format on

/**
 * \class mirror_type
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
 * \details Furthermore, this class is `constexpr` and `noexcept` aware and will
 * respect the properties of the underlying type.
 *
 * \tparam Rep the representation type, e.g `int` or `std::string`.
 * \tparam Tag the tag type that uniquely identifies the type.
 *
 * \headerfile nenya.hpp
 */
template <typename Rep, typename Tag>
class mirror_type
{
  // clang-format off

  inline constexpr static bool nothrowCopy = std::is_nothrow_copy_constructible_v<Rep>;
  inline constexpr static bool nothrowMove = std::is_nothrow_copy_constructible_v<Rep>;

 public:
  using value_type = Rep;

  constexpr mirror_type() = default;

  constexpr explicit mirror_type(const Rep& value) noexcept(nothrowCopy)
      : m_value{value}
  {}

  constexpr explicit mirror_type(Rep&& value) noexcept(nothrowMove)
      : m_value{std::move(value)}
  {}

  /// \name Unary operators
  /// \{

  constexpr mirror_type& operator++() noexcept(noexcept(++m_value))
      requires pre_increment<Rep>
  {
    ++m_value;
    return *this;
  }

  constexpr mirror_type operator++(int) noexcept(noexcept(m_value++))
      requires post_increment<Rep>
  {
    return mirror_type{m_value++};
  }

  constexpr mirror_type& operator--() noexcept(noexcept(--m_value))
      requires pre_decrement<Rep>
  {
    --m_value;
    return *this;
  }

  constexpr mirror_type operator--(int) noexcept(noexcept(m_value--))
      requires post_decrement<Rep>
  {
    return mirror_type{m_value--};
  }

  [[nodiscard]]
  constexpr auto operator+() const noexcept(noexcept(+m_value))
      requires unary_plus<Rep>
  {
    return mirror_type{+m_value};
  }

  [[nodiscard]]
  constexpr auto operator-() const noexcept(noexcept(-m_value))
      requires unary_minus<Rep>
  {
    return mirror_type{-m_value};
  }

  [[nodiscard]]
  constexpr auto operator~() const noexcept(noexcept(~m_value))
      requires bit_not<Rep>
  {
    return mirror_type{~m_value};
  }

  [[nodiscard]]
  constexpr auto operator!() const noexcept(noexcept(!m_value))
      requires negation<Rep>
  {
    return !m_value;
  }

  template <typename Index> requires subscript<Rep, Index>
  constexpr decltype(auto) operator[](const Index& key)
      noexcept(noexcept(m_value.operator[](key)))
  {
    return m_value[key];
  }

  template <typename Index> requires subscript<Rep, Index>
  constexpr decltype(auto) operator[](const Index& key) const
      noexcept(noexcept(m_value.operator[](key)))
  {
    return m_value[key];
  }

  /// \} // end of unary operators

  /// \name Arithmetic operators
  /// \{

  [[nodiscard]]
  constexpr auto operator+(const mirror_type& rhs) const
      noexcept(noexcept(m_value + rhs.m_value))
      requires addition<Rep>
  {
    return mirror_type{m_value + rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator-(const mirror_type& rhs) const
      noexcept(noexcept(m_value - rhs.m_value))
      requires subtraction<Rep>
  {
    return mirror_type{m_value - rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator/(const mirror_type& rhs) const
      noexcept(noexcept(m_value / rhs.m_value))
      requires division<Rep>
  {
    return mirror_type{m_value / rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator*(const mirror_type& rhs) const
      noexcept(noexcept(m_value * rhs.m_value))
      requires multiplication<Rep>
  {
    return mirror_type{m_value * rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator%(const mirror_type& rhs) const
      noexcept(noexcept(m_value % rhs.m_value))
      requires modulo<Rep>
  {
    return mirror_type{m_value % rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator&(const mirror_type& rhs) const
      noexcept(noexcept(m_value & rhs.m_value))
      requires bit_and<Rep>
  {
    return mirror_type{m_value & rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator|(const mirror_type& rhs) const
      noexcept(noexcept(m_value | rhs.m_value))
      requires bit_or<Rep>
  {
    return mirror_type{m_value | rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator^(const mirror_type& rhs) const
      noexcept(noexcept(m_value ^ rhs.m_value))
      requires bit_xor<Rep>
  {
    return mirror_type{m_value ^ rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator<<(const mirror_type& rhs) const
      noexcept(noexcept(m_value << rhs.m_value))
      requires left_shift<Rep>
  {
    return mirror_type{m_value << rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator>>(const mirror_type& rhs) const
      noexcept(noexcept(m_value >> rhs.m_value))
      requires right_shift<Rep>
  {
    return mirror_type{m_value >> rhs.m_value};
  }

  /// \} // end of arithmetic operators

  /// \name Assignment operators
  /// \{

  constexpr mirror_type& operator+=(const mirror_type& rhs)
      noexcept(noexcept(m_value += rhs.m_value))
      requires addition_assignment<Rep>
  {
    m_value += rhs.m_value;
    return *this;
  }

  constexpr mirror_type& operator-=(const mirror_type& rhs)
      noexcept(noexcept(m_value -= rhs.m_value))
      requires subtraction_assignment<Rep>
  {
    m_value -= rhs.m_value;
    return *this;
  }

  constexpr mirror_type& operator/=(const mirror_type& rhs)
      noexcept(noexcept(m_value /= rhs.m_value))
      requires division_assignment<Rep>
  {
    m_value /= rhs.m_value;
    return *this;
  }

  constexpr mirror_type& operator*=(const mirror_type& rhs)
      noexcept(noexcept(m_value *= rhs.m_value))
      requires multiplication_assignment<Rep>
  {
    m_value *= rhs.m_value;
    return *this;
  }

  constexpr mirror_type& operator%=(const mirror_type& rhs)
      noexcept(noexcept(m_value %= rhs.m_value))
      requires modulo_assignment<Rep>
  {
    m_value %= rhs.m_value;
    return *this;
  }

  constexpr mirror_type& operator&=(const mirror_type& rhs)
      noexcept(noexcept(m_value &= rhs.m_value))
      requires bit_and_assignment<Rep>
  {
    m_value &= rhs.m_value;
    return *this;
  }

  constexpr mirror_type& operator|=(const mirror_type& rhs)
      noexcept(noexcept(m_value |= rhs.m_value))
      requires bit_or_assignment<Rep>
  {
    m_value |= rhs.m_value;
    return *this;
  }

  constexpr mirror_type& operator^=(const mirror_type& rhs)
      noexcept(noexcept(m_value ^= rhs.m_value))
      requires bit_xor_assignment<Rep>
  {
    m_value ^= rhs.m_value;
    return *this;
  }

  constexpr mirror_type& operator<<=(const mirror_type& rhs)
      noexcept(noexcept(m_value <<= rhs.m_value))
      requires left_shift_assignment<Rep>
  {
    m_value <<= rhs.m_value;
    return *this;
  }

  constexpr mirror_type& operator>>=(const mirror_type& rhs)
      noexcept(noexcept(m_value >>= rhs.m_value))
      requires right_shift_assignment<Rep>
  {
    m_value >>= rhs.m_value;
    return *this;
  }

  /// \} // end of assignment operators

  /// \name Comparison operators
  /// \{

  [[nodiscard]]
  constexpr auto operator==(const mirror_type& rhs) const
      noexcept(noexcept(m_value == rhs.m_value))
      requires std::equality_comparable<Rep>
  {
    return m_value == rhs.m_value;
  }

  [[nodiscard]]
  constexpr auto operator!=(const mirror_type& rhs) const
      noexcept(noexcept(m_value != rhs.m_value))
      requires std::equality_comparable<Rep>
  {
    return m_value != rhs.m_value;
  }

  [[nodiscard]]
  constexpr auto operator<(const mirror_type& rhs) const
      noexcept(noexcept(m_value < rhs.m_value))
      requires less<Rep>
  {
    return m_value < rhs.m_value;
  }

  [[nodiscard]]
  constexpr auto operator<=(const mirror_type& rhs) const
      noexcept(noexcept(m_value <= rhs.m_value))
      requires less_equal<Rep>
  {
    return m_value <= rhs.m_value;
  }

  [[nodiscard]]
  constexpr auto operator>(const mirror_type& rhs) const
      noexcept(noexcept(m_value > rhs.m_value))
      requires greater<Rep>
  {
    return m_value > rhs.m_value;
  }

  [[nodiscard]]
  constexpr auto operator>=(const mirror_type& rhs) const
      noexcept(noexcept(m_value >= rhs.m_value))
      requires greater_equal<Rep>
  {
    return m_value >= rhs.m_value;
  }

  // clang-format on

  /// \} end of comparison operators

  [[nodiscard]] constexpr Rep& get() noexcept(nothrowCopy)
  {
    return m_value;
  }

  [[nodiscard]] constexpr const Rep& get() const noexcept(nothrowCopy)
  {
    return m_value;
  }

  const Rep* operator->() const noexcept
  {
    return &m_value;
  }

  Rep* operator->() noexcept
  {
    return &m_value;
  };

  constexpr explicit operator Rep() const noexcept(nothrowCopy)
  {
    return m_value;
  }

  // clang-format off

  constexpr explicit operator bool() const
      noexcept(noexcept(static_cast<bool>(m_value)))
      requires bool_convertible<Rep> && (!std::same_as<Rep, bool>)
  {
    return static_cast<bool>(m_value);
  }

  // clang-format on

 private:
  Rep m_value{};
};

}  // namespace nenya

// clang-format off

namespace std {

template <nenya::hashable Rep, typename Tag>
struct hash<nenya::mirror_type<Rep, Tag>> final
{
  using mirror_type = nenya::mirror_type<Rep, Tag>;

  size_t operator()(const mirror_type& key) const noexcept
  {
    return std::hash<Rep>{}(key.get());
  }
};

}  // namespace std

// clang-format on