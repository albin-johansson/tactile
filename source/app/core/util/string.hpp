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

#include "core/common/maybe.hpp"
#include "core/common/vocabulary.hpp"
#include "core/type/string.hpp"
#include "core/type/vector.hpp"

namespace tactile {

/// Splits a string into a collection of tokens, separated by a specific character.
[[nodiscard]] auto split(StringView str, char sep) -> Vec<String>;

/// Converts a string into a 32-bit signed integer.
[[nodiscard]] auto parse_i32(StringView str, int base = 10) -> Maybe<int32>;

}  // namespace tactile
