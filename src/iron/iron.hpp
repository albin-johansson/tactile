#pragma once

#include <concepts>
#include <type_traits>
#include <utility>  // move

namespace iron {

// clang-format off

template <typename T>
concept PreIncrementable = requires (T t) {
  { ++t } -> std::convertible_to<T>;
};

template <typename T>
concept PostIncrementable = requires (T t) {
  { t++ } -> std::convertible_to<T>;
};

template <typename T>
concept Addable = requires (T t) {
  { t + t } -> std::convertible_to<T>;
};

template <typename T>
concept AdditionAssignable = requires (T t) {
  { t += t };
};

template <typename T>
concept UnaryPlus = requires (T t) {
  { +t } -> std::convertible_to<T>;
};

template <typename T>
concept Subtraction = requires (T t) {
  { t - t } -> std::convertible_to<T>;
};

template <typename T>
concept SubtractionAssigment = requires (T t) {
  { t -= t };
};

template <typename T>
concept UnaryMinus = requires (T t) {
  { -t } -> std::convertible_to<T>;
};

template <typename T>
concept Multiplication = requires (T t) {
  { t * t } -> std::convertible_to<T>;
};

template <typename T>
concept MultiplicationAssignment = requires (T t) {
  { t *= t };
};

template <typename T>
concept Division = requires (T t) {
  { t / t } -> std::convertible_to<T>;
};

template <typename T>
concept DivisionAssignment = requires (T t) {
  { t /= t };
};

template <typename T>
concept Modulo = requires (T t) {
  { t % t } -> std::convertible_to<T>;
};

template <typename T>
concept ModuloAssignment = requires (T t) {
  { t %= t };
};

template <typename T>
concept BitwiseNOT = requires (T t) {
  { ~t } -> std::convertible_to<T>;
};

template <typename T>
concept BitwiseAND = requires (T t) {
  { t & t } -> std::convertible_to<T>;
};

template <typename T>
concept BitwiseANDAssignment = requires (T t) {
  { t &= t };
};

template <typename T>
concept BitwiseOR = requires (T t) {
  { t | t } -> std::convertible_to<T>;
};

template <typename T>
concept BitwiseORAssignment = requires (T t) {
  { t |= t };
};

template <typename T>
concept XOR = requires (T t) {
  { t ^ t } -> std::convertible_to<T>;
};

template <typename T>
concept XORAssignment = requires (T t) {
  { t ^= t };
};

template <typename T>
concept LeftShift = requires (T t) {
  { t << t } -> std::convertible_to<T>;
};

template <typename T>
concept LeftShiftAssignment = requires (T t) {
  { t <<= t };
};

template <typename T>
concept RightShift = requires (T t) {
  { t >> t } -> std::convertible_to<T>;
};

template <typename T>
concept RightShiftAssignment = requires (T t) {
  { t >>= t };
};

template <typename T>
concept LessThan = requires (T t) {
  { t < t } -> std::convertible_to<bool>;
};

template <typename T>
concept LessThanOrEqual = requires (T t) {
  { t <= t } -> std::convertible_to<bool>;
};

template <typename T>
concept GreaterThan = requires (T t) {
  { t > t } -> std::convertible_to<bool>;
};

template <typename T>
concept GreaterThanOrEqual = requires (T t) {
  { t >= t } -> std::convertible_to<bool>;
};

template <typename T>
concept LogicalNegation = requires (T t) {
  { !t } -> std::convertible_to<bool>;
};

// clang-format on

/**
 * @class mirror_type
 *
 * @brief A strong type that automatically inherits all of the properties of the
 * representation type.
 *
 * @tparam Rep the representation type, e.g `int` or `std::string`.
 * @tparam Tag the tag type that uniquely identifies the type.
 *
 * @headerfile iron.hpp
 */
template <typename Rep, typename Tag>
class mirror_type
{
 public:
  using arg = const mirror_type&;

  constexpr mirror_type() requires std::is_default_constructible_v<Rep> =
  default;

  constexpr explicit mirror_type(const Rep& value) : m_value{value} {}

  /// @name Unary operators
  /// @{

  auto operator++() requires PreIncrementable<Rep>
  {
    ++m_value;
    return *this;
  }

  auto operator++(int) -> mirror_type requires PostIncrementable<Rep>
  {
    const auto tmp = m_value;
    ++m_value;
    return tmp;
  }

  [[nodiscard]] constexpr auto operator+() const requires UnaryPlus<Rep>
  {
    return mirror_type{+m_value};
  }

  [[nodiscard]] constexpr auto operator-() const requires UnaryMinus<Rep>
  {
    return mirror_type{-m_value};
  }

  [[nodiscard]] constexpr auto operator~() const requires BitwiseNOT<Rep>
  {
    return mirror_type{~m_value};
  }

