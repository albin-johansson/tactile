// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/zlib_compression/zlib_compressor.hpp"

#include <numeric>  // iota

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tactile/base/container/string.hpp"

namespace tactile::test {

// tactile::ZlibCompressor::compress
// tactile::ZlibCompressor::decompress
TEST(ZlibCompressor, CompressAndDecompressBytes)
{
  const ZlibCompressor compressor {};

  ByteStream bytes {};
  bytes.resize(64'000);
  std::iota(bytes.begin(), bytes.end(), 0);

  const auto compressed_bytes = compressor.compress(bytes);
  ASSERT_TRUE(compressed_bytes.has_value());

  const auto decompressed_bytes = compressor.decompress(*compressed_bytes);
  ASSERT_TRUE(decompressed_bytes.has_value());
  EXPECT_THAT(*decompressed_bytes, testing::ContainerEq(bytes));
}

// tactile::ZlibCompressor::compress
// tactile::ZlibCompressor::decompress
TEST(ZlibCompressor, CompressAndDecompressString)
{
  const ZlibCompressor compressor {};

  const StringView original_string =
      "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Mi bibendum neque egestas congue quisque egestas diam in arcu. Varius duis at consectetur lorem. Ultricies tristique nulla aliquet enim tortor at auctor. Nibh nisl condimentum id venenatis a condimentum vitae sapien pellentesque. Venenatis urna cursus eget nunc scelerisque. Mattis molestie a iaculis at erat pellentesque adipiscing commodo elit. Commodo ullamcorper a lacus vestibulum sed arcu non odio euismod. Vivamus arcu felis bibendum ut. Libero enim sed faucibus turpis in eu mi bibendum neque. Blandit volutpat maecenas volutpat blandit aliquam etiam.";

  const auto compressed_bytes =
      compressor.compress(make_byte_span(original_string));
  ASSERT_TRUE(compressed_bytes.has_value());

  const auto decompressed_bytes = compressor.decompress(*compressed_bytes);
  ASSERT_TRUE(decompressed_bytes.has_value());

  const String restored_string {decompressed_bytes->begin(),
                                decompressed_bytes->end()};
  EXPECT_EQ(restored_string, original_string);
}

}  // namespace tactile::test
