// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/debug/error.hpp"

namespace tactile {

/**
 * \brief Throws an exception if the provided pointer is null. Otherwise, the pointer is simply returned.
 *
 * \note This function is particularly useful for use in constructor member initializer lists, as in the following
 *       example.
 *
 *       \code{cpp}
 *       A::A(B* b, C* c)
 *         : _b{require_not_null(b)},
 *           _c{require_not_null(c)}
 *       {}
 *       \endcode
 *
 * \tparam Ptr a pointer-like type.
 *
 * \param ptr           the pointer to validate.
 * \param error_message the error message used if the pointer is null.
 *
 * \return the provided pointer.
 */
template <typename Ptr>
[[nodiscard]] auto require_not_null(Ptr ptr,
                                    const char* error_message = "unexpected null pointer")
    -> Ptr
{
  if (ptr == nullptr) {
    throw RuntimeError {error_message};
  }

  return ptr;
}

}  // namespace tactile