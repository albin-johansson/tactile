#include "io/compression.hpp"

#include <doctest/doctest.h>
#include <zlib.h>
#include <zstd.h>

#include "common/util/random.hpp"

using namespace tactile;

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

TEST_SUITE("Compression" * doctest::skip())
{
  TEST_CASE("zlib compress and decompress")
  {
    const auto original_bytes = _make_random_bytes();

    const auto compressed_bytes = zlib_compress(ByteSpan {original_bytes}).value();
    const auto decompressed_bytes = zlib_decompress(ByteSpan {compressed_bytes}).value();

    CHECK(original_bytes == decompressed_bytes);
  }

  TEST_CASE("zstd compress and decompress")
  {
    const auto original_bytes = _make_random_bytes();

    const auto compressed_bytes = zstd_compress(ByteSpan {original_bytes}).value();
    const auto decompressed_bytes = zstd_decompress(ByteSpan {compressed_bytes}).value();

    CHECK(original_bytes == decompressed_bytes);
  }

  TEST_CASE("Zlib compression level bounds")
  {
    CHECK(min_zlib_compression_level() == Z_BEST_SPEED);
    CHECK(max_zlib_compression_level() == Z_BEST_COMPRESSION);
  }

  TEST_CASE("Zstd compression level bounds")
  {
    // Zstd technically accepts other levels, but we use this range
    CHECK(min_zstd_compression_level() == 1);
    CHECK(max_zstd_compression_level() == 19);
  }

  TEST_CASE("Zlib compression level checking")
  {
    CHECK(is_valid_zlib_compression_level(Z_DEFAULT_COMPRESSION));
    CHECK(is_valid_zlib_compression_level(Z_BEST_SPEED));
    CHECK(is_valid_zlib_compression_level(Z_BEST_COMPRESSION));

    CHECK(!is_valid_zlib_compression_level(Z_BEST_SPEED - 1));
    CHECK(!is_valid_zlib_compression_level(Z_BEST_COMPRESSION + 1));

    CHECK(is_valid_zlib_compression_level(Z_BEST_SPEED + 1));
    CHECK(is_valid_zlib_compression_level(Z_BEST_COMPRESSION - 1));
  }

  TEST_CASE("Zstd compression level checking")
  {
    CHECK(is_valid_zstd_compression_level(ZSTD_defaultCLevel()));
    CHECK(is_valid_zstd_compression_level(1));
    CHECK(is_valid_zstd_compression_level(19));

    CHECK(!is_valid_zstd_compression_level(0));
    CHECK(!is_valid_zstd_compression_level(20));

    CHECK(is_valid_zstd_compression_level(2));
    CHECK(is_valid_zstd_compression_level(10));
  }
}
