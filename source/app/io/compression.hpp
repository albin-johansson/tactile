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

#include "core/type/maybe.hpp"
#include "core/type/span.hpp"
#include "core/type/vector.hpp"
#include "core/vocabulary.hpp"

namespace tactile::io {

using ByteStream = Vec<uint8>;

[[nodiscard]] auto zlib_compress(const void* source, usize source_bytes, int level = -1)
    -> Maybe<ByteStream>;

[[nodiscard]] auto zlib_decompress(const void* source, usize source_bytes)
    -> Maybe<ByteStream>;

[[nodiscard]] auto zstd_compress(const void* source, usize source_bytes)
    -> Maybe<ByteStream>;

[[nodiscard]] auto zstd_decompress(const void* source, usize source_bytes)
    -> Maybe<ByteStream>;

[[nodiscard]] auto zlib_decompress(Span<const uint8> span) -> Maybe<ByteStream>;
[[nodiscard]] auto zstd_decompress(Span<const uint8> span) -> Maybe<ByteStream>;

template <typename T>
[[nodiscard]] auto zlib_compress(const Vec<T>& vec, const int level = -1)
    -> Maybe<ByteStream>
{
  return zlib_compress(vec.data(), vec.size() * sizeof(T), level);
}

template <typename T>
[[nodiscard]] auto zstd_compress(const Vec<T>& vec) -> Maybe<ByteStream>
{
  return zstd_compress(vec.data(), vec.size() * sizeof(T));
}

}  // namespace tactile::io
