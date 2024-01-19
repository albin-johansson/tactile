// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>  // invocable

#include "tactile/foundation/debug/assert.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Utility for partially unrolling loops, processing actions in chunks of 4.
 *
 * \details
 *    This function can be used instead of regular for-loops. For example,
 *    consider the following loop.
 *      \code{cpp}
 *        for (int i = 0; i < 4; ++i) {
 *          do_something(i);
 *        }
 *      \endcode
 *    We can unroll the for-loop using this function as in the following snippet.
 *      \code{cpp}
 *       unroll4(0, 4, [](int index) {
 *         do_something(i);
 *       });
 *      \endcode
 *    The function signature resembles a conventional for-loop, where the first
 *    parameter is the initial index, and the second parameter is used to control
 *    the number of iterations. The index is automatically incremented using
 *    the postfix `++` operator.
 *
 * \pre The number of iterations must not be negative.
 *
 * \see Duff's device (https://en.wikipedia.org/wiki/Duff%27s_device)
 *
 * \tparam IndexType The type of the initial index.
 * \tparam Action    The type of the function object.
 *
 * \param index      The initial index value. Incremented after each call to `action`. Note, this can be negative.
 * \param iterations The number of iterations.
 * \param action     A function object that is invoked for each iteration.
 */
template <typename IndexType, std::invocable<IndexType> Action>
constexpr void unroll4(IndexType index, const ssize iterations, const Action& action)
{
  TACTILE_ASSERT(iterations >= 0);

  if (iterations <= 0) {
    return;
  }

  const ssize chunk_size {4};
  const auto pass_count = (iterations + (chunk_size - 1)) / chunk_size;

  ssize pass {0};
  switch (iterations % chunk_size) {  // NOLINT(*-switch-missing-default-case)
    do {
      case 0: action(index++); [[fallthrough]];
      case 3: action(index++); [[fallthrough]];
      case 2: action(index++); [[fallthrough]];
      case 1: action(index++); ++pass;
    } while (pass < pass_count);
  }
}

/**
 * Utility for partially unrolling loops, processing actions in chunks of 8.
 *
 * \param index      The initial index value. Incremented after each call to `action`. Note, this can be negative.
 * \param iterations The number of iterations.
 * \param action     A function object that is invoked for each iteration.
 *
 * \see The `unroll4` documentation for a detailed description.
 */
template <typename IndexType, std::invocable<IndexType> Action>
constexpr void unroll8(IndexType index, const ssize iterations, const Action& action)
{
  TACTILE_ASSERT(iterations >= 0);

  if (iterations <= 0) {
    return;
  }

  const ssize chunk_size {8};
  const auto pass_count = (iterations + (chunk_size - 1)) / chunk_size;

  ssize pass {0};
  switch (iterations % chunk_size) {  // NOLINT(*-switch-missing-default-case)
    do {
      case 0: action(index++); [[fallthrough]];
      case 7: action(index++); [[fallthrough]];
      case 6: action(index++); [[fallthrough]];
      case 5: action(index++); [[fallthrough]];
      case 4: action(index++); [[fallthrough]];
      case 3: action(index++); [[fallthrough]];
      case 2: action(index++); [[fallthrough]];
      case 1: action(index++); ++pass;
    } while (pass < pass_count);
  }
}

}  // namespace tactile
