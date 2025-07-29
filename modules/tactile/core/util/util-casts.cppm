// Copyright (C) 2025 Albin Johansson

export module tactile.core.util:casts;

import :concepts;
import tactile.core.ext.std;
import tactile.core.prelude;

export namespace tactile {

enum class RangeCheckResult : u8
{
  kInRange,
  kUnderflow,
  kOverflow,
};

/// Determine whether a value is within the range of another type.
template <Integer To, Integer From>
[[nodiscard]] constexpr auto range_check(const From from) noexcept
    -> RangeCheckResult
{
  if constexpr (std::same_as<From, To>) {
    return RangeCheckResult::kInRange;
  }

  // Only signed integer types can underflow another integer type.
  if constexpr (std::is_signed_v<From>) {
    if (std::cmp_less(from, std::numeric_limits<To>::min())) {
      return RangeCheckResult::kUnderflow;
    }
  }

  if (std::cmp_greater(from, std::numeric_limits<To>::max())) {
    return RangeCheckResult::kOverflow;
  }

  return RangeCheckResult::kInRange;
}

/// Performs a checked narrowing conversion of an integral value.
///
/// Throws if the original value isn't representable using the destination type.
template <Integer To, Integer From>
[[nodiscard]] constexpr auto checked_cast(const From from) -> To
{
  if (range_check<To>(from) != RangeCheckResult::kInRange) [[unlikely]] {
    throw std::range_error {"Narrowing conversion would be lossy"};
  }

  return static_cast<To>(from);
}

/// Performs a checked conversion of an unsigned integer to a signed integer.
template <UnsignedInteger T>
[[nodiscard]] constexpr auto signed_cast(const T value) -> std::make_signed_t<T>
{
  return checked_cast<std::make_signed_t<T>>(value);
}

/// Performs a checked conversion of a signed integer to an unsigned integer.
template <SignedInteger T>
[[nodiscard]] constexpr auto unsigned_cast(const T value)
    -> std::make_unsigned_t<T>
{
  return checked_cast<std::make_unsigned_t<T>>(value);
}

/// Performs a saturating narrowing conversion of an integral value.
template <Integer To, Integer From>
[[nodiscard]] constexpr auto saturate_cast(const From from) noexcept -> To
{
  switch (range_check<To>(from)) {
    case RangeCheckResult::kUnderflow:
      return std::numeric_limits<To>::min();

    case RangeCheckResult::kOverflow:
      return std::numeric_limits<To>::max();

    case RangeCheckResult::kInRange:
    default:
      return static_cast<To>(from);
  }
}

}  // namespace tactile
