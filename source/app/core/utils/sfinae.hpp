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

#include <type_traits>  // enable_if_t, ...

namespace tactile {

template <typename T>
using is_integral = std::enable_if_t<std::is_integral_v<T>, int>;

template <typename T>
using is_floating = std::enable_if_t<std::is_floating_point_v<T>, int>;

template <typename T>
using is_unsigned = std::enable_if_t<std::is_unsigned_v<T>, int>;

template <typename Base, typename Derived>
using is_derived_from = std::enable_if_t<std::is_base_of_v<Base, Derived>, int>;

}  // namespace tactile