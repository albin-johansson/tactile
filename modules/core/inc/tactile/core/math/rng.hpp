// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/** Forces initialization of the RNG engine for the current thread. */
TACTILE_CORE_API void rng_init();

[[nodiscard]] TACTILE_CORE_API auto next_random_u32(uint32 min, uint32 max) -> uint32;

[[nodiscard]] TACTILE_CORE_API auto next_random_u64(uint64 min, uint64 max) -> uint64;

[[nodiscard]] TACTILE_CORE_API auto next_random_i32(int32 min, int32 max) -> int32;

[[nodiscard]] TACTILE_CORE_API auto next_random_i64(int64 min, int64 max) -> int64;

[[nodiscard]] TACTILE_CORE_API auto next_random_f32(float32 min, float32 max) -> float32;

/// Returns a random boolean value.
[[nodiscard]] TACTILE_CORE_API auto next_bool() -> bool;

/// Returns a random float in the interval [0, 1].
[[nodiscard]] TACTILE_CORE_API auto next_float() -> float;

}  // namespace tactile
