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

#include "buffers.hpp"

#include <algorithm>  // find
#include <cstring>    // memset
#include <iterator>   // distance

#include "core/common/ints.hpp"
#include "misc/panic.hpp"

namespace tactile {

void zero_buffer(std::span<char> buffer)
{
  std::memset(buffer.data(), 0, buffer.size_bytes());
}

void copy_string_into_buffer(std::span<char> buffer, std::string_view str)
{
  zero_buffer(buffer);

  usize index = 0;
  for (const auto ch : str) {
    if (index < buffer.size()) {
      buffer[index] = ch;
    }
    else {
      /* The string is larger than the buffer, so we are finished */
      break;
    }

    ++index;
  }
}

auto create_string_from_buffer(std::span<const char> buffer) -> std::string
{
  return std::string{create_string_view_from_buffer(buffer)};
}

auto create_string_view_from_buffer(std::span<const char> buffer) -> std::string_view
{
  const auto iter = std::find(buffer.begin(), buffer.end(), '\0');
  if (iter != buffer.end()) {
    const auto index = static_cast<usize>(std::distance(buffer.begin(), iter));
    return std::string_view{buffer.data(), index};
  }
  else {
    throw TactileError{"Invalid string buffer!"};
  }
}

}  // namespace tactile