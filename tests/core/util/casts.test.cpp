// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/core/util/casts.hpp"

#include <gtest/gtest.h>

#include "tactile/core/primitives.hpp"

namespace tactile {
namespace {

class CastsTest : public testing::Test
{};

TEST_F(CastsTest, TrivialCast)
{
  EXPECT_EQ(trivial_cast<uint8>(uint8 {123}), uint8 {123});
  EXPECT_EQ(trivial_cast<uint16>(uint8 {123}), uint16 {123});
  EXPECT_EQ(trivial_cast<int16>(uint8 {123}), int16 {123});
}

TEST_F(CastsTest, CheckedCast)
{
  EXPECT_EQ(checked_cast<uint8>(int8 {-1}), kNone);
  EXPECT_EQ(checked_cast<uint8>(int8 {0}), uint8 {0});
  EXPECT_EQ(checked_cast<uint8>(int8 {127}), uint8 {127});

  EXPECT_EQ(checked_cast<int8>(uint8 {0}), int8 {0});
  EXPECT_EQ(checked_cast<int8>(uint8 {127}), int8 {127});
  EXPECT_EQ(checked_cast<int8>(uint8 {128}), kNone);

  EXPECT_EQ(checked_cast<int32>(int32 {42}), int32 {42});
  EXPECT_EQ(checked_cast<int8>(uint64 {42}), int8 {42});
}

}  // namespace
}  // namespace tactile
