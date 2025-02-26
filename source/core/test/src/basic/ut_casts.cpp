// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#include <stdexcept>  // range_error

#include <gtest/gtest.h>

#include "tactile/core/basic/casts.hpp"
#include "tactile/core/basic/primitives.hpp"

namespace tactile {
namespace {

TEST(CastsTest, SaturateCast)
{
  // Underflow
  EXPECT_EQ(saturate_cast<u8>(i8 {-1}), u8 {0});
  EXPECT_EQ(saturate_cast<u16>(i32 {-1}), u16 {0});

  // Overflow
  EXPECT_EQ(saturate_cast<u8>(u32 {256}), u8 {255});

  // In range
  EXPECT_EQ(saturate_cast<u8>(u16 {123}), u8 {123});
  EXPECT_EQ(saturate_cast<u16>(u8 {123}), u16 {123});
}

TEST(CastsTest, NarrowCast)
{
  // Underflow
  EXPECT_THROW((void) narrow_cast<u8>(i8 {-1}), std::range_error);
  EXPECT_THROW((void) narrow_cast<u16>(i32 {-1}), std::range_error);

  // Overflow
  EXPECT_THROW((void) narrow_cast<u8>(u32 {256}), std::range_error);

  // In range
  EXPECT_EQ(narrow_cast<u8>(u16 {123}), u8 {123});
  EXPECT_EQ(narrow_cast<u16>(u8 {123}), u16 {123});
}

}  // namespace
}  // namespace tactile
