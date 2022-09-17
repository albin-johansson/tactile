#pragma once

#include <span>    // span
#include <vector>  // vector

#include "core/common/maybe.hpp"
#include "core/common/vocabulary.hpp"

namespace tactile::io {

using ByteStream = std::vector<uint8>;

[[nodiscard]] auto zlib_compress(const void* source, usize source_bytes, int level = -1)
    -> Maybe<ByteStream>;

[[nodiscard]] auto zlib_decompress(const void* source, usize source_bytes)
    -> Maybe<ByteStream>;

[[nodiscard]] auto zstd_compress(const void* source, usize source_bytes)
    -> Maybe<ByteStream>;

[[nodiscard]] auto zstd_decompress(const void* source, usize source_bytes)
    -> Maybe<ByteStream>;

[[nodiscard]] auto zlib_decompress(std::span<const uint8> span) -> Maybe<ByteStream>;
[[nodiscard]] auto zstd_decompress(std::span<const uint8> span) -> Maybe<ByteStream>;

template <typename T>
[[nodiscard]] auto zlib_compress(const std::vector<T>& vec, const int level = -1)
    -> Maybe<ByteStream>
{
  return zlib_compress(vec.data(), vec.size() * sizeof(T), level);
}

template <typename T>
[[nodiscard]] auto zstd_compress(const std::vector<T>& vec) -> Maybe<ByteStream>
{
  return zstd_compress(vec.data(), vec.size() * sizeof(T));
}

}  // namespace tactile::io
