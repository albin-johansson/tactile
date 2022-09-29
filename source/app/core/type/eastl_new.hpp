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

/// This header provides overloads of operator new[], required by EASTL.

#include <cstddef>  // size_t, byte
#include <new>      // align_val_t

inline auto operator new[](const std::size_t size,
                           const char* /* p_name */,
                           int /* flags */,
                           unsigned /* debug_flags */,
                           const char* /* file */,
                           int /* line */) -> void*
{
  return new std::byte[size];
}

inline auto operator new[](const std::size_t size,
                           const std::size_t alignment,
                           std::size_t /* alignment_offset */,
                           const char* /* p_name */,
                           int /* flags */,
                           unsigned /* debug_flags */,
                           const char* /* file */,
                           int /* line */) -> void*
{
  return new (std::align_val_t {alignment}) std::byte[size];
}
