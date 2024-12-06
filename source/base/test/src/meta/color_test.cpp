// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/base/meta/color.hpp"

#include <gtest/gtest.h>

namespace tactile::core {
namespace {

inline constexpr UColor kLimeGreen {0x32, 0xCD, 0x32, 0xFF};
inline constexpr UColor kDarkBlue {0x00, 0x00, 0x8B, 0xFF};
inline constexpr UColor kHotPink {0xFF, 0x69, 0xB4, 0xFF};

// tactile::to_uint32_abgr [UColor]
TEST(Color, ToUInt32ABGR)
{
  EXPECT_EQ(to_uint32_abgr(kColorBlack), std::uint32_t {0xFF000000});
  EXPECT_EQ(to_uint32_abgr(kColorWhite), std::uint32_t {0xFFFFFFFF});
  EXPECT_EQ(to_uint32_abgr(kLimeGreen), std::uint32_t {0xFF32CD32});
  EXPECT_EQ(to_uint32_abgr(kDarkBlue), std::uint32_t {0xFF8B0000});
  EXPECT_EQ(to_uint32_abgr(kHotPink), std::uint32_t {0xFFB469FF});
}

// tactile::to_fcolor
TEST(Color, ToFColor)
{
  constexpr UColor color {0xFF, 0x00, 0x33, 0x66};
  const auto normalized_color = to_fcolor(color);

  EXPECT_FLOAT_EQ(normalized_color.red, 1.0f);
  EXPECT_FLOAT_EQ(normalized_color.green, 0.0f);
  EXPECT_FLOAT_EQ(normalized_color.blue, 0.2f);
  EXPECT_FLOAT_EQ(normalized_color.alpha, 0.4f);
}

}  // namespace
}  // namespace tactile::core
