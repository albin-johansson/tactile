// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/math/rng.hpp"

#include <concepts>  // same_as, floating_point
#include <random>  // mt19937, random_device, uniform_real_distribution, uniform_int_distribution
#include <thread>  // this_thread

#include <fmt/ostream.h>

#include "tactile/core/debug/assert.hpp"
#include "tactile/core/debug/log/logger.hpp"

namespace tactile {
namespace {

using RandomEngine = std::mt19937;

/// Creates a seeded pseudo-random number generation engine.
[[nodiscard]] auto _make_random_engine() -> RandomEngine
{
  std::random_device entropy_source;
  const auto seed = entropy_source();

  TACTILE_LOG_DEBUG("Thread {} uses RNG seed {}",
                    fmt::streamed(std::this_thread::get_id()),
                    seed);

  return RandomEngine {seed};
}

[[nodiscard]] auto _get_random_engine() -> RandomEngine&
{
  thread_local static auto engine = _make_random_engine();
  return engine;
}

/// Returns a random value in the range [min, max].
template <typename T>
[[nodiscard]] auto _next_random(const T min, const T max) -> T
{
  static_assert(!std::same_as<T, bool>);
  static_assert(!std::same_as<T, char>);
  static_assert(!std::same_as<T, uchar>);

  TACTILE_ASSERT(min <= max);
  auto& engine = _get_random_engine();

  if constexpr (std::floating_point<T>) {
    return std::uniform_real_distribution<T> {min, max}(engine);
  }
  else {
    return std::uniform_int_distribution<T> {min, max}(engine);
  }
}

}  // namespace

void rng_init()
{
  (void) _get_random_engine();
}

auto next_random_u32(const uint32 min, const uint32 max) -> uint32
{
  return _next_random(min, max);
}

auto next_random_u64(const uint64 min, const uint64 max) -> uint64
{
  return _next_random(min, max);
}

auto next_random_i32(const int32 min, const int32 max) -> int32
{
  return _next_random(min, max);
}

auto next_random_i64(const int64 min, const int64 max) -> int64
{
  return _next_random(min, max);
}

auto next_random_f32(const float32 min, const float32 max) -> float32
{
  return _next_random(min, max);
}

auto next_bool() -> bool
{
  return _next_random(0, 99) < 50;
}

auto next_float() -> float
{
  return next_random_f32(0.0f, 1.0f);
}

}  // namespace tactile