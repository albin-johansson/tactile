// Copyright (C) 2025 Albin Johansson

export module tactile.core.math:arithmetic;

import tactile.core.ext.boost.safe_numerics;
import tactile.core.common;
import tactile.core.numeric;
import tactile.core.error;
import tactile.core.util;

namespace tactile {

[[nodiscard]]
constexpr auto to_error(
    const boost::safe_numerics::safe_numerics_error error) noexcept -> Error
{
  using Err = boost::safe_numerics::safe_numerics_error;
  switch (error) {
    case Err::positive_overflow_error:
      return Error::kArithmeticOverflow;

    case Err::negative_overflow_error:
    case Err::underflow_error:
      return Error::kArithmeticUnderflow;

    case Err::precision_overflow_error:
      return Error::kArithmeticPrecision;

    case Err::negative_value_shift:
    case Err::negative_shift:
    case Err::shift_too_large:
      return Error::kInvalidOp;

    case Err::range_error:
    case Err::domain_error:
    case Err::uninitialized_value:
      return Error::kArithmeticInvalidValue;

    case Err::success:
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
