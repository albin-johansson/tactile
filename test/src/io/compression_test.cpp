#include "io/compression.hpp"

#include <doctest/doctest.h>

#include "common/util/random.hpp"

namespace tactile::test {
namespace {

[[nodiscard]] auto _make_random_bytes() -> ByteStream
{
  ByteStream bytes;
  bytes.reserve(42'000);

  for (usize i = 0, capacity = bytes.capacity(); i < capacity; ++i) {
    bytes.push_back(static_cast<uint8>(next_random_u32(+'a', +'z')));
  }

  return bytes;
}

}  // namespace

TEST_SUITE("Compression")
{
  TEST_CASE("zlib compress and decompress")
  {
    const auto original_bytes = _make_random_bytes();

    const auto compressed_bytes = zlib_compress(ByteSpan {original_bytes}).value();
    const auto decompressed_bytes = zlib_decompress(ByteSpan {compressed_bytes}).value();

    REQUIRE(original_bytes == decompressed_bytes);
  }

  TEST_CASE("zstd compress and decompress")
  {
    const auto original_bytes = _make_random_bytes();

    const auto compressed_bytes = zstd_compress(ByteSpan {original_bytes}).value();
    const auto decompressed_bytes = zstd_decompress(ByteSpan {compressed_bytes}).value();

    REQUIRE(original_bytes == decompressed_bytes);
  }
}

}  // namespace tactile::test
