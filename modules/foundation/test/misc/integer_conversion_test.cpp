// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/misc/integer_conversion.hpp"

#include <limits>  // numeric_limits

#include <gtest/gtest.h>

using namespace tactile;

/// \tests tactile::as_signed
TEST(IntegerConversion, AsSigned)
{
  const auto max_i32 = std::numeric_limits<int32>::max();
  const auto max_u32 = std::numeric_limits<uint32>::max();

  EXPECT_EQ(as_signed(0u), 0);
  EXPECT_EQ(as_signed(42u), 42);
  EXPECT_EQ(as_signed(uint8 {53}), int8 {53});
  EXPECT_EQ(as_signed(uint16 {8'273}), int16 {8'273});
  EXPECT_EQ(as_signed(static_cast<uint32>(max_i32)), max_i32);

  EXPECT_THROW((void) as_signed(static_cast<uint32>(max_i32) + 1), Exception);
  EXPECT_THROW((void) as_signed(max_u32), Exception);
}

/// \tests tactile::as_unsigned
TEST(IntegerConversion, AsUnsigned)
{
  const auto min_i32 = std::numeric_limits<int32>::min();
  const auto max_i32 = std::numeric_limits<int32>::max();

  EXPECT_EQ(as_unsigned(0), 0u);
  EXPECT_EQ(as_unsigned(42), 42u);
  EXPECT_EQ(as_unsigned(int8 {0x7F}), uint8 {0x7F});
  EXPECT_EQ(as_unsigned(max_i32), static_cast<uint32>(max_i32));

  EXPECT_THROW((void) as_unsigned(-1), Exception);
  EXPECT_THROW((void) as_unsigned(min_i32), Exception);
}
