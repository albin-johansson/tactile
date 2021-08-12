#ifndef RUNE_MATH_RANDOM_UTILS_HPP
#define RUNE_MATH_RANDOM_UTILS_HPP

#include <algorithm>   // generate
#include <array>       // array
#include <concepts>    // floating_point, integral
#include <functional>  // ref
#include <random>      // mt19937, random_device, uniform_int_distribution, ...

namespace rune {

/// \addtogroup math
/// \{

/// \name Random number generation
/// \{

/// \brief The random engine type used by the RNG functions.
using random_engine = std::mt19937;

/**
 * \brief Creates and returns a seeded random engine.
 *
 * \return a seeded random engine.
 */
[[nodiscard]] inline auto make_random_engine() -> random_engine
{
  using result_type = std::random_device::result_type;

  constexpr auto n = random_engine::state_size;

  std::random_device device;
  std::array<result_type, (n - 1) / sizeof(result_type) + 1> data{};

  std::ranges::generate(data, std::ref(device));
  std::seed_seq seeds(data.begin(), data.end());

  return random_engine(seeds);
}

/**
 * \brief Returns a random value in the specified range [min, max].
 *
 * \tparam T the type of the value to be obtained.
 *
 * \param min the minimum possible value.
 * \param max the maximum possible value.
 *
 * \return a random value in the specified range.
 */
template <typename T>
  requires std::integral<T> || std::floating_point<T>
[[nodiscard]] auto next_random(const T min, const T max) -> T
{
  static auto engine = make_random_engine();

  if constexpr (std::floating_point<T>)
  {
    return std::uniform_real_distribution<T>{min, max}(engine);
  }
  else
  {
    return std::uniform_int_distribution<T>{min, max}(engine);
  }
}

/**
 * \brief Returns a random `bool` value.
 *
 * \return a random `bool` value.
 *
 * \see `next_random()`
 */
[[nodiscard]] inline auto next_bool() -> bool
{
  return next_random(0.0, 1.0) > 0.5;
}

/**
 * \brief Returns a random `float` in the interval [0, 1].
 *
 * \return a random `float` value.
 *
 * \see `next_random()`
 */
[[nodiscard]] inline auto next_float() -> float
{
  return next_random(0.0f, 1.0f);
}

/**
 * \brief Returns a random `double` in the interval [0, 1].
 *
 * \return a random `double` value.
 *
 * \see `next_random()`
 */
[[nodiscard]] inline auto next_double() -> double
{
  return next_random(0.0, 1.0);
}

/// \} End of random number generation

/// \} End of group math

}  // namespace rune

#endif  // RUNE_MATH_RANDOM_UTILS_HPP
