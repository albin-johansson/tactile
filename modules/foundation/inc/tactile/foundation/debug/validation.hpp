// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/debug/exception.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Throws an exception if the provided pointer is null. Otherwise, the pointer is simply returned.
 *
 * \note
 *    This function is particularly useful for use in constructor member initializer lists, as in
 *    the following example.
 *
 *    \code{cpp}
 *    A::A(B* b, C* c)
 *      : _b{require_not_null(b)},
 *        _c{require_not_null(c)}
 *    {}
 *    \endcode
 *
 * \tparam Ptr A pointer-like type.
 *
 * \param ptr           The pointer to validate.
 * \param error_message The error message used if the pointer is null.
 *
 * \return
 *    The provided pointer.
 */
template <typename Ptr>
[[nodiscard]] auto require_not_null(Ptr ptr,
                                    const char* error_message = "unexpected null pointer")
    -> Ptr
{
  if (ptr == nullptr) [[unlikely]] {
    throw Exception {error_message};
  }

  return ptr;
}

/**
 * Throws an exception if the given result represents an error.
 *
 * \param result The result to check.
 */
inline void require_ok(const Result<void> result)
{
  if (!result.has_value()) [[unlikely]] {
    throw Exception {"unexpected empty result"};
  }
}

}  // namespace tactile