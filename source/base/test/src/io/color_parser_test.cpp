// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/base/io/color_parser.hpp"

#include <gtest/gtest.h>

namespace tactile::test {

class ColorParserTest : public testing::Test
{
 public:
  const UColor kBlack {0x00, 0x00, 0x00, 0xFF};
  const UColor kWhite {0xFF, 0xFF, 0xFF, 0xFF};
  const UColor kLimeGreen {0x32, 0xCD, 0x32, 0xFF};
  const UColor kDarkBlue {0x00, 0x00, 0x8B, 0xFF};
  const UColor kHotPink {0xFF, 0x69, 0xB4, 0xFF};
};

// tactile::parse_color_rgb
TEST_F(ColorParserTest, ParseColorRGB)
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

// tactile::parse_color_rgba
TEST_F(ColorParserTest, ParseColorRGBA)
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

// tactile::parse_color_argb
TEST_F(ColorParserTest, ParseColorARGB)
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

}  // namespace tactile::test
