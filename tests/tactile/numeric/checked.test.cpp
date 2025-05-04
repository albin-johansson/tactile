// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#include <gtest/gtest.h>

import std;
import tactile.numeric;

namespace tactile::tests {

TEST(Numeric_Checked, CheckedAdd_SignedInt)
{
  // Valid
  EXPECT_EQ(checked_add(1'234, 4'321).value(), 5'555);
  EXPECT_EQ(checked_add(150, -300).value(), -150);
  EXPECT_EQ(checked_add(kMaxI32, i32 {0u}).value(), kMaxI32);
  EXPECT_EQ(checked_add(kMaxI64, kMinI64).value(), -1);

  // Overflow
  EXPECT_EQ(checked_add(kMaxI32, kMaxI32).error_or(Error::kUnknown),
            Error::kArithmeticOverflow);
  EXPECT_EQ(checked_add(kMaxI32, i32 {1}).error_or(Error::kUnknown),
            Error::kArithmeticOverflow);

  // Underflow
  EXPECT_EQ(checked_add(kMinI32, i32 {-1}).error_or(Error::kUnknown),
            Error::kArithmeticUnderflow);
}

TEST(Numeric_Checked, CheckedAdd_UnsignedInt)
{
  // Valid
  EXPECT_EQ(checked_add(1'234u, 4'321u).value(), 5'555u);
  EXPECT_EQ(checked_add(kMaxU16, u16 {0u}).value(), kMaxU16);

  // Overflow
  EXPECT_EQ(checked_add(kMaxU8, kMaxU8).error_or(Error::kUnknown),
            Error::kArithmeticOverflow);
  EXPECT_EQ(checked_add(kMaxU64, u64 {1u}).error_or(Error::kUnknown),
            Error::kArithmeticOverflow);
}

TEST(Numeric_Checked, CheckedSub_SignedInt)
{
  // Valid
  EXPECT_EQ(checked_sub(987, 123).value(), 864);
  EXPECT_EQ(checked_sub(100, -10).value(), 110);
  EXPECT_EQ(checked_sub(-50, 20).value(), -70);
  EXPECT_EQ(checked_sub(kMinI32, i32 {0}).value(), kMinI32);

  // Overflow
  EXPECT_EQ(checked_sub(kMaxI32, i32 {-1}).error_or(Error::kUnknown),
            Error::kArithmeticOverflow);

  // Underflow
  EXPECT_EQ(checked_sub(kMinI16, i16 {1}).error_or(Error::kUnknown),
            Error::kArithmeticUnderflow);
}

TEST(Numeric_Checked, CheckedSub_UnsignedInt)
{
  // Valid
  EXPECT_EQ(checked_sub(150u, 100u).value(), 50u);
  EXPECT_EQ(checked_sub(kMaxU32, kMaxU32).value(), kMinU32);
  EXPECT_EQ(checked_sub(kMinU8, kMinU8).value(), kMinU8);

  // Underflow
  EXPECT_EQ(checked_sub(kMinU64, u64 {1u}).error_or(Error::kUnknown),
            Error::kArithmeticUnderflow);
}

TEST(Numeric_Checked, CheckedMul_SignedInt)
{
  // Valid
  EXPECT_EQ(checked_mul(25, 4).value(), 100);
  EXPECT_EQ(checked_mul(100, -10).value(), -1'000);
  EXPECT_EQ(checked_mul(999, 0).value(), 0);
  EXPECT_EQ(checked_mul(kMaxI16, i16 {1}).value(), kMaxI16);

  // Underflow
  EXPECT_EQ(checked_mul(kMinI32, i32 {2}).error_or(Error::kUnknown),
            Error::kArithmeticUnderflow);

  // Overflow
  EXPECT_EQ(checked_mul(kMaxI8, i8 {2}).error_or(Error::kUnknown),
            Error::kArithmeticOverflow);
}

TEST(Numeric_Checked, CheckedMul_UnsignedInt)
{
  // Valid
  EXPECT_EQ(checked_mul(25u, 4u).value(), 100u);
  EXPECT_EQ(checked_mul(999u, 0u).value(), 0u);
  EXPECT_EQ(checked_mul(kMaxU16, u16 {1u}).value(), kMaxU16);

  // Overflow
  EXPECT_EQ(checked_mul(kMaxU16, u16 {2u}).error_or(Error::kUnknown),
            Error::kArithmeticOverflow);
}

TEST(Numeric_Checked, CheckedDiv_SignedInt)
{
  // Valid
  EXPECT_EQ(checked_div(100, 4).value(), 25);
  EXPECT_EQ(checked_div(400, -2).value(), -200);
  EXPECT_EQ(checked_div(-50, 5).value(), -10);
  EXPECT_EQ(checked_div(999, 1).value(), 999);
  EXPECT_EQ(checked_div(1, 2).value(), 0);

  // Division by zero
  EXPECT_EQ(checked_div(42, 0).error_or(Error::kUnknown),
            Error::kArithmeticInvalidValue);
}

TEST(Numeric_Checked, CheckedDiv_UnsignedInt)
{
  // Valid
  EXPECT_EQ(checked_div(100u, 4u).value(), 25u);
  EXPECT_EQ(checked_div(999u, 1u).value(), 999u);
  EXPECT_EQ(checked_div(1u, 2u).value(), 0u);

  // Division by zero
  EXPECT_EQ(checked_div(42u, 0u).error_or(Error::kUnknown),
            Error::kArithmeticInvalidValue);
}

}  // namespace tactile::tests
