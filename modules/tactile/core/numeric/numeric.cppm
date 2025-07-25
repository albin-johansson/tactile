// Copyright (C) 2025 Albin Johansson

/// Provides common numeric APIs.
///
/// This module serves as a fundamental building block for other modules, and
/// must therefore only depend on the "tactile.common" module.
export module tactile.core.numeric;

export import :checked;
export import :concepts;
export import :vec;
import tactile.core.ext.std;
import tactile.core.common;

export namespace tactile {

inline constexpr auto kMinI8 = std::numeric_limits<i8>::min();
inline constexpr auto kMinI16 = std::numeric_limits<i16>::min();
inline constexpr auto kMinI32 = std::numeric_limits<i32>::min();
inline constexpr auto kMinI64 = std::numeric_limits<i64>::min();

inline constexpr auto kMaxI8 = std::numeric_limits<i8>::max();
inline constexpr auto kMaxI16 = std::numeric_limits<i16>::max();
inline constexpr auto kMaxI32 = std::numeric_limits<i32>::max();
inline constexpr auto kMaxI64 = std::numeric_limits<i64>::max();

// Provided for consistency, these are of course all zero.
inline constexpr auto kMinU8 = std::numeric_limits<u8>::min();
inline constexpr auto kMinU16 = std::numeric_limits<u16>::min();
inline constexpr auto kMinU32 = std::numeric_limits<u32>::min();
inline constexpr auto kMinU64 = std::numeric_limits<u64>::min();

inline constexpr auto kMaxU8 = std::numeric_limits<u8>::max();
inline constexpr auto kMaxU16 = std::numeric_limits<u16>::max();
inline constexpr auto kMaxU32 = std::numeric_limits<u32>::max();
inline constexpr auto kMaxU64 = std::numeric_limits<u64>::max();

/// Performs a checked narrowing conversion of an integral value.
///
/// Throws if the original value isn't representable using the destination type.
template <std::integral To, std::integral From>
  requires(sizeof(To) <= sizeof(From))
[[nodiscard]] constexpr auto checked_cast(const From from) -> To
{
  if (!std::in_range<To>(from)) {
    throw std::range_error {"Narrowing conversion would be lossy"};
  }

  return static_cast<To>(from);
}

/// Performs a checked conversion of an unsigned integer to a signed integer.
template <std::unsigned_integral T>
[[nodiscard]] constexpr auto signed_cast(const T value) -> std::make_signed_t<T>
{
  return checked_cast<std::make_signed_t<T>>(value);
}

/// Performs a checked conversion of a signed integer to an unsigned integer.
template <std::signed_integral T>
[[nodiscard]] constexpr auto unsigned_cast(const T value)
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

/// Hashes a value and combines the result with an existing hash value.
///
/// See https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0814r2.pdf
template <typename T>
constexpr void hash_combine(usize& seed, const T& value) noexcept
{
  const auto value_hash = std::hash<T> {}(value);
  seed ^= value_hash + 0x9E3779B9uz + (seed << 6uz) + (seed >> 2uz);
}

/// Hashes a generic collection of values.
[[nodiscard]]
constexpr auto hash_combine(const auto&... args) noexcept -> usize
{
  auto seed = 0uz;
  (hash_combine(seed, args), ...);
  return seed;
}

}  // namespace tactile
