// Copyright (C) 2025 Albin Johansson

#include <gmock/gmock.h>
#include <gtest/gtest.h>

import std;
import tactile.core;
import tactile.zlib;

namespace tactile::tests {
namespace {

using testing::ContainerEq;

class ZlibCompressionFormatTest : public testing::Test
{};

TEST_F(ZlibCompressionFormatTest, CompressAndDecompressBytes)
{
  const ZlibCompressionFormat zlib {};

  Vector<u8> bytes {};
  bytes.resize(64'000uz);
  std::iota(bytes.begin(), bytes.end(), u8 {0});

  const auto compressed_bytes = zlib.compress(bytes);
  ASSERT_TRUE(compressed_bytes.has_value());

  const auto decompressed_bytes = zlib.decompress(*compressed_bytes);
  ASSERT_TRUE(decompressed_bytes.has_value());
  EXPECT_THAT(*decompressed_bytes, ContainerEq(bytes));
}

}  // namespace
}  // namespace tactile::tests
