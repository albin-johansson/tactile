// Copyright (C) 2025 Albin Johansson

#include <gtest/gtest.h>

import tactile.core.ext.std;
import tactile.core.common;
import tactile.core.numeric;
import tactile.core.util;

namespace tactile::tests {
namespace {

class CastsTest : public testing::Test
{};

TEST_F(CastsTest, CheckedCast_SignedToSigned)
{
  EXPECT_EQ(checked_cast<i8>(i8 {42}), i8 {42});

  EXPECT_EQ(checked_cast<i8>(i16 {kMinI8}), kMinI8);
  EXPECT_EQ(checked_cast<i8>(i16 {kMaxI8}), kMaxI8);

  EXPECT_EQ(checked_cast<i16>(i32 {kMinI16}), kMinI16);
  EXPECT_EQ(checked_cast<i16>(i32 {kMaxI16}), kMaxI16);

  EXPECT_THROW((void) checked_cast<i16>(i32 {kMinI16 - 1}), std::range_error);
  EXPECT_THROW((void) checked_cast<i16>(i32 {kMaxI16 + 1}), std::range_error);
}

TEST_F(CastsTest, CheckedCast_UnsignedToUnsigned)
{
  EXPECT_EQ(checked_cast<u8>(u8 {42}), u8 {42});

  EXPECT_EQ(checked_cast<u8>(u16 {kMinU8}), kMinU8);
  EXPECT_EQ(checked_cast<u8>(u16 {kMaxU8}), kMaxU8);

  EXPECT_EQ(checked_cast<u16>(u32 {kMinU16}), kMinU16);
  EXPECT_EQ(checked_cast<u16>(u32 {kMaxU16}), kMaxU16);

  EXPECT_THROW((void) checked_cast<u16>(u32 {kMaxU16 + 1}), std::range_error);
}

TEST_F(CastsTest, CheckedCast_SignedToUnsigned)
{
  EXPECT_EQ(checked_cast<u8>(i8 {42}), u8 {42});

  EXPECT_EQ(checked_cast<u8>(i16 {kMinU8}), kMinU8);
  EXPECT_EQ(checked_cast<u8>(i16 {kMaxU8}), kMaxU8);

  EXPECT_EQ(checked_cast<u16>(i32 {kMinU16}), kMinU16);
  EXPECT_EQ(checked_cast<u16>(i32 {kMaxU16}), kMaxU16);

  EXPECT_THROW((void) checked_cast<u8>(i16 {-1}), std::range_error);
  EXPECT_THROW((void) checked_cast<u8>(i16 {kMaxU8 + 1}), std::range_error);
}

TEST_F(CastsTest, CheckedCast_UnsignedToSigned)
{
  EXPECT_EQ(checked_cast<i8>(u8 {42}), i8 {42});

  EXPECT_EQ(checked_cast<i8>(u16 {0}), i8 {0});
  EXPECT_EQ(checked_cast<i8>(u16 {kMaxI8}), kMaxI8);

  EXPECT_EQ(checked_cast<i16>(u32 {0}), i16 {0});
  EXPECT_EQ(checked_cast<i16>(u32 {kMaxI16}), kMaxI16);

  EXPECT_THROW((void) checked_cast<i8>(u16 {kMaxI8 + 1}), std::range_error);
}

TEST_F(CastsTest, SignedCast)
{
  static_assert(std::same_as<decltype(signed_cast(u8 {})), i8>);
  static_assert(std::same_as<decltype(signed_cast(u16 {})), i16>);
  static_assert(std::same_as<decltype(signed_cast(u32 {})), i32>);
  static_assert(std::same_as<decltype(signed_cast(u64 {})), i64>);
  static_assert(std::same_as<decltype(signed_cast(usize {})), isize>);

  EXPECT_EQ(signed_cast(0u), 0);
  EXPECT_EQ(signed_cast(u8 {100u}), i8 {100});
  EXPECT_EQ(signed_cast(u16 {18u}), i16 {18});
  EXPECT_EQ(signed_cast(u32 {42u}), i32 {42});
  EXPECT_EQ(signed_cast(usize {10'000u}), isize {10'000});

  EXPECT_THROW((void) signed_cast(u8 {128u}), std::range_error);
  EXPECT_THROW((void) signed_cast(u16 {32'768u}), std::range_error);
}

TEST_F(CastsTest, UnsignedCast)
{
  static_assert(std::same_as<decltype(unsigned_cast(i8 {})), u8>);
  static_assert(std::same_as<decltype(unsigned_cast(i16 {})), u16>);
  static_assert(std::same_as<decltype(unsigned_cast(i32 {})), u32>);
  static_assert(std::same_as<decltype(unsigned_cast(i64 {})), u64>);
  static_assert(std::same_as<decltype(unsigned_cast(isize {})), usize>);

  EXPECT_EQ(unsigned_cast(0), 0u);
  EXPECT_EQ(unsigned_cast(i8 {100}), u8 {100});
  EXPECT_EQ(unsigned_cast(i16 {18}), u16 {18});
  EXPECT_EQ(unsigned_cast(i32 {42}), u32 {42});
  EXPECT_EQ(unsigned_cast(isize {10'000}), usize {10'000});

  EXPECT_THROW((void) unsigned_cast(i8 {-1}), std::range_error);
}

TEST_F(CastsTest, SaturateCast_SignedToSigned)
{
  EXPECT_EQ(saturate_cast<i8>(i8 {42}), i8 {42});

  EXPECT_EQ(saturate_cast<i8>(i16 {kMinI8}), kMinI8);
  EXPECT_EQ(saturate_cast<i8>(i16 {kMaxI8}), kMaxI8);

  EXPECT_EQ(saturate_cast<i16>(i32 {kMinI16}), kMinI16);
  EXPECT_EQ(saturate_cast<i16>(i32 {kMaxI16}), kMaxI16);

  EXPECT_EQ(saturate_cast<i16>(i32 {kMinI16 - 1}), kMinI16);
  EXPECT_EQ(saturate_cast<i16>(i32 {kMaxI16 + 1}), kMaxI16);
}

TEST_F(CastsTest, SaturateCast_UnsignedToUnsigned)
{
  EXPECT_EQ(saturate_cast<u8>(u8 {42}), u8 {42});

  EXPECT_EQ(saturate_cast<u8>(u16 {kMinU8}), kMinU8);
  EXPECT_EQ(saturate_cast<u8>(u16 {kMaxU8}), kMaxU8);

  EXPECT_EQ(saturate_cast<u16>(u32 {kMinU16}), kMinU16);
  EXPECT_EQ(saturate_cast<u16>(u32 {kMaxU16}), kMaxU16);

  EXPECT_EQ(saturate_cast<u16>(u32 {kMaxU16 + 1}), kMaxU16);
}

TEST_F(CastsTest, SaturateCast_SignedToUnsigned)
{
  EXPECT_EQ(saturate_cast<u8>(i8 {42}), u8 {42});

  EXPECT_EQ(saturate_cast<u8>(i16 {kMinU8}), kMinU8);
  EXPECT_EQ(saturate_cast<u8>(i16 {kMaxU8}), kMaxU8);

  EXPECT_EQ(saturate_cast<u16>(i32 {kMinU16}), kMinU16);
  EXPECT_EQ(saturate_cast<u16>(i32 {kMaxU16}), kMaxU16);

  EXPECT_EQ(saturate_cast<u8>(i16 {-1}), kMinU8);
  EXPECT_EQ(saturate_cast<u8>(i16 {kMaxU8 + 1}), kMaxU8);
}

TEST_F(CastsTest, SaturateCast_UnsignedToSigned)
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
