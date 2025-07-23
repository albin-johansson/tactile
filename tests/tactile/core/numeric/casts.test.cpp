// Copyright (C) 2025 Albin Johansson

#include <gtest/gtest.h>

import tactile.core.ext.std;
import tactile.core.common;
import tactile.core.numeric;

namespace tactile::tests {
namespace {

class NumericCastsTest : public testing::Test
{};

TEST_F(NumericCastsTest, CheckedCast_SignedToSigned)
{
  EXPECT_EQ(checked_cast<i8>(i8 {42}), i8 {42});

  EXPECT_EQ(checked_cast<i8>(i16 {kMinI8}), kMinI8);
  EXPECT_EQ(checked_cast<i8>(i16 {kMaxI8}), kMaxI8);

  EXPECT_EQ(checked_cast<i16>(i32 {kMinI16}), kMinI16);
  EXPECT_EQ(checked_cast<i16>(i32 {kMaxI16}), kMaxI16);

  EXPECT_THROW((void) checked_cast<i16>(i32 {kMinI16 - 1}), std::underflow_error);
  EXPECT_THROW((void) checked_cast<i16>(i32 {kMaxI16 + 1}), std::overflow_error);
}

TEST_F(NumericCastsTest, CheckedCast_UnsignedToUnsigned)
{
  EXPECT_EQ(checked_cast<u8>(u8 {42}), u8 {42});

  EXPECT_EQ(checked_cast<u8>(u16 {kMinU8}), kMinU8);
  EXPECT_EQ(checked_cast<u8>(u16 {kMaxU8}), kMaxU8);

  EXPECT_EQ(checked_cast<u16>(u32 {kMinU16}), kMinU16);
  EXPECT_EQ(checked_cast<u16>(u32 {kMaxU16}), kMaxU16);

  EXPECT_THROW((void) checked_cast<u16>(u32 {kMaxU16 + 1}), std::overflow_error);
}

TEST_F(NumericCastsTest, CheckedCast_SignedToUnsigned)
{
  EXPECT_EQ(checked_cast<u8>(i8 {42}), u8 {42});

  EXPECT_EQ(checked_cast<u8>(i16 {kMinU8}), kMinU8);
  EXPECT_EQ(checked_cast<u8>(i16 {kMaxU8}), kMaxU8);

  EXPECT_EQ(checked_cast<u16>(i32 {kMinU16}), kMinU16);
  EXPECT_EQ(checked_cast<u16>(i32 {kMaxU16}), kMaxU16);

  EXPECT_THROW((void) checked_cast<u8>(i16 {-1}), std::underflow_error);
  EXPECT_THROW((void) checked_cast<u8>(i16 {kMaxU8 + 1}), std::overflow_error);
}

TEST_F(NumericCastsTest, CheckedCast_UnsignedToSigned)
{
  EXPECT_EQ(checked_cast<i8>(u8 {42}), i8 {42});

  EXPECT_EQ(checked_cast<i8>(u16 {0}), i8 {0});
  EXPECT_EQ(checked_cast<i8>(u16 {kMaxI8}), kMaxI8);

  EXPECT_EQ(checked_cast<i16>(u32 {0}), i16 {0});
  EXPECT_EQ(checked_cast<i16>(u32 {kMaxI16}), kMaxI16);

  EXPECT_THROW((void) checked_cast<i8>(u16 {kMaxI8 + 1}), std::overflow_error);
}

TEST_F(NumericCastsTest, ToSigned)
{
  static_assert(std::same_as<decltype(to_signed(u8 {})), i8>);
  static_assert(std::same_as<decltype(to_signed(u16 {})), i16>);
  static_assert(std::same_as<decltype(to_signed(u32 {})), i32>);
  static_assert(std::same_as<decltype(to_signed(u64 {})), i64>);
  static_assert(std::same_as<decltype(to_signed(usize {})), isize>);

  EXPECT_EQ(to_signed(0u), 0);
  EXPECT_EQ(to_signed(u8 {100u}), i8 {100});
  EXPECT_EQ(to_signed(u16 {18u}), i16 {18});
  EXPECT_EQ(to_signed(u32 {42u}), i32 {42});
  EXPECT_EQ(to_signed(usize {10'000u}), isize {10'000});

  EXPECT_THROW((void) to_signed(u8 {128u}), std::overflow_error);
  EXPECT_THROW((void) to_signed(u16 {32'768u}), std::overflow_error);
}

TEST_F(NumericCastsTest, ToUnsigned)
{
  static_assert(std::same_as<decltype(to_unsigned(i8 {})), u8>);
  static_assert(std::same_as<decltype(to_unsigned(i16 {})), u16>);
  static_assert(std::same_as<decltype(to_unsigned(i32 {})), u32>);
  static_assert(std::same_as<decltype(to_unsigned(i64 {})), u64>);
  static_assert(std::same_as<decltype(to_unsigned(isize {})), usize>);

  EXPECT_EQ(to_unsigned(0), 0u);
  EXPECT_EQ(to_unsigned(i8 {100}), u8 {100});
  EXPECT_EQ(to_unsigned(i16 {18}), u16 {18});
  EXPECT_EQ(to_unsigned(i32 {42}), u32 {42});
  EXPECT_EQ(to_unsigned(isize {10'000}), usize {10'000});

  EXPECT_THROW((void) to_unsigned(i8 {-1}), std::underflow_error);
}

TEST_F(NumericCastsTest, SaturateCast_SignedToSigned)
{
  EXPECT_EQ(saturate_cast<i8>(i8 {42}), i8 {42});

  EXPECT_EQ(saturate_cast<i8>(i16 {kMinI8}), kMinI8);
  EXPECT_EQ(saturate_cast<i8>(i16 {kMaxI8}), kMaxI8);

  EXPECT_EQ(saturate_cast<i16>(i32 {kMinI16}), kMinI16);
  EXPECT_EQ(saturate_cast<i16>(i32 {kMaxI16}), kMaxI16);

  EXPECT_EQ(saturate_cast<i16>(i32 {kMinI16 - 1}), kMinI16);
  EXPECT_EQ(saturate_cast<i16>(i32 {kMaxI16 + 1}), kMaxI16);
}

TEST_F(NumericCastsTest, SaturateCast_UnsignedToUnsigned)
{
  EXPECT_EQ(saturate_cast<u8>(u8 {42}), u8 {42});

  EXPECT_EQ(saturate_cast<u8>(u16 {kMinU8}), kMinU8);
  EXPECT_EQ(saturate_cast<u8>(u16 {kMaxU8}), kMaxU8);

  EXPECT_EQ(saturate_cast<u16>(u32 {kMinU16}), kMinU16);
  EXPECT_EQ(saturate_cast<u16>(u32 {kMaxU16}), kMaxU16);

  EXPECT_EQ(saturate_cast<u16>(u32 {kMaxU16 + 1}), kMaxU16);
}

TEST_F(NumericCastsTest, SaturateCast_SignedToUnsigned)
{
  EXPECT_EQ(saturate_cast<u8>(i8 {42}), u8 {42});

  EXPECT_EQ(saturate_cast<u8>(i16 {kMinU8}), kMinU8);
  EXPECT_EQ(saturate_cast<u8>(i16 {kMaxU8}), kMaxU8);

  EXPECT_EQ(saturate_cast<u16>(i32 {kMinU16}), kMinU16);
  EXPECT_EQ(saturate_cast<u16>(i32 {kMaxU16}), kMaxU16);

  EXPECT_EQ(saturate_cast<u8>(i16 {-1}), kMinU8);
  EXPECT_EQ(saturate_cast<u8>(i16 {kMaxU8 + 1}), kMaxU8);
}

TEST_F(NumericCastsTest, SaturateCast_UnsignedToSigned)
{
  EXPECT_EQ(saturate_cast<i8>(u8 {42}), i8 {42});

  EXPECT_EQ(saturate_cast<i8>(u16 {0}), i8 {0});
  EXPECT_EQ(saturate_cast<i8>(u16 {kMaxI8}), kMaxI8);

  EXPECT_EQ(saturate_cast<i16>(u32 {0}), i16 {0});
  EXPECT_EQ(saturate_cast<i16>(u32 {kMaxI16}), kMaxI16);

  EXPECT_EQ(saturate_cast<i8>(u16 {kMaxI8 + 1}), kMaxI8);
}

}  // namespace
}  // namespace tactile::tests
