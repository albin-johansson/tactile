// Copyright (C) 2025 Albin Johansson

export module tactile.numeric:constants;

export import std;
export import tactile.common;

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

}  // namespace tactile
