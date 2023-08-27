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

#include "tactile/core/api.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/** Forces initialization of the RNG engine for the current thread. */
TACTILE_CORE_API void rng_init();

[[nodiscard]] TACTILE_CORE_API auto next_random_u32(uint32 min, uint32 max) -> uint32;

[[nodiscard]] TACTILE_CORE_API auto next_random_u64(uint64 min, uint64 max) -> uint64;

[[nodiscard]] TACTILE_CORE_API auto next_random_i32(int32 min, int32 max) -> int32;

[[nodiscard]] TACTILE_CORE_API auto next_random_i64(int64 min, int64 max) -> int64;

[[nodiscard]] TACTILE_CORE_API auto next_random_f32(float32 min, float32 max) -> float32;

/// Returns a random boolean value.
[[nodiscard]] TACTILE_CORE_API auto next_bool() -> bool;

/// Returns a random float in the interval [0, 1].
[[nodiscard]] TACTILE_CORE_API auto next_float() -> float;

}  // namespace tactile
