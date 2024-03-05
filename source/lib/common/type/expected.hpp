/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <type_traits>  // decay_t
#include <utility>      // forward
#include <version>      // __cpp_lib_expected

#if __cpp_lib_expected >= 202211L
  #define TACTILE_HAS_STD_EXPECTED 1
#else
  #define TACTILE_HAS_STD_EXPECTED 0
#endif

#if TACTILE_HAS_STD_EXPECTED
  #include <expected>  // expected, unexpected
#else
  #include <tl/expected.hpp>
#endif

namespace tactile {

#if TACTILE_HAS_STD_EXPECTED

template <typename T, typename E>
using Expected = std::expected<T, E>;

template <typename E>
using Unexpected = std::unexpected<E>;

#else

template <typename T, typename E>
using Expected = tl::expected<T, E>;

template <typename E>
using Unexpected = tl::unexpected<E>;

#endif

template <typename E>
[[nodiscard]] auto unexpected(E&& e) -> Unexpected<std::decay_t<E>>
{
#if TACTILE_HAS_STD_EXPECTED
  return std::unexpected {std::forward<E>(e)};
#else
  return tl::make_unexpected(std::forward<E>(e));
#endif
}

template <typename T, typename E>
[[nodiscard]] auto propagate_unexpected(const Expected<T, E>& e) -> Unexpected<E>
{
  return unexpected(e.error());
}

}  // namespace tactile
