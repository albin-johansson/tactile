/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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
#include <concepts>    // same_as, floating_point
#include <functional>  // ref
#include <random>  // mt19937, random_device, seed_seq, uniform_real_distribution, uniform_int_distribution

#include "common/debug/assert.hpp"
#include "common/type/array.hpp"

namespace tactile {
namespace {

using RandomEngine = std::mt19937;
using Seed = std::random_device::result_type;
using SeedArray = Array<Seed, RandomEngine::state_size>;

/// Creates a seeded pseudo-random number generation engine.
[[nodiscard]] auto make_random_engine() -> RandomEngine
{
  std::random_device device;

  SeedArray data;
  std::generate(data.begin(), data.end(), std::ref(device));

  std::seed_seq seeds(data.begin(), data.end());
  return RandomEngine {seeds};
}

[[nodiscard]] auto get_random_engine() -> RandomEngine&
{
  thread_local static auto engine = make_random_engine();
  return engine;
}

/// Returns a random value in the range [min, max].
template <typename T>
[[nodiscard]] auto next_random(const T min, const T max) -> T
{
  static_assert(!std::same_as<T, bool>);
  static_assert(!std::same_as<T, char>);
  static_assert(!std::same_as<T, uchar>);

  TACTILE_ASSERT(min <= max);
  auto& engine = get_random_engine();

  if constexpr (std::floating_point<T>) {
    return std::uniform_real_distribution<T> {min, max}(engine);
  }
  else {
    return std::uniform_int_distribution<T> {min, max}(engine);
  }
}

}  // namespace

auto next_random_u32(const uint32 min, const uint32 max) -> uint32
{
  return next_random(min, max);
}

auto next_random_u64(const uint64 min, const uint64 max) -> uint64
{
  return next_random(min, max);
}

auto next_random_i32(const int32 min, const int32 max) -> int32
{
  return next_random(min, max);
}

auto next_random_i64(const int64 min, const int64 max) -> int64
{
  return next_random(min, max);
}

auto next_random_f32(const float min, const float max) -> float
{
  return next_random(min, max);
}

auto next_bool() -> bool
{
  return next_random(0, 99) < 50;
}

}  // namespace tactile
