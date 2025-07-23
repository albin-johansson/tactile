// Copyright (C) 2025 Albin Johansson

module;

#include <exception>  // Needed for boost::safe_numerics

#include <boost/safe_numerics/checked_integer.hpp>

export module tactile.core.numeric:checked;

export import tactile.core.common;
export import :concepts;

namespace tactile {

[[nodiscard]]
constexpr auto to_error(
    const boost::safe_numerics::safe_numerics_error error) noexcept -> Error
{
  using enum boost::safe_numerics::safe_numerics_error;
  switch (error) {
    case positive_overflow_error:
      return Error::kArithmeticOverflow;
    case negative_overflow_error:
      [[fallthrough]];
    case underflow_error:
      return Error::kArithmeticUnderflow;
    case precision_overflow_error:
      return Error::kArithmeticPrecision;
    case negative_value_shift:
      [[fallthrough]];
    case negative_shift:
      [[fallthrough]];
    case shift_too_large:
      return Error::kInvalidOp;
    case range_error:
      [[fallthrough]];
    case domain_error:
      [[fallthrough]];
    case uninitialized_value:
      return Error::kArithmeticInvalidValue;
    case success:
      [[fallthrough]];
    default:
      return Error::kUnknown;
  }
}

}  // namespace tactile

export namespace tactile {

/// Performs checked addition of two numerical values.
template <Numeric T>
[[nodiscard]] constexpr auto checked_add(const T lhs, const T rhs) noexcept
    -> Result<T>
{
  const auto result = boost::safe_numerics::checked::add(lhs, rhs);

  if (result.exception()) [[unlikely]] {
    return err(to_error(result.m_e));
  }

  return result.m_contents.m_r;
}

/// Performs checked subtraction of two numerical values.
template <Numeric T>
[[nodiscard]] constexpr auto checked_sub(const T lhs, const T rhs) noexcept
    -> Result<T>
{
  const auto result = boost::safe_numerics::checked::subtract(lhs, rhs);

  if (result.exception()) [[unlikely]] {
    return err(to_error(result.m_e));
  }

  return result.m_contents.m_r;
}

/// Performs checked multiplication of two numerical values.
template <Numeric T>
[[nodiscard]] constexpr auto checked_mul(const T lhs, const T rhs) noexcept
    -> Result<T>
{
  const auto result = boost::safe_numerics::checked::multiply(lhs, rhs);

  if (result.exception()) [[unlikely]] {
    return err(to_error(result.m_e));
  }

  return result.m_contents.m_r;
}

/// Performs checked division of two numerical values.
template <Numeric T>
[[nodiscard]] constexpr auto checked_div(const T lhs, const T rhs) noexcept
    -> Result<T>
{
  const auto result = boost::safe_numerics::checked::divide(lhs, rhs);

  if (result.exception()) [[unlikely]] {
    return err(to_error(result.m_e));
  }

  return result.m_contents.m_r;
}

}  // namespace tactile
