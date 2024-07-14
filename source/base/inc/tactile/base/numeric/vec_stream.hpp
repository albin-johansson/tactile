// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <ostream>  // ostream

#include "tactile/base/int.hpp"
#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/util/concepts.hpp"

namespace tactile {

/**
 * Outputs a vector to a stream (for debugging purposes).
 *
 * \tparam T The element type used by the vector.
 * \tparam N The number of elements in the vector.
 *
 * \param stream The output stream.
 * \param vec    The vector to output.
 *
 * \return
 * The output stream.
 */
template <ArithmeticType T, usize N>
auto operator<<(std::ostream& stream, const Vec<T, N>& vec) -> std::ostream&
{
  stream << '{' << vec[0] << ',' << ' ' << vec[1];

  if constexpr (N > 2) {
    stream << ',' << ' ' << vec[2];
  }

  if constexpr (N > 3) {
    stream << ',' << ' ' << vec[3];
  }

  stream << '}';

  return stream;
}

}  // namespace tactile
