// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/misc/integer_conversion.hpp"

#include <limits>  // numeric_limits

#include <gtest/gtest.h>

using namespace tactile;

namespace {

inline constexpr auto kMinI32 = std::numeric_limits<int32>::min();
inline constexpr auto kMaxI32 = std::numeric_limits<int32>::max();
inline constexpr auto kMaxU32 = std::numeric_limits<uint32>::max();

}  // namespace

/// \tests tactile::narrow
TEST(IntegerConversion, Narrow)
{
  // Signed -> signed
  EXPECT_EQ(narrow<int8>(int8 {42}), int8 {42});
  EXPECT_EQ(narrow<int8>(int16 {100}), int8 {100});
  EXPECT_EQ(narrow<int8>(int32 {-128}), int8 {-128});
  EXPECT_EQ(narrow<int16>(int32 {1'234}), int16 {1'234});
  EXPECT_EQ(narrow<int16>(int32 {0x7FFF}), int16 {0x7FFF});
  EXPECT_EQ(narrow<int16>(int32 {-10'000}), int16 {-10'000});
  EXPECT_EQ(narrow<int32>(int64 {kMinI32}), kMinI32);
  EXPECT_EQ(narrow<int32>(int64 {kMaxI32}), kMaxI32);

  // Signed -> unsigned
  EXPECT_EQ(narrow<uint8>(int8 {42}), uint8 {42});
  EXPECT_EQ(narrow<uint8>(int16 {245}), uint8 {245});
  EXPECT_EQ(narrow<uint16>(int32 {0x7FFF}), uint16 {0x7FFF});
  EXPECT_EQ(narrow<uint16>(int32 {0x1234}), uint16 {0x1234});

  // Unsigned -> unsigned
  EXPECT_EQ(narrow<uint8>(uint8 {99}), uint8 {99});
  EXPECT_EQ(narrow<uint8>(uint16 {0xFA}), uint8 {0xFA});
  EXPECT_EQ(narrow<uint16>(uint16 {0xFFFF}), uint16 {0xFFFF});
  EXPECT_EQ(narrow<uint32>(uint64 {kMaxU32}), kMaxU32);

  // Unsigned -> signed
  EXPECT_EQ(narrow<int8>(uint32 {42}), int8 {42});
  EXPECT_EQ(narrow<int8>(uint32 {127}), int8 {127});
  EXPECT_EQ(narrow<int16>(uint32 {0x7FFF}), int16 {0x7FFF});
  EXPECT_EQ(narrow<int32>(uint64 {28}), int32 {28});
}

/// \tests tactile::narrow
TEST(IntegerConversion, NarrowWithOverflow)
{
  EXPECT_THROW((void) narrow<int8>(int16 {129}), Exception);
  EXPECT_THROW((void) narrow<int8>(uint16 {129}), Exception);

  EXPECT_THROW((void) narrow<uint8>(int16 {256}), Exception);
  EXPECT_THROW((void) narrow<uint8>(uint16 {256}), Exception);

  EXPECT_THROW((void) narrow<int16>(int32 {0x8FFF}), Exception);
  EXPECT_THROW((void) narrow<int16>(uint32 {0x8FFF}), Exception);
}

/// \tests tactile::narrow
TEST(IntegerConversion, NarrowWithUnderflow)
{
  EXPECT_THROW((void) narrow<uint8>(int8 {-1}), Exception);
  EXPECT_THROW((void) narrow<uint8>(int16 {-1}), Exception);
  EXPECT_THROW((void) narrow<uint16>(int32 {-1}), Exception);
  EXPECT_THROW((void) narrow<uint32>(int64 {-1}), Exception);

  EXPECT_THROW((void) narrow<int8>(int16 {-129}), Exception);
  EXPECT_THROW((void) narrow<int16>(kMinI32), Exception);
  EXPECT_THROW((void) narrow<int32>(int64 {kMinI32} - int64 {1}), Exception);
}

/// \tests tactile::as_signed
TEST(IntegerConversion, AsSigned)
{
  EXPECT_EQ(as_signed(0u), 0);
  EXPECT_EQ(as_signed(42u), 42);
  EXPECT_EQ(as_signed(uint8 {53}), int8 {53});
  EXPECT_EQ(as_signed(uint16 {8'273}), int16 {8'273});
  EXPECT_EQ(as_signed(static_cast<uint32>(kMaxI32)), kMaxI32);

  EXPECT_THROW((void) as_signed(static_cast<uint32>(kMaxI32) + 1), Exception);
  EXPECT_THROW((void) as_signed(kMaxU32), Exception);
}

/// \tests tactile::as_unsigned
TEST(IntegerConversion, AsUnsigned)
{
  EXPECT_EQ(as_unsigned(0), 0u);
  EXPECT_EQ(as_unsigned(42), 42u);
  EXPECT_EQ(as_unsigned(int8 {0x7F}), uint8 {0x7F});
  EXPECT_EQ(as_unsigned(kMaxI32), static_cast<uint32>(kMaxI32));

  EXPECT_THROW((void) as_unsigned(-1), Exception);
  EXPECT_THROW((void) as_unsigned(kMinI32), Exception);
}
