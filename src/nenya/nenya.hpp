#pragma once

#include <compare>
#include <concepts>
#include <functional>  // hash
#include <type_traits>
#include <utility>  // move

namespace nenya {

// clang-format off

template <typename T>
concept PreIncrementable = requires(T t)
{
  { ++t } -> std::convertible_to<T>;
};

template <typename T>
concept PostIncrementable = requires(T t)
{
  { t++ } -> std::convertible_to<T>;
};

template <typename T>
concept Addable = requires(T t)
{
  { t + t } -> std::convertible_to<T>;
};

template <typename T>
concept AdditionAssignable = requires(T t)
{
  { t += t };
};

template <typename T>
concept UnaryPlus = requires(T t)
{
  { +t } -> std::convertible_to<T>;
};

template <typename T>
concept Subtraction = requires(T t)
{
  { t - t } -> std::convertible_to<T>;
};

template <typename T>
concept SubtractionAssigment = requires(T t)
{
  { t -= t };
};

template <typename T>
concept UnaryMinus = requires(T t)
{
  { -t } -> std::convertible_to<T>;
};

template <typename T>
concept Multiplication = requires(T t)
{
  { t * t } -> std::convertible_to<T>;
};

template <typename T>
concept MultiplicationAssignment = requires(T t)
{
  { t *= t };
};

template <typename T>
concept Division = requires(T t)
{
  { t / t } -> std::convertible_to<T>;
};

template <typename T>
concept DivisionAssignment = requires(T t)
{
  { t /= t };
};

template <typename T>
concept Modulo = requires(T t)
{
  { t % t } -> std::convertible_to<T>;
};

template <typename T>
concept ModuloAssignment = requires(T t)
{
  { t %= t };
};

template <typename T>
concept BitwiseNOT = requires(T t)
{
  { ~t } -> std::convertible_to<T>;
};

template <typename T>
concept BitwiseAND = requires(T t)
{
  { t& t } -> std::convertible_to<T>;
};

template <typename T>
concept BitwiseANDAssignment = requires(T t)
{
  { t &= t };
};

template <typename T>
concept BitwiseOR = requires(T t)
{
  { t | t } -> std::convertible_to<T>;
};

template <typename T>
concept BitwiseORAssignment = requires(T t)
{
  { t |= t };
};

template <typename T>
concept XOR = requires(T t)
{
  { t ^ t } -> std::convertible_to<T>;
};

template <typename T>
concept XORAssignment = requires(T t)
{
  { t ^= t };
};

template <typename T>
concept LeftShift = requires(T t)
{
  { t << t } -> std::convertible_to<T>;
};

template <typename T>
concept LeftShiftAssignment = requires(T t)
{
  { t <<= t };
};

template <typename T>
concept RightShift = requires(T t)
{
  { t >> t } -> std::convertible_to<T>;
};

template <typename T>
concept RightShiftAssignment = requires(T t)
{
  { t >>= t };
};

template <typename T>
concept LessThan = requires(T t)
{
  { t < t } -> std::convertible_to<bool>;
};

template <typename T>
concept LessThanOrEqual = requires(T t)
{
  { t <= t } -> std::convertible_to<bool>;
};

template <typename T>
concept GreaterThan = requires(T t)
{
  { t > t } -> std::convertible_to<bool>;
};

template <typename T>
concept GreaterThanOrEqual = requires(T t)
{
  { t >= t } -> std::convertible_to<bool>;
};

template <typename T>
concept LogicalNegation = requires(T t)
{
  { !t } -> std::convertible_to<bool>;
};

template<typename T>
concept Hashable = requires(T a) {
  { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
  noexcept(std::hash<T>{}(a));
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
 * @headerfile nenya.hpp
 */
template <typename Rep, typename Tag>
class mirror_type
{
  // clang-format off

  inline static constexpr bool nothrowCopy = std::is_nothrow_copy_constructible_v<Rep>;
  inline static constexpr bool nothrowMove = std::is_nothrow_copy_constructible_v<Rep>;

 public:
  constexpr mirror_type() = default;

  constexpr explicit mirror_type(const Rep& value) noexcept(nothrowCopy)
      : m_value{value}
  {}

  constexpr explicit mirror_type(Rep&& value) noexcept(nothrowMove)
      : m_value{std::move(value)}
  {}

  /// @name Unary operators
  /// @{

  auto operator++() noexcept(noexcept(++m_value)) requires PreIncrementable<Rep>
  {
    ++m_value;
    return *this;
  }

  auto operator++(int) noexcept(noexcept(m_value++)) -> mirror_type
      requires PostIncrementable<Rep>
  {
    return mirror_type{m_value++};
  }

  [[nodiscard]]
  constexpr auto operator+() const noexcept(noexcept(+m_value))
      requires UnaryPlus<Rep>
  {
    return mirror_type{+m_value};
  }

  [[nodiscard]]
  constexpr auto operator-() const noexcept(noexcept(-m_value))
      requires UnaryMinus<Rep>
  {
    return mirror_type{-m_value};
  }

  [[nodiscard]]
  constexpr auto operator~() const noexcept(noexcept(~m_value))
      requires BitwiseNOT<Rep>
  {
    return mirror_type{~m_value};
  }

  [[nodiscard]]
  constexpr auto operator!() const noexcept(noexcept(!m_value))
      requires LogicalNegation<Rep>
  {
    return !m_value;
  }

  /// @} // end of unary operators

  /// @name Arithmetic operators
  /// @{

  [[nodiscard]]
  constexpr auto operator+(const mirror_type& rhs) const
      noexcept(noexcept(m_value + rhs.m_value))
      requires Addable<Rep>
  {
    return mirror_type{m_value + rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator-(const mirror_type& rhs) const
      noexcept(noexcept(m_value - rhs.m_value))
      requires Subtraction<Rep>
  {
    return mirror_type{m_value - rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator/(const mirror_type& rhs) const
      noexcept(noexcept(m_value / rhs.m_value))
      requires Division<Rep>
  {
    return mirror_type{m_value / rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator*(const mirror_type& rhs) const
      noexcept(noexcept(m_value * rhs.m_value))
      requires Multiplication<Rep>
  {
    return mirror_type{m_value * rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator%(const mirror_type& rhs) const
      noexcept(noexcept(m_value % rhs.m_value))
      requires Modulo<Rep>
  {
    return mirror_type{m_value % rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator&(const mirror_type& rhs) const
      noexcept(noexcept(m_value & rhs.m_value))
      requires BitwiseAND<Rep>
  {
    return mirror_type{m_value & rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator|(const mirror_type& rhs) const
      noexcept(noexcept(m_value | rhs.m_value))
      requires BitwiseOR<Rep>
  {
    return mirror_type{m_value | rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator^(const mirror_type& rhs) const
      noexcept(noexcept(m_value ^ rhs.m_value))
      requires XOR<Rep>
  {
    return mirror_type{m_value ^ rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator<<(const mirror_type& rhs) const
      noexcept(noexcept(m_value << rhs.m_value))
      requires LeftShift<Rep>
  {
    return mirror_type{m_value << rhs.m_value};
  }

  [[nodiscard]]
  constexpr auto operator>>(const mirror_type& rhs) const
      noexcept(noexcept(m_value >> rhs.m_value))
      requires RightShift<Rep>
  {
    return mirror_type{m_value >> rhs.m_value};
  }

  /// @} // end of arithmetic operators

  /// @name Assignment operators
  /// @{

  constexpr auto operator+=(const mirror_type& rhs) noexcept(noexcept(m_value += rhs.m_value))
      requires AdditionAssignable<Rep>
  {
    m_value += rhs.m_value;
    return *this;
  }

  constexpr auto operator-=(const mirror_type& rhs) noexcept(noexcept(m_value -= rhs.m_value))
      requires SubtractionAssigment<Rep>
  {
    m_value -= rhs.m_value;
    return *this;
  }

  constexpr auto operator/=(const mirror_type& rhs) noexcept(noexcept(m_value /= rhs.m_value))
      requires DivisionAssignment<Rep>
  {
    m_value /= rhs.m_value;
    return *this;
  }

  constexpr auto operator*=(const mirror_type& rhs) noexcept(noexcept(m_value *= rhs.m_value))
      requires MultiplicationAssignment<Rep>
  {
    m_value *= rhs.m_value;
    return *this;
  }

  constexpr auto operator%=(const mirror_type& rhs) noexcept(noexcept(m_value %= rhs.m_value))
      requires ModuloAssignment<Rep>
  {
    m_value %= rhs.m_value;
    return *this;
  }

  constexpr auto operator&=(const mirror_type& rhs) noexcept(noexcept(m_value &= rhs.m_value))
      requires BitwiseANDAssignment<Rep>
  {
    m_value &= rhs.m_value;
    return *this;
  }

  constexpr auto operator|=(const mirror_type& rhs) noexcept(noexcept(m_value |= rhs.m_value))
      requires BitwiseORAssignment<Rep>
  {
    m_value |= rhs.m_value;
    return *this;
  }

  constexpr auto operator^=(const mirror_type& rhs) noexcept(noexcept(m_value ^= rhs.m_value))
      requires XORAssignment<Rep>
  {
    m_value ^= rhs.m_value;
    return *this;
  }

  constexpr auto operator<<=(const mirror_type& rhs) noexcept(noexcept(m_value <<= rhs.m_value))
      requires LeftShiftAssignment<Rep>
  {
    m_value <<= rhs.m_value;
    return *this;
  }

  constexpr auto operator>>=(const mirror_type& rhs) noexcept(noexcept(m_value >>= rhs.m_value))
      requires RightShiftAssignment<Rep>
  {
    m_value >>= rhs.m_value;
    return *this;
  }

  /// @} // end of assignment operators

  /// @name Comparison operators
  /// @{

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
      requires LessThan<Rep>
  {
    return m_value < rhs.m_value;
  }

  [[nodiscard]]
  constexpr auto operator<=(const mirror_type& rhs) const
      noexcept(noexcept(m_value <= rhs.m_value))
      requires LessThanOrEqual<Rep>
  {
    return m_value <= rhs.m_value;
  }

  [[nodiscard]]
  constexpr auto operator>(const mirror_type& rhs) const
      noexcept(noexcept(m_value > rhs.m_value))
      requires GreaterThan<Rep>
  {
    return m_value > rhs.m_value;
  }

  [[nodiscard]]
  constexpr auto operator>=(const mirror_type& rhs) const
      noexcept(noexcept(m_value >= rhs.m_value))
      requires GreaterThanOrEqual<Rep>
  {
    return m_value >= rhs.m_value;
  }

  // clang-format on

  /// @} end of comparison operators

  [[nodiscard]] auto get() -> Rep& { return m_value; }

  [[nodiscard]] auto get() const -> const Rep& { return m_value; }

  // TODO: operator->, operator*, operator&&, operator||, operator[]

 private:
  Rep m_value{};
};

}  // namespace nenya

// clang-format off

namespace std {

template <typename Rep, typename Tag> requires nenya::Hashable<Rep>
struct hash<::nenya::mirror_type<Rep, Tag>>
{
  using mirror_type = ::nenya::mirror_type<Rep, Tag>;

  auto operator()(const mirror_type& key) const noexcept -> size_t
  {
    return std::hash<Rep>{}(key.get());
  }
};

}  // namespace std

// clang-format on