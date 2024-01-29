// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/io/compression/zlib_compression_provider.hpp"

#include <gtest/gtest.h>

#include "tactile/testutil/random.hpp"

using namespace tactile;
using namespace tactile::int_literals;

namespace {

/// \tests tactile::ZlibCompressionProvider::compress
/// \tests tactile::ZlibCompressionProvider::decompress
TEST(ZlibCompressionProvider, CompressAndDecompress)
{
  const ZlibCompressionProvider compressor;
  const auto original_bytes = testutil::make_random_byte_stream(50'000);

  const auto compressed_bytes = compressor.compress(original_bytes);
  ASSERT_TRUE(compressed_bytes.has_value());

  EXPECT_LT(compressed_bytes->size(), original_bytes.size());

  const auto decompressed_bytes = compressor.decompress(*compressed_bytes);
  ASSERT_TRUE(decompressed_bytes.has_value());

  EXPECT_EQ(decompressed_bytes, original_bytes);
}

/// \tests tactile::ZlibCompressionProvider::set_compression_level
/// \tests tactile::ZlibCompressionProvider::get_compression_level
/// \tests tactile::ZlibCompressionProvider::min_compression_level
/// \tests tactile::ZlibCompressionProvider::max_compression_level
TEST(ZlibCompressionProvider, CompressionLevel)
{
  const auto min_level = ZlibCompressionProvider::min_compression_level();
  const auto max_level = ZlibCompressionProvider::max_compression_level();
  ASSERT_LT(min_level, max_level);

  ZlibCompressionProvider compressor;
  EXPECT_FALSE(compressor.get_compression_level().has_value());

  compressor.set_compression_level(max_level + 1);
  EXPECT_EQ(compressor.get_compression_level(), max_level);

  compressor.set_compression_level(min_level - 1);
  EXPECT_EQ(compressor.get_compression_level(), min_level);

  compressor.set_compression_level(kNone);
  EXPECT_FALSE(compressor.get_compression_level().has_value());

  compressor.set_compression_level(min_level + 2);
  EXPECT_EQ(compressor.get_compression_level(), min_level + 2);
}

}  // namespace
