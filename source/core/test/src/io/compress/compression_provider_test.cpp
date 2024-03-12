// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include <algorithm>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tactile/base/container/string.hpp"
#include "tactile/core/io/compress/zlib_compression_provider.hpp"
#include "tactile/core/io/compress/zstd_compression_provider.hpp"
#include "tactile/core/numeric/narrow.hpp"
#include "tactile/core/numeric/random.hpp"

namespace tactile {

template <typename T>
class CompressionProviderTest : public testing::Test {};

using CompressionProviderTypes =
    testing::Types<ZlibCompressionProvider, ZstdCompressionProvider>;

TYPED_TEST_SUITE(CompressionProviderTest, CompressionProviderTypes);

/**
 * \trace tactile::ZlibCompressionProvider::compress
 * \trace tactile::ZlibCompressionProvider::decompress
 */
TYPED_TEST(CompressionProviderTest, CompressAndDecompressRandom)
{
  const TypeParam provider {};

  ByteStream bytes {};
  bytes.resize(64'000);
  std::ranges::generate(bytes,
                        [] { return narrow_cast<uint8>(get_random_uint(+'a', +'z')); });

  const auto compressed_bytes = provider.compress(bytes);
  ASSERT_TRUE(compressed_bytes.has_value());

  const auto decompressed_bytes = provider.decompress(*compressed_bytes);
  ASSERT_TRUE(decompressed_bytes.has_value());
  EXPECT_THAT(*decompressed_bytes, testing::ContainerEq(bytes));
}

/**
 * \trace tactile::ZlibCompressionProvider::compress
 * \trace tactile::ZlibCompressionProvider::decompress
 */
TYPED_TEST(CompressionProviderTest, CompressAndDecompressString)
{
  const TypeParam provider {};

  const StringView original_string =
      "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor "
      "incididunt ut labore et dolore magna aliqua. Mi bibendum neque egestas congue "
      "quisque egestas diam in arcu. Varius duis at consectetur lorem. Ultricies "
      "tristique nulla aliquet enim tortor at auctor. Nibh nisl condimentum id venenatis "
      "a condimentum vitae sapien pellentesque. Venenatis urna cursus eget nunc "
      "scelerisque. Mattis molestie a iaculis at erat pellentesque adipiscing commodo "
      "elit. Commodo ullamcorper a lacus vestibulum sed arcu non odio euismod. Vivamus "
      "arcu felis bibendum ut. Libero enim sed faucibus turpis in eu mi bibendum neque. "
      "Blandit volutpat maecenas volutpat blandit aliquam etiam.";

  const auto compressed_bytes = provider.compress(make_byte_span(original_string));
  ASSERT_TRUE(compressed_bytes.has_value());

  const auto decompressed_bytes = provider.decompress(*compressed_bytes);
  ASSERT_TRUE(decompressed_bytes.has_value());

  const String restored_string {decompressed_bytes->begin(), decompressed_bytes->end()};
  EXPECT_EQ(restored_string, original_string);
}

}  // namespace tactile
