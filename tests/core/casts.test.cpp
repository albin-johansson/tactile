// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/core/casts.hpp"

#include <stdexcept>

#include <gtest/gtest.h>

#include "tactile/core/primitives.hpp"

namespace tactile {
namespace {

class CastsTest : public testing::Test
{};

TEST_F(CastsTest, TrivialCast)
{
  // unsigned -> unsigned
  EXPECT_EQ(trivial_cast<uint8>(uint8 {123}), uint8 {123});
  EXPECT_EQ(trivial_cast<uint16>(uint8 {123}), uint16 {123});

  // unsigned -> signed
  EXPECT_EQ(trivial_cast<int16>(uint8 {123}), int16 {123});

  // signed -> float
  EXPECT_EQ(trivial_cast<float32>(int16 {42}), float32 {42});

  // unsigned -> float
  EXPECT_EQ(trivial_cast<float32>(uint16 {42}), float32 {42});

  // float -> float
  EXPECT_EQ(trivial_cast<float64>(float32 {0.5}), float64 {0.5});
}

TEST_F(CastsTest, TryConvertTo)
{
  EXPECT_EQ(try_convert_to<uint8>(int8 {-1}), kNone);
  EXPECT_EQ(try_convert_to<uint8>(int8 {0}), uint8 {0});
  EXPECT_EQ(try_convert_to<uint8>(int8 {127}), uint8 {127});

  EXPECT_EQ(try_convert_to<int8>(uint8 {0}), int8 {0});
  EXPECT_EQ(try_convert_to<int8>(uint8 {127}), int8 {127});
  EXPECT_EQ(try_convert_to<int8>(uint8 {128}), kNone);

  EXPECT_EQ(try_convert_to<int32>(int32 {42}), int32 {42});
  EXPECT_EQ(try_convert_to<int8>(uint64 {42}), int8 {42});
}

TEST_F(CastsTest, ConvertTo)
{
  EXPECT_THROW(convert_to<uint8>(int8 {-1}), std::range_error);
  EXPECT_EQ(convert_to<uint8>(int8 {0}), uint8 {0});
  EXPECT_EQ(convert_to<uint8>(int8 {127}), uint8 {127});

  EXPECT_EQ(convert_to<int8>(uint8 {0}), int8 {0});
  EXPECT_EQ(convert_to<int8>(uint8 {127}), int8 {127});
  EXPECT_THROW(convert_to<int8>(uint8 {128}), std::range_error);

  EXPECT_EQ(convert_to<int32>(int32 {42}), int32 {42});
  EXPECT_EQ(convert_to<int8>(uint64 {42}), int8 {42});
}

}  // namespace
}  // namespace tactile
