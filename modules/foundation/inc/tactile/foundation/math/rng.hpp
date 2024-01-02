// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Forces initialization of the RNG engine for the current thread.
 */
TACTILE_FOUNDATION_API void initialize_rng();

/**
 * Returns a pseudo-random uint32 value.
 *
 * \param min The minimum possible value.
 * \param max The maximum possible value.
 *
 * \return
 *    A pseudo-random value in the interval [min, max].
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto next_random_u32(uint32 min, uint32 max) -> uint32;

/**
 * Returns a pseudo-random uint64 value.
 *
 * \param min The minimum possible value.
 * \param max The maximum possible value.
 *
 * \return
 *    A pseudo-random value in the interval [min, max].
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto next_random_u64(uint64 min, uint64 max) -> uint64;

/**
 * Returns a pseudo-random int32 value.
 *
 * \param min The minimum possible value.
 * \param max The maximum possible value.
 *
 * \return
 *    A pseudo-random value in the interval [min, max].
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto next_random_i32(int32 min, int32 max) -> int32;

/**
 * Returns a pseudo-random int64 value.
 *
 * \param min The minimum possible value.
 * \param max The maximum possible value.
 *
 * \return
 *    A pseudo-random value in the interval [min, max].
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto next_random_i64(int64 min, int64 max) -> int64;

/**
 * Returns a pseudo-random float32 value.
 *
 * \param min The minimum possible value.
 * \param max The maximum possible value.
 *
 * \return
 *    A pseudo-random value in the interval [min, max].
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto next_random_f32(float32 min, float32 max) -> float32;

/**
 * Returns a pseudo-random boolean value.
 *
 * \return
 *    True or false.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto next_random_bool() -> bool;

/**
 * Returns a pseudo-random float value.
 *
 * \return
 *    A float in the interval [0, 1].
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto next_random_normalized_float() -> float;

}  // namespace tactile
