#pragma once

#include "tactile_error.hpp"

namespace tactile {

/**
 * \brief Verifies that the supplied pointer isn't null.
 *
 * \param ptr the pointer that will be verified.
 *
 * \return the supplied pointer.
 *
 * \throws TactileError if the supplied pointer is null.
 */
template <typename T>
[[nodiscard]] auto VerifyNotNull(T* ptr) -> T*
{
  if (ptr) [[likely]]
  {
    return ptr;
  }
  else
  {
    throw TactileError{"Invalid null pointer!"};
  }
}

}  // namespace tactile