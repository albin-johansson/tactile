// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile {

/**
 * Throws an exception if the provided pointer is null, returns it if not.
 *
 * \note
 * This function is particularly useful for use in constructor member
 * initializer lists, as in the following example.
 *
 * \code{cpp}
 * A::A(B* b, C* c)
 *   : _b{require_not_null(b)},
 *     _c{require_not_null(c)}
 * {}
 * \endcode
 *
 * \tparam PointerType A pointer-like type.
 *
 * \param ptr           The pointer to validate.
 * \param error_message An error message used if the pointer is null.
 *
 * \return
 * The provided pointer.
 */
template <typename PointerType>
[[nodiscard]] auto require_not_null(PointerType ptr,
                                    const char* error_message = "null pointer")
    -> PointerType
{
  if (ptr == nullptr) [[unlikely]] {
    throw Exception {error_message};
  }

  return ptr;
}

}  // namespace tactile
