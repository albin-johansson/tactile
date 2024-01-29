// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include <gtest/gtest.h>

#include "tactile/foundation/io/compression/zlib_compression_provider.hpp"
#include "tactile/foundation/io/compression/zstd_compression_provider.hpp"
#include "tactile/testutil/random.hpp"

using namespace tactile;
using namespace tactile::int_literals;

namespace {

template <typename T>
class ICompressionProviderTest : public testing::Test {};

using CompressionProviderTypes =
    testing::Types<ZlibCompressionProvider, ZstdCompressionProvider>;

TYPED_TEST_SUITE(ICompressionProviderTest, CompressionProviderTypes);

/// \tests tactile::ZlibCompressor::compress
/// \tests tactile::ZlibCompressor::decompress
TYPED_TEST(ICompressionProviderTest, CompressAndDecompress)
{
  const TypeParam compressor;
  const auto original_bytes = testutil::make_random_byte_stream(50'000);

  const auto compressed_bytes = compressor.compress(original_bytes);
  ASSERT_TRUE(compressed_bytes.has_value());

  EXPECT_LT(compressed_bytes->size(), original_bytes.size());

  const auto decompressed_bytes = compressor.decompress(*compressed_bytes);
  ASSERT_TRUE(decompressed_bytes.has_value());

  EXPECT_EQ(decompressed_bytes, original_bytes);
}

}  // namespace
