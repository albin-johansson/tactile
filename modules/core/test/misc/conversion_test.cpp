// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/misc/conversion.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(Conversion, AsSigned)
{
  const uint8 u8 {53};
  const uint16 u16 {8'273};
  const uint32 u32 {832'123};
  const uint64 u64 {93'443'392};

  const auto s8 = as_signed(u8);
  const auto s16 = as_signed(u16);
  const auto s32 = as_signed(u32);
  const auto s64 = as_signed(u64);

  EXPECT_EQ(s8, static_cast<int8>(u8));
  EXPECT_EQ(s16, static_cast<int16>(u16));
  EXPECT_EQ(s32, static_cast<int32>(u32));
  EXPECT_EQ(s64, static_cast<int64>(u64));

  EXPECT_EQ(sizeof u8, sizeof s8);
  EXPECT_EQ(sizeof u16, sizeof s16);
  EXPECT_EQ(sizeof u32, sizeof s32);
  EXPECT_EQ(sizeof u64, sizeof s64);
}

TEST(Conversion, AsUnsigned)
{
  const int8 s8 {-1};
  const int16 s16 {839};
  const int32 s32 {92'761};
  const int64 s64 {649'231};

  const auto u8 = as_unsigned(s8);
  const auto u16 = as_unsigned(s16);
  const auto u32 = as_unsigned(s32);
  const auto u64 = as_unsigned(s64);

  EXPECT_EQ(u8, static_cast<uint8>(s8));
  EXPECT_EQ(u16, static_cast<uint16>(s16));
  EXPECT_EQ(u32, static_cast<uint32>(s32));
  EXPECT_EQ(u64, static_cast<uint64>(s64));

  EXPECT_EQ(sizeof s8, sizeof u8);
  EXPECT_EQ(sizeof s16, sizeof u16);
  EXPECT_EQ(sizeof s32, sizeof u32);
  EXPECT_EQ(sizeof s64, sizeof u64);
}
