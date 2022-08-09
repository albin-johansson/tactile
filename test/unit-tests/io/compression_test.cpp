#include "io/compression.hpp"

#include <array>  // array
#include <bit>    // endian

#include <gtest/gtest.h>

#include "core/util/random.hpp"
#include "core/util/tiles.hpp"

namespace tactile::test {

TEST(Compression, CompressAndDecompress)
{
  std::vector<int> source;
  source.reserve(1'000);

  for (usize i = 0; i < 1'000; ++i) {
    source.push_back(next_random(0, 100));
  }

  const auto compressed = zlib_compress(source).value();
  const auto decompressed = zlib_decompress(compressed).value();

  const auto n_items = decompressed.size() / sizeof(int);
  ASSERT_EQ(source.size(), n_items);

  std::vector<int> restored;
  restored.reserve(source.size());

  for (usize i = 0; i < n_items; ++i) {
    const auto index = i * sizeof(int);

    const auto a = static_cast<int>(decompressed[index]);
    const auto b = static_cast<int>(decompressed[index + 1]);
    const auto c = static_cast<int>(decompressed[index + 2]);
    const auto d = static_cast<int>(decompressed[index + 3]);

    restored.push_back(std::endian::native == std::endian::little
                           ? (d << 3) | (c << 2) | (b << 1) | a
                           : (a << 3) | (b << 2) | (c << 1) | d);
  }

  ASSERT_EQ(restored, source);
}

}  // namespace tactile::test
