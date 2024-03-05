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

#include <limits>  // numeric_limits

#include "tactile/base/int.hpp"

namespace tactile {

inline constexpr auto kMinFloat = std::numeric_limits<float>::min();
inline constexpr auto kMaxFloat = std::numeric_limits<float>::max();

inline constexpr auto kMinInt = std::numeric_limits<int>::min();
inline constexpr auto kMaxInt = std::numeric_limits<int>::max();

inline constexpr auto kMaxUint = std::numeric_limits<uint>::max();

inline constexpr int32 kMinFontSize = 8;   ///< The smallest available font size.
inline constexpr int32 kMaxFontSize = 32;  ///< The largest available font size.
inline constexpr int32 kDefFontSize = 14;  ///< The default font size.

}  // namespace tactile
