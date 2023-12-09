// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/platform/bits.hpp"

#include <bit>  // bit_cast

#include <gtest/gtest.h>

#include "tactile/foundation/container/array.hpp"

using namespace tactile;

TEST(BitUtilities, ReverseBytes)
{
  using ByteArray = Array<uint8, sizeof(uint32)>;

  const uint32 original = 0xDEADBEEF;
  const auto swapped = reverse_bytes(original);

  const auto original_bytes = interpret_as<ByteArray>(original);
  const auto swapped_bytes = interpret_as<ByteArray>(swapped);

  EXPECT_EQ(original_bytes.at(0), swapped_bytes.at(3));
  EXPECT_EQ(original_bytes.at(1), swapped_bytes.at(2));
  EXPECT_EQ(original_bytes.at(2), swapped_bytes.at(1));
  EXPECT_EQ(original_bytes.at(3), swapped_bytes.at(0));
}
