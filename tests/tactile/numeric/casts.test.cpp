// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#include <gtest/gtest.h>

import std;
import tactile.core;
import tactile.numeric;

namespace tactile::tests {

TEST(Numeric_Casts, CheckedCast_SignedToSigned)
{
  EXPECT_EQ(checked_cast<i8>(i8 {42}), i8 {42});

  EXPECT_EQ(checked_cast<i8>(i16 {kMinI8}), kMinI8);
  EXPECT_EQ(checked_cast<i8>(i16 {kMaxI8}), kMaxI8);

  EXPECT_EQ(checked_cast<i16>(i32 {kMinI16}), kMinI16);
  EXPECT_EQ(checked_cast<i16>(i32 {kMaxI16}), kMaxI16);

  EXPECT_THROW((void) checked_cast<i16>(i32 {kMinI16 - 1}), std::underflow_error);
  EXPECT_THROW((void) checked_cast<i16>(i32 {kMaxI16 + 1}), std::overflow_error);
}

TEST(Numeric_Casts, CheckedCast_UnsignedToUnsigned)
{
  EXPECT_EQ(checked_cast<u8>(u8 {42}), u8 {42});

  EXPECT_EQ(checked_cast<u8>(u16 {kMinU8}), kMinU8);
  EXPECT_EQ(checked_cast<u8>(u16 {kMaxU8}), kMaxU8);

  EXPECT_EQ(checked_cast<u16>(u32 {kMinU16}), kMinU16);
  EXPECT_EQ(checked_cast<u16>(u32 {kMaxU16}), kMaxU16);

  EXPECT_THROW((void) checked_cast<u16>(u32 {kMaxU16 + 1}), std::overflow_error);
}

TEST(Numeric_Casts, CheckedCast_SignedToUnsigned)
{
  EXPECT_EQ(checked_cast<u8>(i8 {42}), u8 {42});

  EXPECT_EQ(checked_cast<u8>(i16 {kMinU8}), kMinU8);
  EXPECT_EQ(checked_cast<u8>(i16 {kMaxU8}), kMaxU8);

  EXPECT_EQ(checked_cast<u16>(i32 {kMinU16}), kMinU16);
  EXPECT_EQ(checked_cast<u16>(i32 {kMaxU16}), kMaxU16);

  EXPECT_THROW((void) checked_cast<u8>(i16 {-1}), std::underflow_error);
  EXPECT_THROW((void) checked_cast<u8>(i16 {kMaxU8 + 1}), std::overflow_error);
}

TEST(Numeric_Casts, CheckedCast_UnsignedToSigned)
{
  EXPECT_EQ(checked_cast<i8>(u8 {42}), i8 {42});

  EXPECT_EQ(checked_cast<i8>(u16 {0}), i8 {0});
  EXPECT_EQ(checked_cast<i8>(u16 {kMaxI8}), kMaxI8);

  EXPECT_EQ(checked_cast<i16>(u32 {0}), i16 {0});
  EXPECT_EQ(checked_cast<i16>(u32 {kMaxI16}), kMaxI16);

  EXPECT_THROW((void) checked_cast<i8>(u16 {kMaxI8 + 1}), std::overflow_error);
}

TEST(Numeric_Casts, SaturateCast_SignedToSigned)
{
  EXPECT_EQ(saturate_cast<i8>(i8 {42}), i8 {42});

  EXPECT_EQ(saturate_cast<i8>(i16 {kMinI8}), kMinI8);
  EXPECT_EQ(saturate_cast<i8>(i16 {kMaxI8}), kMaxI8);

  EXPECT_EQ(saturate_cast<i16>(i32 {kMinI16}), kMinI16);
  EXPECT_EQ(saturate_cast<i16>(i32 {kMaxI16}), kMaxI16);

  EXPECT_EQ(saturate_cast<i16>(i32 {kMinI16 - 1}), kMinI16);
  EXPECT_EQ(saturate_cast<i16>(i32 {kMaxI16 + 1}), kMaxI16);
}

TEST(Numeric_Casts, SaturateCast_UnsignedToUnsigned)
{
  EXPECT_EQ(saturate_cast<u8>(u8 {42}), u8 {42});

  EXPECT_EQ(saturate_cast<u8>(u16 {kMinU8}), kMinU8);
  EXPECT_EQ(saturate_cast<u8>(u16 {kMaxU8}), kMaxU8);

  EXPECT_EQ(saturate_cast<u16>(u32 {kMinU16}), kMinU16);
  EXPECT_EQ(saturate_cast<u16>(u32 {kMaxU16}), kMaxU16);

  EXPECT_EQ(saturate_cast<u16>(u32 {kMaxU16 + 1}), kMaxU16);
}

TEST(Numeric_Casts, SaturateCast_SignedToUnsigned)
{
  EXPECT_EQ(saturate_cast<u8>(i8 {42}), u8 {42});

  EXPECT_EQ(saturate_cast<u8>(i16 {kMinU8}), kMinU8);
  EXPECT_EQ(saturate_cast<u8>(i16 {kMaxU8}), kMaxU8);

  EXPECT_EQ(saturate_cast<u16>(i32 {kMinU16}), kMinU16);
  EXPECT_EQ(saturate_cast<u16>(i32 {kMaxU16}), kMaxU16);

  EXPECT_EQ(saturate_cast<u8>(i16 {-1}), kMinU8);
  EXPECT_EQ(saturate_cast<u8>(i16 {kMaxU8 + 1}), kMaxU8);
}

TEST(Numeric_Casts, SaturateCast_UnsignedToSigned)
{
  EXPECT_EQ(saturate_cast<i8>(u8 {42}), i8 {42});

  EXPECT_EQ(saturate_cast<i8>(u16 {0}), i8 {0});
  EXPECT_EQ(saturate_cast<i8>(u16 {kMaxI8}), kMaxI8);

  EXPECT_EQ(saturate_cast<i16>(u32 {0}), i16 {0});
  EXPECT_EQ(saturate_cast<i16>(u32 {kMaxI16}), kMaxI16);

  EXPECT_EQ(saturate_cast<i8>(u16 {kMaxI8 + 1}), kMaxI8);
}

}  // namespace tactile::tests
