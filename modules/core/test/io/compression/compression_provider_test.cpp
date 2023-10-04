// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include <gtest/gtest.h>

#include "tactile/core/io/compression/zlib_compressor.hpp"
#include "tactile/core/io/compression/zstd_compressor.hpp"
#include "tactile/core/math/rng.hpp"

using namespace tactile;
using namespace tactile::int_literals;

namespace {

[[nodiscard]]
auto _make_random_byte_stream(const usize byte_count) -> ByteStream
{
  ByteStream bytes;
  bytes.reserve(byte_count);

  for (usize i = 0; i < byte_count; ++i) {
    bytes.push_back(static_cast<uint8>(next_random_u32(+'a', +'z')));
  }

  return bytes;
}

}  // namespace

template <typename T>
class ICompressionProviderTest : public testing::Test {};

using CompressorProviderTypes = testing::Types<ZlibCompressor, ZstdCompressor>;

TYPED_TEST_SUITE(ICompressionProviderTest, CompressorProviderTypes);

/// \tests tactile::ZlibCompressor::compress
/// \tests tactile::ZlibCompressor::decompress
TYPED_TEST(ICompressionProviderTest, CompressAndDecompress)
{
  const TypeParam compressor;
  const auto original_bytes = _make_random_byte_stream(50'000);

  const auto compressed_bytes = compressor.compress(original_bytes);
  ASSERT_TRUE(compressed_bytes.has_value());

  EXPECT_LT(compressed_bytes->size(), original_bytes.size());

  const auto decompressed_bytes = compressor.decompress(*compressed_bytes);
  ASSERT_TRUE(decompressed_bytes.has_value());

  EXPECT_EQ(decompressed_bytes, original_bytes);
}
