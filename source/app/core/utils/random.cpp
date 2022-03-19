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

#include "random.hpp"

#include <algorithm>   // generate
#include <array>       // array
#include <functional>  // ref
#include <random>      // random_device, seed_seq

namespace tactile {

auto make_random_engine() -> RandomEngine
{
  using result_type = std::random_device::result_type;

  constexpr auto n = RandomEngine::state_size;

  std::random_device device;
  std::array<result_type, (n - 1) / sizeof(result_type) + 1> data{};

  std::generate(data.begin(), data.end(), std::ref(device));
  std::seed_seq seeds(data.begin(), data.end());

  return RandomEngine(seeds);
}

auto next_bool() -> bool
{
  return next_random(0, 100) <= 50;
}

auto next_float() -> float
{
  return next_random(0.0f, 1.0f) <= 0.5f;
}

}  // namespace tactile