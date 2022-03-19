/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <random>       // mt19937, uniform_real_distribution, uniform_int_distribution
#include <type_traits>  // is_floating_point_v

#include "tactile.hpp"

namespace tactile {

using RandomEngine = std::mt19937;

/**
 * \brief Creates a seeded pseudo-random number generation engine.
 *
 * \return a seeded random engine.
 */
[[nodiscard]] auto make_random_engine() -> RandomEngine;

/**
 * \brief Returns a random value in the range [min, max].
 *
 * \param min the minimum possible value.
 * \param max the maximum possible value.
 *
 * \return a random value in the range.
 */
template <typename T>
[[nodiscard]] auto next_random(const T min, const T max) -> T
{
  static auto engine = make_random_engine();

  if constexpr (std::is_floating_point_v<T>) {
    return std::uniform_real_distribution<T>{min, max}(engine);
  }
  else {
    return std::uniform_int_distribution<T>{min, max}(engine);
  }
}

/**
 * \brief Returns a random boolean value.
 *
 * \return a random boolean value.
 */
[[nodiscard]] auto next_bool() -> bool;

/**
 * \brief Returns a random float in the interval [0, 1].
 *
 * \return a random normalized float.
 */
[[nodiscard]] auto next_float() -> float;

}  // namespace tactile
