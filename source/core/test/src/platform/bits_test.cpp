// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/platform/bits.hpp"

#include <gtest/gtest.h>

namespace tactile {

/** \trace tactile::to_little_endian */
TEST(Bits, ToLittleEndian)
{
  const uint32 native_value = 0xFFEE2211u;
  const uint32 little_value = to_little_endian(native_value);

  EXPECT_EQ(nth_byte(little_value, 0), 0x11);
  EXPECT_EQ(nth_byte(little_value, 1), 0x22);
  EXPECT_EQ(nth_byte(little_value, 2), 0xEE);
  EXPECT_EQ(nth_byte(little_value, 3), 0xFF);
}

}  // namespace tactile
