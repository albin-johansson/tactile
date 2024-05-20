// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/base/numeric/saturate_cast.hpp"

#include <limits>  // numeric_limits

#include <gtest/gtest.h>

#include "tactile/base/int.hpp"

namespace tactile {
inline namespace saturate_cast_test {

template <typename T = int8>
inline constexpr T kMinI8 = T {std::numeric_limits<int8>::min()};

template <typename T = int8>
inline constexpr T kMaxI8 = T {std::numeric_limits<int8>::max()};

template <typename T = uint8>
inline constexpr T kMaxU8 = T {std::numeric_limits<uint8>::max()};

template <typename T = int16>
inline constexpr T kMinI16 = T {std::numeric_limits<int16>::min()};

template <typename T = int16>
inline constexpr T kMaxI16 = T {std::numeric_limits<int16>::max()};

}  // namespace saturate_cast_test

/// \trace tactile::saturate_cast
TEST(SaturateCast, SignedToSignedNarrowing)
{
  EXPECT_EQ(saturate_cast<int8>(int16 {0x00}), int8 {0x00});
  EXPECT_EQ(saturate_cast<int8>(int16 {0x41}), int8 {0x41});
  EXPECT_EQ(saturate_cast<int8>(int16 {-0x2E}), int8 {-0x2E});

  EXPECT_EQ(saturate_cast<int8>(kMinI8<int16>), kMinI8<>);
  EXPECT_EQ(saturate_cast<int8>(kMinI8<int16> - int16 {1}), kMinI8<>);

  EXPECT_EQ(saturate_cast<int8>(kMaxI8<int16>), kMaxI8<>);
  EXPECT_EQ(saturate_cast<int8>(kMaxI8<int16> + int16 {1}), kMaxI8<>);
}

/// \trace tactile::saturate_cast
TEST(SaturateCast, SignedToSignedPromoting)
{
  EXPECT_EQ(saturate_cast<int32>(int16 {0}), int32 {0});
  EXPECT_EQ(saturate_cast<int32>(int16 {12'345}), 12'345);

  EXPECT_EQ(saturate_cast<int32>(kMinI16<>), kMinI16<int32>);
  EXPECT_EQ(saturate_cast<int32>(kMaxI16<>), kMaxI16<int32>);
}

/// \trace tactile::saturate_cast
TEST(SaturateCast, UnsignedToSignedNarrowing)
{
  EXPECT_EQ(saturate_cast<int8>(uint8 {0}), int8 {0});
  EXPECT_EQ(saturate_cast<int8>(uint8 {42}), int8 {42});

  EXPECT_EQ(saturate_cast<int8>(kMaxI8<uint8>), kMaxI8<>);
  EXPECT_EQ(saturate_cast<int8>(kMaxI8<uint8> + uint8 {1}), kMaxI8<>);
}

/// \trace tactile::saturate_cast
TEST(SaturateCast, UnsignedToSignedPromoting)
{
  EXPECT_EQ(saturate_cast<int16>(uint8 {0}), int16 {0});
  EXPECT_EQ(saturate_cast<int16>(uint8 {0xEA}), int16 {0xEA});

  EXPECT_EQ(saturate_cast<int16>(kMaxU8<>), kMaxU8<int16>);
}

/// \trace tactile::saturate_cast
TEST(SaturateCast, SignedToUnsignedNarrowing)
{
  EXPECT_EQ(saturate_cast<uint8>(int16 {0}), uint8 {0});
  EXPECT_EQ(saturate_cast<uint8>(int16 {-12}), uint8 {0});
  EXPECT_EQ(saturate_cast<uint8>(int16 {0x4E}), uint8 {0x4E});

  EXPECT_EQ(saturate_cast<uint8>(kMinI16<>), uint8 {0});
  EXPECT_EQ(saturate_cast<uint8>(kMaxI16<>), kMaxU8<>);

  EXPECT_EQ(saturate_cast<uint8>(kMaxU8<int16>), kMaxU8<>);
  EXPECT_EQ(saturate_cast<uint8>(kMaxU8<int16> + int16 {1}), kMaxU8<>);
}

/// \trace tactile::saturate_cast
TEST(SaturateCast, SignedToUnsignedPromoting)
{
  EXPECT_EQ(saturate_cast<uint8>(int8 {0}), uint8 {0});
  EXPECT_EQ(saturate_cast<uint8>(int8 {-1}), uint8 {0});
  EXPECT_EQ(saturate_cast<uint8>(int8 {99}), uint8 {99});

  EXPECT_EQ(saturate_cast<uint8>(kMinI8<>), uint8 {0});
  EXPECT_EQ(saturate_cast<uint8>(kMaxI8<>), kMaxI8<uint8>);
}

}  // namespace tactile
