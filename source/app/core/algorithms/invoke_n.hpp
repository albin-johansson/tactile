/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

namespace tactile {

/**
 * \brief Invokes the supplied callable for the specified amount of times.
 *
 * \ingroup core
 *
 * \tparam Integer the integral type that represents the number of invocations.
 * \tparam T the type of the callable.
 *
 * \param n the amount of times the callable should be invoked.
 * \param callable the callable that will be invoked.
 */
template <typename Integer, typename T>
constexpr void invoke_n(const Integer n, T&& callable) noexcept(noexcept(callable()))
{
  for (Integer i = 0; i < n; ++i) {
    callable();
  }
}

}  // namespace tactile
