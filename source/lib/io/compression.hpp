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

#include "common/primitives.hpp"
#include "common/type/maybe.hpp"
#include "common/type/span.hpp"
#include "common/type/vector.hpp"

namespace tactile {

/// Represents a sequence of bytes.
using ByteStream = Vector<uint8>;

/// Represents a read-only view into a sequence of bytes.
using ByteSpan = Span<const uint8>;

/**
 * Compresses a sequence of bytes with Zlib.
 *
 * \details Supplying a null data pointer or a data stream of zero bytes will cause the
 * function to simply return nothing.
 *
 * \param source the source data that will be compressed.
 * \param source_bytes the total size of the source data in bytes.
 * \param level the compression level that will be used (clamped to a valid value).
 *
 * \return a compressed byte stream, or nothing if something went wrong.
 */
[[nodiscard]] auto zlib_compress(const void* source, usize source_bytes, int level = -1)
    -> Maybe<ByteStream>;

/// Compresses a span of bytes with Zlib.
/// \see zlib_compress(const void*, usize, int)
template <typename T>
[[nodiscard]] auto zlib_compress(Span<const T> span, const int level = -1)
    -> Maybe<ByteStream>
{
  return zlib_compress(span.data(), span.size_bytes(), level);
}

/**
 * Decompresses a sequence of compressed bytes with Zlib.
 *
 * \pre the source data must have been compressed using Zlib.
 *
 * \param source the compressed source data.
 * \param source_bytes the total size of the compressed source data in bytes.
 *
 * \return a byte stream of uncompressed data, or nothing if something went wrong.
 */
[[nodiscard]] auto zlib_decompress(const void* source, usize source_bytes)
    -> Maybe<ByteStream>;

/// Decompresses a sequence of compressed bytes with Zlib.
/// \see zlib_decompress(const void*, usize)
template <typename T>
[[nodiscard]] auto zlib_decompress(Span<const T> span) -> Maybe<ByteStream>
{
  return zlib_decompress(span.data(), span.size_bytes());
}

/**
 * Indicates whether the specified value is a valid Zlib compression level.
 *
 * \param level the value to evaluate.
 * \return true if the value is valid; false otherwise.
 */
[[nodiscard]] auto is_valid_zlib_compression_level(int level) -> bool;

[[nodiscard]] auto min_zlib_compression_level() -> int;

[[nodiscard]] auto max_zlib_compression_level() -> int;

/**
 * Compresses a sequence of bytes with Zstd.
 *
 * \details Supplying a null data pointer or a data stream of zero bytes will cause the
 * function to simply return nothing.
 *
 * \param source the source data that will be compressed.
 * \param source_bytes the total size of the source data, in bytes.
 *
 * \return a compressed byte stream, or nothing if something went wrong.
 */
[[nodiscard]] auto zstd_compress(const void* source, usize source_bytes)
    -> Maybe<ByteStream>;

/// Compresses a span of bytes with Zstd.
/// \see zstd_compress(const void*, usize)
template <typename T>
[[nodiscard]] auto zstd_compress(Span<const T> span) -> Maybe<ByteStream>
{
  return zstd_compress(span.data(), span.size_bytes());
}

/**
 * Decompresses a sequence of compressed bytes with Zstd.
 *
 * \pre the source data must have been compressed using Zstd.
 *
 * \param source the compressed source data.
 * \param source_bytes the total size of the compressed source data in bytes.
 *
 * \return a byte stream of uncompressed data, or nothing if something went wrong.
 */
[[nodiscard]] auto zstd_decompress(const void* source, usize source_bytes)
    -> Maybe<ByteStream>;

/// Decompresses a span of compressed bytes with Zstd.
/// \see zstd_decompress(const void*, usize, usize)
template <typename T>
[[nodiscard]] auto zstd_decompress(Span<const T> span) -> Maybe<ByteStream>
{
  return zstd_decompress(span.data(), span.size_bytes());
}

/**
 * Indicates whether the specified value is a valid Zstd compression level.
 *
 * \param level the value to evaluate.
 * \return true if the value is valid; false otherwise.
 *
 * \see https://github.com/facebook/zstd/issues/3133 for Zstd compression levels.
 */
[[nodiscard]] auto is_valid_zstd_compression_level(int level) -> bool;

[[nodiscard]] auto min_zstd_compression_level() -> int;

[[nodiscard]] auto max_zstd_compression_level() -> int;

}  // namespace tactile
