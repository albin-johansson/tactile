// Copyright (C) 2025 Albin Johansson

export module tactile.core.numeric:casts;

export import tactile.core.ext.std;

export namespace tactile {

/// Performs a checked narrowing conversion of an integral value.
///
/// Throws if the original value isn't representable using the destination type.
template <std::integral To, std::integral From>
  requires(sizeof(To) <= sizeof(From))
[[nodiscard]] constexpr auto checked_cast(const From from) -> To
{
  if constexpr (!std::same_as<From, To>) {
    if constexpr (std::signed_integral<From>) {
      if (std::cmp_less(from, std::numeric_limits<To>::min())) [[unlikely]] {
        throw std::underflow_error {
          "integral narrowing conversion would be lossy"};
      }
    }

    if (std::cmp_greater(from, std::numeric_limits<To>::max())) [[unlikely]] {
      throw std::overflow_error {
        "integral narrowing conversion would be lossy"};
    }
  }

  return static_cast<To>(from);
}

/// Performs a checked conversion of an unsigned integer to a signed integer.
template <std::unsigned_integral T>
[[nodiscard]] constexpr auto to_signed(const T value) -> std::make_signed_t<T>
{
  return checked_cast<std::make_signed_t<T>>(value);
}

/// Performs a checked conversion of a signed integer to an unsigned integer.
template <std::signed_integral T>
[[nodiscard]] constexpr auto to_unsigned(const T value)
    -> std::make_unsigned_t<T>
{
  return checked_cast<std::make_unsigned_t<T>>(value);
}

/// Performs a saturating narrowing conversion of an integral value.
template <std::integral To, std::integral From>
[[nodiscard]] constexpr auto saturate_cast(const From from) noexcept -> To
{
  constexpr auto kToMin = std::numeric_limits<To>::min();
  constexpr auto kToMax = std::numeric_limits<To>::max();

  if constexpr (!std::same_as<From, To>) {
    if constexpr (std::signed_integral<From>) {
      if (std::cmp_less(from, kToMin)) {
        return kToMin;
      }
    }

    if (std::cmp_greater(from, kToMax)) {
      return kToMax;
    }
  }

  return static_cast<To>(from);
}

}  // namespace tactile
