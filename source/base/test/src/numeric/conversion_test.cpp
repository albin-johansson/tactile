// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/base/numeric/conversion.hpp"

#include <cstdint>    // uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t
#include <limits>     // numeric_limits
#include <stdexcept>  // underflow_error, overflow_error

#include <gtest/gtest.h>

namespace tactile {
namespace {

constexpr auto kMinS8 = std::numeric_limits<std::int8_t>::min();
constexpr auto kMaxS8 = std::numeric_limits<std::int8_t>::max();
constexpr auto kMinU8 = std::numeric_limits<std::uint8_t>::min();
constexpr auto kMaxU8 = std::numeric_limits<std::uint8_t>::max();

constexpr auto kMinS16 = std::numeric_limits<std::int16_t>::min();
constexpr auto kMaxS16 = std::numeric_limits<std::int16_t>::max();
constexpr auto kMinU16 = std::numeric_limits<std::uint16_t>::min();
constexpr auto kMaxU16 = std::numeric_limits<std::uint16_t>::max();

// tactile::narrow
TEST(Conversion, NarrowSignedValuesOfSameType)
{
  EXPECT_EQ(narrow<std::int8_t>(kMinS8), kMinS8);
  EXPECT_EQ(narrow<std::int8_t>(kMaxS8), kMaxS8);

  EXPECT_EQ(narrow<std::int16_t>(kMinS16), kMinS16);
  EXPECT_EQ(narrow<std::int16_t>(kMaxS16), kMaxS16);
}

// tactile::narrow
TEST(Conversion, NarrowUnsignedValuesOfSameType)
{
  EXPECT_EQ(narrow<std::uint8_t>(kMinU8), kMinU8);
  EXPECT_EQ(narrow<std::uint8_t>(kMaxU8), kMaxU8);

  EXPECT_EQ(narrow<std::uint16_t>(kMinU16), kMinU16);
  EXPECT_EQ(narrow<std::uint16_t>(kMaxU16), kMaxU16);
}

// tactile::narrow
TEST(Conversion, NarrowSignedValues)
{
  EXPECT_EQ(narrow<std::int8_t>(std::int16_t {0x12}), std::int8_t {0x12});
  EXPECT_EQ(narrow<std::int16_t>(std::int32_t {0x1234}), std::int16_t {0x1234});
}

// tactile::narrow
TEST(Conversion, NarrowUnsignedValues)
{
  EXPECT_EQ(narrow<std::uint8_t>(std::uint16_t {0xAB}), std::uint8_t {0xAB});
  EXPECT_EQ(narrow<std::uint16_t>(std::uint32_t {0xABCD}), std::uint16_t {0xABCD});
}

// tactile::narrow
TEST(Conversion, NarrowUnderflow)
{
  EXPECT_THROW((void) narrow<std::int8_t>(std::int16_t {kMinS8} - 1), std::underflow_error);
  EXPECT_THROW((void) narrow<std::int16_t>(std::int32_t {kMinS16} - 1), std::underflow_error);
}

// tactile::narrow
TEST(Conversion, NarrowOverflow)
{
  EXPECT_THROW((void) narrow<std::int8_t>(std::int16_t {kMaxS8} + 1), std::overflow_error);
  EXPECT_THROW((void) narrow<std::int16_t>(std::int32_t {kMaxS16} + 1), std::overflow_error);
}

}  // namespace
}  // namespace tactile
