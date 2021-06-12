#pragma once

#include <concepts>  // invocable

namespace tactile {

/**
 * \brief Invokes the supplied callable for the specified amount of times.
 *
 * \tparam T the type of the callable.
 *
 * \param n the amount of times the callable should be invoked.
 * \param callable the callable that will be invoked, must provide `operator()`.
 *
 * \since 0.1.0
 */
template <std::invocable T>
constexpr void InvokeN(const int n, T&& callable) noexcept(noexcept(callable()))
{
  for (auto i = 0; i < n; ++i)
  {
    callable();
  }
}

}  // namespace tactile
