// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/** \brief Forces initialization of the RNG engine for the current thread. */
TACTILE_CORE_API void rng_init();

/**
 * \brief Returns a pseudo-random uint32 value.
 *
 * \param min the minimum possible value.
 * \param max the maximum possible value.
 *
 * \return a pseudo-random value in the interval [min, max].
 */
[[nodiscard]]
TACTILE_CORE_API auto next_random_u32(uint32 min, uint32 max) -> uint32;

/**
 * \brief Returns a pseudo-random uint64 value.
 *
 * \param min the minimum possible value.
 * \param max the maximum possible value.
 *
 * \return a pseudo-random value in the interval [min, max].
 */
[[nodiscard]]
TACTILE_CORE_API auto next_random_u64(uint64 min, uint64 max) -> uint64;

/**
 * \brief Returns a pseudo-random int32 value.
 *
 * \param min the minimum possible value.
 * \param max the maximum possible value.
 *
 * \return a pseudo-random value in the interval [min, max].
 */
[[nodiscard]]
TACTILE_CORE_API auto next_random_i32(int32 min, int32 max) -> int32;

/**
 * \brief Returns a pseudo-random int64 value.
 *
 * \param min the minimum possible value.
 * \param max the maximum possible value.
 *
 * \return a pseudo-random value in the interval [min, max].
 */
[[nodiscard]]
TACTILE_CORE_API auto next_random_i64(int64 min, int64 max) -> int64;

/**
 * \brief Returns a pseudo-random float32 value.
 *
 * \param min the minimum possible value.
 * \param max the maximum possible value.
 *
 * \return a pseudo-random value in the interval [min, max].
 */
[[nodiscard]]
TACTILE_CORE_API auto next_random_f32(float32 min, float32 max) -> float32;

/**
 * \brief Returns a pseudo-random boolean value.
 *
 * \return true or false.
 */
[[nodiscard]]
TACTILE_CORE_API auto next_bool() -> bool;

/**
 * \brief Returns a pseudo-random float value.
 *
 * \return a float in the interval [0, 1].
 */
[[nodiscard]]
TACTILE_CORE_API auto next_normalized_float() -> float;

}  // namespace tactile
