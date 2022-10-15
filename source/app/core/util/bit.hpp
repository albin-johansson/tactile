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

#include <concepts>  // integral

#include <EASTL/algorithm.h>
#include <EASTL/bit.h>
#include <EASTL/type_traits.h>
#include <SDL.h>

#include "core/type/array.hpp"
#include "core/vocabulary.hpp"

namespace tactile {

[[nodiscard]] inline auto to_little_endian(const uint32 value) noexcept -> uint32
{
  return SDL_SwapLE32(value);
}

[[nodiscard]] inline auto to_little_endian(const int32 value) noexcept -> int32
{
  return SDL_SwapLE32(value);
}

template <std::integral T>
[[nodiscard]] constexpr auto byteswap(T value) noexcept -> T
{
  // Based on example implementation: https://en.cppreference.com/w/cpp/numeric/byteswap
  static_assert(eastl::has_unique_object_representations_v<T>,
                "T may not have padding bits");

  using Bytes = Array<uint8, sizeof(value)>;

  auto bytes = eastl::bit_cast<Bytes>(value);
  eastl::reverse(eastl::begin(bytes), eastl::end(bytes));

  return eastl::bit_cast<T>(bytes);
}

}  // namespace tactile
