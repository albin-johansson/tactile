// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

namespace tactile::core {

/**
 * Forces initialization of the RNG engine for the current thread.
 */
void init_random_number_generator();

/**
 * Returns a pseudo-random signed integer.
 *
 * \param min The minimum possible value.
 * \param max The maximum possible value.
 *
 * \return
 * A pseudo-random value in the interval [min, max].
 */
[[nodiscard]]
auto get_random_int(int min, int max) -> int;

/**
 * Returns a pseudo-random unsigned integer.
 *
 * \param min The minimum possible value.
 * \param max The maximum possible value.
 *
 * \return
 * A pseudo-random value in the interval [min, max].
 */
[[nodiscard]]
auto get_random_uint(unsigned min, unsigned max) -> unsigned;

/**
 * Returns a pseudo-random float.
 *
 * \param min The minimum possible value.
 * \param max The maximum possible value.
 *
 * \return
 * A pseudo-random value in the interval [min, max].
 */
[[nodiscard]]
auto get_random_float(float min, float max) -> float;

/**
 * Returns a pseudo-random float.
 *
 * \return
 * A float in the interval [0, 1].
 */
[[nodiscard]]
auto get_random_float_normalized() -> float;

/**
 * Returns a pseudo-random boolean value.
 *
 * \return
 * True or false.
 */
[[nodiscard]]
auto get_random_bool() -> bool;

}  // namespace tactile::core
