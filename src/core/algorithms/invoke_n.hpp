#pragma once

#include <concepts>  // invocable

#include "aliases/ints.hpp"

namespace Tactile {

/**
 * \brief Invokes the supplied callable for the specified amount of times.
 *
 * \tparam T the type of the callable.
 *
 * \param n the amount of times the callable should be invoked.
 * \param callable the callable that will be invoked, must provide `operator()`.
 */
template <std::invocable T>
constexpr void InvokeN(const usize n,
                       T&& callable) noexcept(noexcept(callable()))
{
  for (usize i = 0; i < n; ++i)
  {
    callable();
  }
}

}  // namespace Tactile
