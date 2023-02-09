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

#include <tl/expected.hpp>

namespace tactile {

template <typename T, typename E>
using Expected = tl::expected<T, E>;

template <typename E>
using Unexpected = tl::unexpected<E>;

template <typename E>
[[nodiscard]] auto unexpected(E&& e) -> Unexpected<std::decay_t<E>>
{
  return tl::make_unexpected(std::forward<E>(e));
}

template <typename T, typename E>
[[nodiscard]] auto propagate_unexpected(const Expected<T, E>& e) -> Unexpected<E>
{
  return unexpected(e.error());
}

}  // namespace tactile
