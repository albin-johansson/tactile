// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

module tactile.numeric;

import std;
import tactile.core;

namespace tactile {
namespace {

using RandomEngine = std::mt19937;

[[nodiscard]]
auto _make_random_engine() -> RandomEngine
{
  std::random_device entropy_source {};
  const auto seed = entropy_source();
  return RandomEngine {seed};
}

[[nodiscard]]
auto _get_random_engine() -> RandomEngine&
{
  thread_local auto engine = _make_random_engine();
  return engine;
}

}  // namespace

void random_init()
{
  (void) _get_random_engine();
}

auto random_i32(const i32 min, const i32 max) -> i32
{
  auto& engine = _get_random_engine();
  return std::uniform_int_distribution {min, max}(engine);
}

auto random_u32(const u32 min, const u32 max) -> u32
{
  auto& engine = _get_random_engine();
  return std::uniform_int_distribution {min, max}(engine);
}

auto random_f32(const f32 min, const f32 max) -> f32
{
  auto& engine = _get_random_engine();
  return std::uniform_real_distribution {min, max}(engine);
}

auto random_bool() -> bool
{
  return random_i32(0, 1) == 1;
}

}  // namespace tactile
