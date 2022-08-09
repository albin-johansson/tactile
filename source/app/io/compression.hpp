#pragma once

#include <span>    // span
#include <vector>  // vector

#include "core/common/ints.hpp"
#include "core/common/maybe.hpp"

namespace tactile {

using ZlibData = std::vector<uint8>;

[[nodiscard]] auto zlib_compress(const void* source, usize source_bytes, int level = -1)
    -> Maybe<ZlibData>;

template <typename T>
[[nodiscard]] auto zlib_compress(const std::vector<T>& vec, const int level = -1)
    -> Maybe<ZlibData>
{
  return zlib_compress(vec.data(), vec.size() * sizeof(T), level);
}

[[nodiscard]] auto zlib_decompress(const void* source, usize source_bytes)
    -> Maybe<ZlibData>;

[[nodiscard]] auto zlib_decompress(std::span<const uint8> span) -> Maybe<ZlibData>;

}  // namespace tactile