  [[nodiscard]] constexpr auto operator!() const requires LogicalNegation<Rep>
  {
    return !m_value;
  }

  /// @} // end of unary operators

  /// @name Arithmetic operators
  /// @{

  [[nodiscard]] constexpr auto operator+(arg rhs) const requires Addable<Rep>
  {
    return mirror_type{m_value + rhs.get()};
  }

  [[nodiscard]] constexpr auto operator-(arg rhs) const
  requires Subtraction<Rep>
  {
    return mirror_type{m_value - rhs.get()};
  }

  [[nodiscard]] constexpr auto operator/(arg rhs) const requires Division<Rep>
  {
    return mirror_type{m_value / rhs.get()};
  }

  [[nodiscard]] constexpr auto operator*(arg rhs) const
  requires Multiplication<Rep>
  {
    return mirror_type{m_value * rhs.get()};
  }

  [[nodiscard]] constexpr auto operator%(arg rhs) const requires Modulo<Rep>
  {
    return mirror_type{m_value % rhs.get()};
  }

  [[nodiscard]] constexpr auto operator&(arg rhs) const requires BitwiseAND<Rep>
  {
    return mirror_type{m_value & rhs.get()};
  }

  [[nodiscard]] constexpr auto operator|(arg rhs) const requires BitwiseOR<Rep>
  {
    return mirror_type{m_value | rhs.get()};
  }

  [[nodiscard]] constexpr auto operator^(arg rhs) const requires XOR<Rep>
  {
    return mirror_type{m_value ^ rhs.get()};
  }

  [[nodiscard]] constexpr auto operator<<(arg rhs) const requires LeftShift<Rep>
  {
    return mirror_type{m_value << rhs.get()};
  }

  [[nodiscard]] constexpr auto operator>>(arg rhs) const
  requires RightShift<Rep>
  {
    return mirror_type{m_value >> rhs.get()};
  }

  /// @} // end of arithmetic operators

  /// @name Assignment operators
  /// @{

  constexpr auto operator+=(arg rhs) requires AdditionAssignable<Rep>
  {
    m_value += rhs.m_value;
    return *this;
  }

  constexpr auto operator-=(arg rhs) requires SubtractionAssigment<Rep>
  {
    m_value -= rhs.m_value;
    return *this;
  }

  constexpr auto operator/=(arg rhs) requires DivisionAssignment<Rep>
  {
    m_value /= rhs.m_value;
    return *this;
  }

  constexpr auto operator*=(arg rhs) requires MultiplicationAssignment<Rep>
  {
    m_value *= rhs.m_value;
    return *this;
  }

  constexpr auto operator%=(arg rhs) requires ModuloAssignment<Rep>
  {
    m_value %= rhs.m_value;
    return *this;
  }

  constexpr auto operator&=(arg rhs) requires BitwiseANDAssignment<Rep>
  {
    m_value &= rhs.m_value;
    return *this;
  }

  constexpr auto operator|=(arg rhs) requires BitwiseORAssignment<Rep>
  {
    m_value |= rhs.m_value;
    return *this;
  }

  constexpr auto operator^=(arg rhs) requires XORAssignment<Rep>
  {
    m_value ^= rhs.m_value;
    return *this;
  }

  constexpr auto operator<<=(arg rhs) requires LeftShiftAssignment<Rep>
  {
    m_value <<= rhs.m_value;
    return *this;
  }

  constexpr auto operator>>=(arg rhs) requires RightShiftAssignment<Rep>
  {
    m_value >>= rhs.m_value;
    return *this;
  }

  /// @} // end of assignment operators

  /// @name Comparison operators
  /// @{

  [[nodiscard]] constexpr auto operator==(arg rhs) const
  requires std::equality_comparable<Rep>
  {
    return m_value == rhs.m_value;
  }

  [[nodiscard]] constexpr auto operator!=(arg rhs) const
  requires std::equality_comparable<Rep>
  {
    return m_value != rhs.m_value;
  }

  [[nodiscard]] constexpr auto operator<(arg rhs) const requires LessThan<Rep>
  {
    return m_value < rhs.m_value;
  }

  [[nodiscard]] constexpr auto operator<=(arg rhs) const
  requires LessThanOrEqual<Rep>
  {
    return m_value <= rhs.m_value;
  }

  [[nodiscard]] constexpr auto operator>(arg rhs) const
  requires GreaterThan<Rep>
  {
    return m_value > rhs.m_value;
  }

  [[nodiscard]] constexpr auto operator>=(arg rhs) const
  requires GreaterThanOrEqual<Rep>
  {
    return m_value >= rhs.m_value;
  }

  /// @} end of comparison operators

  [[nodiscard]] auto get() -> Rep& { return m_value; }

  [[nodiscard]] auto get() const -> const Rep& { return m_value; }

  // TODO: operator->, operator*, operator&&, operator||, operator[]

 private:
  Rep m_value;
};
