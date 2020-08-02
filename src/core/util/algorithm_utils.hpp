#pragma once

#include <algorithm>

namespace tactile {

/**
 * @brief Applies a unary function to all of the elements in an iterable
 * container.
 *
 * @param iterable the object that provides the objects that will be iterated,
 * must provide `begin()` and `end()` member functions.
 * @param functor a unary function object.
 *
 * @since 0.1.0
 */
template <typename Iterable, typename Functor>
inline void for_all(Iterable iterable, Functor functor)
{
  std::for_each(iterable.begin(), iterable.end(), functor);
}

/**
 * @brief Invokes the supplied callable for the specified amount of times.
 *
 * @param nIterations the amount of times the callable should be invoked.
 * @param callable the callable that will be invoked, must overload the
 * ()-operator.
 *
 * @since 0.1.0
 */
template <typename Callable>
inline void repeat(int nIterations, Callable callable)
{
  for (auto i = 0; i < nIterations; ++i) {
    callable();
  }
}

}  // namespace tactile
