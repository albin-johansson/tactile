// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/misc/narrow.hpp"

#include <limits>  // numeric_limits

#include <gtest/gtest.h>

using namespace tactile;

/// \tests tactile::narrow_checked
TEST(Narrow, NarrowChecked)
{
  const auto min_i32 = std::numeric_limits<int32>::min();
  const auto max_i32 = std::numeric_limits<int32>::max();
  const auto max_u32 = std::numeric_limits<uint32>::max();

  // Signed -> signed
  EXPECT_EQ(narrow_checked<int8>(int8 {42}), int8 {42});
  EXPECT_EQ(narrow_checked<int8>(int16 {100}), int8 {100});
  EXPECT_EQ(narrow_checked<int8>(int32 {-128}), int8 {-128});
  EXPECT_EQ(narrow_checked<int16>(int32 {1'234}), int16 {1'234});
  EXPECT_EQ(narrow_checked<int16>(int32 {0x7FFF}), int16 {0x7FFF});
  EXPECT_EQ(narrow_checked<int16>(int32 {-10'000}), int16 {-10'000});
  EXPECT_EQ(narrow_checked<int32>(int64 {min_i32}), min_i32);
  EXPECT_EQ(narrow_checked<int32>(int64 {max_i32}), max_i32);

  // Signed -> unsigned
  EXPECT_EQ(narrow_checked<uint8>(int8 {42}), uint8 {42});
  EXPECT_EQ(narrow_checked<uint8>(int16 {245}), uint8 {245});
  EXPECT_EQ(narrow_checked<uint16>(int32 {0x7FFF}), uint16 {0x7FFF});
  EXPECT_EQ(narrow_checked<uint16>(int32 {0x1234}), uint16 {0x1234});

  // Unsigned -> unsigned
  EXPECT_EQ(narrow_checked<uint8>(uint8 {99}), uint8 {99});
  EXPECT_EQ(narrow_checked<uint8>(uint16 {0xFA}), uint8 {0xFA});
  EXPECT_EQ(narrow_checked<uint16>(uint16 {0xFFFF}), uint16 {0xFFFF});
  EXPECT_EQ(narrow_checked<uint32>(uint64 {max_u32}), max_u32);

  // Unsigned -> signed
  EXPECT_EQ(narrow_checked<int8>(uint32 {42}), int8 {42});
  EXPECT_EQ(narrow_checked<int8>(uint32 {127}), int8 {127});
  EXPECT_EQ(narrow_checked<int16>(uint32 {0x7FFF}), int16 {0x7FFF});
  EXPECT_EQ(narrow_checked<int32>(uint64 {28}), int32 {28});
}

/// \tests tactile::narrow_checked
TEST(Narrow, NarrowCheckedWithOverflow)
{
  EXPECT_THROW((void) narrow_checked<int8>(int16 {129}), Exception);
  EXPECT_THROW((void) narrow_checked<int8>(uint16 {129}), Exception);

  EXPECT_THROW((void) narrow_checked<uint8>(int16 {256}), Exception);
  EXPECT_THROW((void) narrow_checked<uint8>(uint16 {256}), Exception);

  EXPECT_THROW((void) narrow_checked<int16>(int32 {0x8FFF}), Exception);
  EXPECT_THROW((void) narrow_checked<int16>(uint32 {0x8FFF}), Exception);
}

/// \tests tactile::narrow_checked
TEST(Narrow, NarrowCheckedWithUnderflow)
{
  const auto min_i32 = std::numeric_limits<int32>::min();

  EXPECT_THROW((void) narrow_checked<uint8>(int8 {-1}), Exception);
  EXPECT_THROW((void) narrow_checked<uint8>(int16 {-1}), Exception);
  EXPECT_THROW((void) narrow_checked<uint16>(int32 {-1}), Exception);
  EXPECT_THROW((void) narrow_checked<uint32>(int64 {-1}), Exception);

  EXPECT_THROW((void) narrow_checked<int8>(int16 {-129}), Exception);
  EXPECT_THROW((void) narrow_checked<int16>(min_i32), Exception);
  EXPECT_THROW((void) narrow_checked<int32>(int64 {min_i32} - int64 {1}), Exception);
}
