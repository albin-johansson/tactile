// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/common/numeric/random.hpp"

#include <random>

namespace tactile::common {
namespace {

using RandomEngine = std::mt19937;

[[nodiscard]]
auto make_random_engine() -> RandomEngine
{
  std::random_device entropy_source {};
  const auto seed = entropy_source();
  return RandomEngine {seed};
}

[[nodiscard]]
auto get_random_engine() -> RandomEngine&
{
  thread_local auto engine = make_random_engine();
  return engine;
}

}  // namespace

void random_init()
{
  (void) get_random_engine();
}

auto random_int(const int min, const int max) -> int
{
  auto& engine = get_random_engine();
  return std::uniform_int_distribution {min, max}(engine);
}

auto random_uint(const unsigned min, const unsigned max) -> unsigned
{
  auto& engine = get_random_engine();
  return std::uniform_int_distribution {min, max}(engine);
}

auto random_double(const double min, const double max) -> double
{
  auto& engine = get_random_engine();
  return std::uniform_real_distribution {min, max}(engine);
}

auto random_bool() -> bool
{
  return random_int(0, 1) == 1;
}

}  // namespace tactile::common
