// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/common/meta/colors.hpp"

#include <gtest/gtest.h>

namespace tactile::common {
namespace {

inline constexpr UColor kBlack {0x00, 0x00, 0x00, 0xFF};
inline constexpr UColor kWhite {0xFF, 0xFF, 0xFF, 0xFF};
inline constexpr UColor kLimeGreen {0x32, 0xCD, 0x32, 0xFF};
inline constexpr UColor kDarkBlue {0x00, 0x00, 0x8B, 0xFF};
inline constexpr UColor kHotPink {0xFF, 0x69, 0xB4, 0xFF};

// tactile::common::to_string [UColor]
TEST(Colors, ToStringRGB)
{
  EXPECT_EQ(to_string(kColorBlack, ColorFormat::kRgb), "#000000");
  EXPECT_EQ(to_string(kColorWhite, ColorFormat::kRgb), "#FFFFFF");
  EXPECT_EQ(to_string(kLimeGreen, ColorFormat::kRgb), "#32CD32");
  EXPECT_EQ(to_string(kDarkBlue, ColorFormat::kRgb), "#00008B");
  EXPECT_EQ(to_string(kHotPink, ColorFormat::kRgb), "#FF69B4");

  EXPECT_EQ(to_string(UColor {0x1A, 0x2B, 0x3C, 0x4D}, ColorFormat::kRgb), "#1A2B3C");
  EXPECT_EQ(to_string(UColor {0xAB, 0xCD, 0xEF, 0x42}, ColorFormat::kRgb), "#ABCDEF");
}

// tactile::common::to_string [UColor]
TEST(Colors, ToStringRGBA)
{
  EXPECT_EQ(to_string(kColorBlack, ColorFormat::kRgba), "#000000FF");
  EXPECT_EQ(to_string(kColorWhite, ColorFormat::kRgba), "#FFFFFFFF");
  EXPECT_EQ(to_string(kLimeGreen, ColorFormat::kRgba), "#32CD32FF");
  EXPECT_EQ(to_string(kDarkBlue, ColorFormat::kRgba), "#00008BFF");
  EXPECT_EQ(to_string(kHotPink, ColorFormat::kRgba), "#FF69B4FF");

  EXPECT_EQ(to_string(UColor {0x1A, 0x2B, 0x3C, 0x4D}, ColorFormat::kRgba), "#1A2B3C4D");
  EXPECT_EQ(to_string(UColor {0xAB, 0xCD, 0xEF, 0x42}, ColorFormat::kRgba), "#ABCDEF42");
}

// tactile::common::to_string [UColor]
TEST(Colors, ToStringARGB)
{
  EXPECT_EQ(to_string(kColorBlack, ColorFormat::kArgb), "#FF000000");
  EXPECT_EQ(to_string(kColorWhite, ColorFormat::kArgb), "#FFFFFFFF");
  EXPECT_EQ(to_string(kLimeGreen, ColorFormat::kArgb), "#FF32CD32");
  EXPECT_EQ(to_string(kDarkBlue, ColorFormat::kArgb), "#FF00008B");
  EXPECT_EQ(to_string(kHotPink, ColorFormat::kArgb), "#FFFF69B4");

  EXPECT_EQ(to_string(UColor {0x1A, 0x2B, 0x3C, 0x4D}, ColorFormat::kArgb), "#4D1A2B3C");
  EXPECT_EQ(to_string(UColor {0xAB, 0xCD, 0xEF, 0x42}, ColorFormat::kArgb), "#42ABCDEF");
}

// tactile::common::parse_color_rgb
TEST(Colors, ParseColorRGB)
{
  EXPECT_FALSE(parse_color_rgb("").has_value());
  EXPECT_FALSE(parse_color_rgb("000000").has_value());    // no hashtag
  EXPECT_FALSE(parse_color_rgb("#0000000").has_value());  // too long
  EXPECT_FALSE(parse_color_rgb("#G00000").has_value());   // invalid digit
  EXPECT_FALSE(parse_color_rgb("#00G000").has_value());   // invalid digit
  EXPECT_FALSE(parse_color_rgb("#0000G0").has_value());   // invalid digit

  EXPECT_EQ(parse_color_rgb("#F00000"), (UColor {0xF0, 0x00, 0x00, 0xFF}));
  EXPECT_EQ(parse_color_rgb("#00F000"), (UColor {0x00, 0xF0, 0x00, 0xFF}));
  EXPECT_EQ(parse_color_rgb("#0000F0"), (UColor {0x00, 0x00, 0xF0, 0xFF}));

  EXPECT_EQ(parse_color_rgb("#000000"), kBlack);
  EXPECT_EQ(parse_color_rgb("#FFFFFF"), kWhite);
  EXPECT_EQ(parse_color_rgb("#32CD32"), kLimeGreen);
  EXPECT_EQ(parse_color_rgb("#00008B"), kDarkBlue);
  EXPECT_EQ(parse_color_rgb("#FF69B4"), kHotPink);
}

// tactile::common::parse_color_rgba
TEST(Colors, ParseColorRGBA)
{
  EXPECT_FALSE(parse_color_rgba("").has_value());
  EXPECT_FALSE(parse_color_rgba("00000000").has_value());    // no hashtag
  EXPECT_FALSE(parse_color_rgba("#000000000").has_value());  // too long
  EXPECT_FALSE(parse_color_rgba("#G0000000").has_value());   // invalid digit
  EXPECT_FALSE(parse_color_rgba("#00G00000").has_value());   // invalid digit
  EXPECT_FALSE(parse_color_rgba("#0000G000").has_value());   // invalid digit
  EXPECT_FALSE(parse_color_rgba("#000000G0").has_value());   // invalid digit

  EXPECT_EQ(parse_color_rgba("#F00000FF"), (UColor {0xF0, 0x00, 0x00, 0xFF}));
  EXPECT_EQ(parse_color_rgba("#00F000FF"), (UColor {0x00, 0xF0, 0x00, 0xFF}));
  EXPECT_EQ(parse_color_rgba("#0000F0FF"), (UColor {0x00, 0x00, 0xF0, 0xFF}));

  EXPECT_EQ(parse_color_rgba("#000000FF"), kBlack);
  EXPECT_EQ(parse_color_rgba("#FFFFFFFF"), kWhite);
  EXPECT_EQ(parse_color_rgba("#32CD32FF"), kLimeGreen);
  EXPECT_EQ(parse_color_rgba("#00008BFF"), kDarkBlue);
  EXPECT_EQ(parse_color_rgba("#FF69B4FF"), kHotPink);
}

// tactile::common::parse_color_argb
TEST(Colors, ParseColorARGB)
{
  EXPECT_FALSE(parse_color_argb("").has_value());
  EXPECT_FALSE(parse_color_argb("00000000").has_value());    // no hashtag
  EXPECT_FALSE(parse_color_argb("#000000000").has_value());  // too long
  EXPECT_FALSE(parse_color_argb("#G0000000").has_value());   // invalid digit
  EXPECT_FALSE(parse_color_argb("#00G00000").has_value());   // invalid digit
  EXPECT_FALSE(parse_color_argb("#0000G000").has_value());   // invalid digit
  EXPECT_FALSE(parse_color_argb("#000000G0").has_value());   // invalid digit

  EXPECT_EQ(parse_color_argb("#FFF00000"), (UColor {0xF0, 0x00, 0x00, 0xFF}));
  EXPECT_EQ(parse_color_argb("#FF00F000"), (UColor {0x00, 0xF0, 0x00, 0xFF}));
  EXPECT_EQ(parse_color_argb("#FF0000F0"), (UColor {0x00, 0x00, 0xF0, 0xFF}));

  EXPECT_EQ(parse_color_argb("#FF000000"), kBlack);
  EXPECT_EQ(parse_color_argb("#FFFFFFFF"), kWhite);
  EXPECT_EQ(parse_color_argb("#FF32CD32"), kLimeGreen);
  EXPECT_EQ(parse_color_argb("#FF00008B"), kDarkBlue);
  EXPECT_EQ(parse_color_argb("#FFFF69B4"), kHotPink);
}

// tactile::common::parse_color_rgb
// tactile::common::to_string [UColor]
TEST(Colors, RoundTripConversionRGB)
{
  EXPECT_EQ(parse_color_rgb(to_string(kHotPink, ColorFormat::kRgb)), kHotPink);
}

// tactile::common::parse_color_rgba
// tactile::common::to_string [UColor]
TEST(Colors, RoundTripConversionRGBA)
{
  EXPECT_EQ(parse_color_rgba(to_string(kDarkBlue, ColorFormat::kRgba)), kDarkBlue);
}

// tactile::common::parse_color_argb
// tactile::common::to_string [UColor]
TEST(Colors, RoundTripConversionARGB)
{
  EXPECT_EQ(parse_color_argb(to_string(kLimeGreen, ColorFormat::kArgb)), kLimeGreen);
}

}  // namespace
}  // namespace tactile::common
