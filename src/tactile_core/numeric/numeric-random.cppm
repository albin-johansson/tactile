// Copyright (C) 2025 Albin Johansson

export module tactile.numeric:random;

export import tactile.common;

export namespace tactile {

/// Forces initialization of the RNG engine for the current thread.
void random_init();

/// Returns a pseudo-random `i32` within the interval [min, max].
[[nodiscard]]
auto random_i32(i32 min, i32 max) -> i32;

/// Returns a pseudo-random `u32` within the interval [min, max].
[[nodiscard]]
auto random_u32(u32 min, u32 max) -> u32;

/// Returns a pseudo-random `f32` within the interval [min, max].
[[nodiscard]]
auto random_f32(f32 min, f32 max) -> f32;

/// Returns a pseudo-random boolean value.
[[nodiscard]]
auto random_bool() -> bool;

}  // namespace tactile
