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

#include <optional>     // optional
#include <string_view>  // string_view

#include "tactile/core/common/prelude.hpp"
#include "tactile/core/core.hpp"

namespace tactile {

/** Converts a string into a 32-bit signed integer. */
[[nodiscard]] TACTILE_CORE_API auto str_to_i32(std::string_view str, int base = 10)
    -> std::optional<int32>;

/** Converts a string into a 32-bit unsigned integer. */
[[nodiscard]] TACTILE_CORE_API auto str_to_u32(std::string_view str, int base = 10)
    -> std::optional<uint32>;

/** Converts a string into a 32-bit float. */
[[nodiscard]] TACTILE_CORE_API auto str_to_f32(std::string_view str)
    -> std::optional<float32>;

}  // namespace tactile
