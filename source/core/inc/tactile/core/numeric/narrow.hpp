// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>  // integral

namespace tactile {

/**
 * Performs a narrowing conversion of an integral value.
 *
 * \note
 *    This function is simply equivalent to a \c static_cast. The
 *    benefit of using this function to perform narrowing casts
 *    is improved readability, since the intention of the code
 *    becomes clearer.
 *
 * \tparam To   The target type.
 * \tparam From The source type.
 *
 * \param from The original value.
 *
 * \return
 *    The casted value.
 */
template <std::integral To, std::integral From>
  requires(sizeof(To) <= sizeof(From))
[[nodiscard]] constexpr auto narrow_cast(const From& from) noexcept -> To
{
  return static_cast<To>(from);
}

}  // namespace tactile
