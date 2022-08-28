#include "io/compression.hpp"

#include <gtest/gtest.h>

#include "core/util/random.hpp"

namespace tactile::test {
namespace {

[[nodiscard]] auto make_bytes() -> io::ByteStream
{
  io::ByteStream bytes;
  bytes.reserve(42'000);

  for (usize i = 0, capacity = bytes.capacity(); i < capacity; ++i) {
    bytes.push_back(static_cast<uint8>(next_random(+'a', +'z')));
  }

  return bytes;
}

}  // namespace

TEST(Compression, ZlibCompressAndDecompress)
{
  const auto source = make_bytes();

  const auto compressed = io::zlib_compress(source).value();
  const auto decompressed = io::zlib_decompress(compressed).value();

  ASSERT_EQ(source, decompressed);
}

TEST(Compression, ZstdCompressAndDecompress)
{
  const auto source = make_bytes();

  const auto compressed = io::zstd_compress(source).value();
  const auto decompressed = io::zstd_decompress(compressed).value();

  ASSERT_EQ(source, decompressed);
}

}  // namespace tactile::test
