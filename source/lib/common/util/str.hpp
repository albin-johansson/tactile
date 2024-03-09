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

#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/int.hpp"

namespace tactile {

/// Converts a string into a 32-bit signed integer.
[[nodiscard]] auto parse_i32(StringView str, int base = 10) -> Maybe<int32>;

/// Converts a string into a 32-bit unsigned integer.
[[nodiscard]] auto parse_u32(StringView str, int base = 10) -> Maybe<uint32>;

/// Converts a string into a 32-bit float.
[[nodiscard]] auto parse_f32(StringView str) -> Maybe<float>;

}  // namespace tactile
