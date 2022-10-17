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

#include <string>       // string
#include <string_view>  // string_view

#include "core/type/maybe.hpp"
#include "core/type/string.hpp"
#include "core/type/vector.hpp"
#include "core/vocabulary.hpp"

namespace tactile {

/// Splits a string into a collection of tokens, separated by a specific character.
[[nodiscard]] auto split(StringView str, char sep) -> Vec<String>;

/// Converts a string into a 32-bit signed integer.
[[nodiscard]] auto parse_i32(StringView str, int base = 10) -> Maybe<int32>;

/// Converts a Tactile string to a standard string.
[[nodiscard]] auto to_std(StringView str) -> std::string;

/// Converts a Tactile string view to a standard string view.
[[nodiscard]] auto to_std_view(StringView str) -> std::string_view;

/// Converts a standard string to the representation used in Tactile.
/// Use of this function is discouraged other than in low-level scenarios where standard
/// strings have to be handled.
[[nodiscard]] auto from_std(std::string_view str) -> String;

}  // namespace tactile
