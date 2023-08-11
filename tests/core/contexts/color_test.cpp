/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "tactile/core/contexts/color.hpp"

#include <gtest/gtest.h>

using namespace tactile;

namespace {

inline constexpr UColor kBlack {0x00, 0x00, 0x00, 0xFF};
inline constexpr UColor kWhite {0xFF, 0xFF, 0xFF, 0xFF};
inline constexpr UColor kLimeGreen {0x32, 0xCD, 0x32, 0xFF};
inline constexpr UColor kDarkBlue {0x00, 0x00, 0x8B, 0xFF};
inline constexpr UColor kHotPink {0xFF, 0x69, 0xB4, 0xFF};

}  // namespace

TEST(Color, UColorDefaults)
{
  const UColor color;
  EXPECT_EQ(color.red, 0);
  EXPECT_EQ(color.green, 0);
  EXPECT_EQ(color.blue, 0);
  EXPECT_EQ(color.alpha, 255);
}

TEST(Color, NColorDefaults)
{
  const NColor color;
  EXPECT_EQ(color.red, 0.0f);
  EXPECT_EQ(color.green, 0.0f);
  EXPECT_EQ(color.blue, 0.0f);
  EXPECT_EQ(color.alpha, 1.0f);
}

TEST(Color, ToColorRGB)
{
  EXPECT_FALSE(to_color_rgb("").has_value());
  EXPECT_FALSE(to_color_rgb("000000").has_value());    // no hashtag
  EXPECT_FALSE(to_color_rgb("#0000000").has_value());  // too long
  EXPECT_FALSE(to_color_rgb("#G00000").has_value());   // invalid digit
  EXPECT_FALSE(to_color_rgb("#00G000").has_value());   // invalid digit
  EXPECT_FALSE(to_color_rgb("#0000G0").has_value());   // invalid digit

  EXPECT_EQ(to_color_rgb("#F00000"), (UColor {0xF0, 0x00, 0x00, 0xFF}));
  EXPECT_EQ(to_color_rgb("#00F000"), (UColor {0x00, 0xF0, 0x00, 0xFF}));
  EXPECT_EQ(to_color_rgb("#0000F0"), (UColor {0x00, 0x00, 0xF0, 0xFF}));

  EXPECT_EQ(to_color_rgb("#000000"), kBlack);
  EXPECT_EQ(to_color_rgb("#FFFFFF"), kWhite);
  EXPECT_EQ(to_color_rgb("#32CD32"), kLimeGreen);
  EXPECT_EQ(to_color_rgb("#00008B"), kDarkBlue);
  EXPECT_EQ(to_color_rgb("#FF69B4"), kHotPink);
}

TEST(Color, ToColorRGBA)
{
  EXPECT_FALSE(to_color_rgba("").has_value());
  EXPECT_FALSE(to_color_rgba("00000000").has_value());    // no hashtag
  EXPECT_FALSE(to_color_rgba("#000000000").has_value());  // too long
  EXPECT_FALSE(to_color_rgba("#G0000000").has_value());   // invalid digit
  EXPECT_FALSE(to_color_rgba("#00G00000").has_value());   // invalid digit
  EXPECT_FALSE(to_color_rgba("#0000G000").has_value());   // invalid digit
  EXPECT_FALSE(to_color_rgba("#000000G0").has_value());   // invalid digit

  EXPECT_EQ(to_color_rgba("#F00000FF"), (UColor {0xF0, 0x00, 0x00, 0xFF}));
  EXPECT_EQ(to_color_rgba("#00F000FF"), (UColor {0x00, 0xF0, 0x00, 0xFF}));
  EXPECT_EQ(to_color_rgba("#0000F0FF"), (UColor {0x00, 0x00, 0xF0, 0xFF}));

  EXPECT_EQ(to_color_rgba("#000000FF"), kBlack);
  EXPECT_EQ(to_color_rgba("#FFFFFFFF"), kWhite);
  EXPECT_EQ(to_color_rgba("#32CD32FF"), kLimeGreen);
  EXPECT_EQ(to_color_rgba("#00008BFF"), kDarkBlue);
  EXPECT_EQ(to_color_rgba("#FF69B4FF"), kHotPink);
}

TEST(Color, ToColorARGB)
{
  EXPECT_FALSE(to_color_argb("").has_value());
  EXPECT_FALSE(to_color_argb("00000000").has_value());    // no hashtag
  EXPECT_FALSE(to_color_argb("#000000000").has_value());  // too long
  EXPECT_FALSE(to_color_argb("#G0000000").has_value());   // invalid digit
  EXPECT_FALSE(to_color_argb("#00G00000").has_value());   // invalid digit
  EXPECT_FALSE(to_color_argb("#0000G000").has_value());   // invalid digit
  EXPECT_FALSE(to_color_argb("#000000G0").has_value());   // invalid digit

  EXPECT_EQ(to_color_argb("#FFF00000"), (UColor {0xF0, 0x00, 0x00, 0xFF}));
  EXPECT_EQ(to_color_argb("#FF00F000"), (UColor {0x00, 0xF0, 0x00, 0xFF}));
  EXPECT_EQ(to_color_argb("#FF0000F0"), (UColor {0x00, 0x00, 0xF0, 0xFF}));

  EXPECT_EQ(to_color_argb("#FF000000"), kBlack);
  EXPECT_EQ(to_color_argb("#FFFFFFFF"), kWhite);
  EXPECT_EQ(to_color_argb("#FF32CD32"), kLimeGreen);
  EXPECT_EQ(to_color_argb("#FF00008B"), kDarkBlue);
  EXPECT_EQ(to_color_argb("#FFFF69B4"), kHotPink);
}

TEST(Color, Normalize)
{
  const UColor color {0xFF, 0x00, 0x33, 0x66};
  const auto converted = normalize(color);

  EXPECT_FLOAT_EQ(converted.red, 1.0f);
  EXPECT_FLOAT_EQ(converted.green, 0.0f);
  EXPECT_FLOAT_EQ(converted.blue, 0.2f);
  EXPECT_FLOAT_EQ(converted.alpha, 0.4f);
}

TEST(Color, Unnormalize)
{
  const NColor color {1.0f, 0.0f, 0.2f, 0.4f};
  const auto converted = unnormalize(color);

  EXPECT_EQ(converted.red, 0xFF);
  EXPECT_EQ(converted.green, 0x00);
  EXPECT_EQ(converted.blue, 0x33);
  EXPECT_EQ(converted.alpha, 0x66);
}

TEST(Color, GetLuminance)
{
  // Based on https://planetcalc.com/7778/
  EXPECT_EQ(get_luminance(normalize(kBlack)), 0.0f);
  EXPECT_EQ(get_luminance(normalize(kWhite)), 1.0f);
  EXPECT_FLOAT_EQ(get_luminance(normalize(kLimeGreen)), 0.4457104f);
  EXPECT_FLOAT_EQ(get_luminance(normalize(kDarkBlue)), 0.0186408f);
  EXPECT_FLOAT_EQ(get_luminance(normalize(kHotPink)), 0.3465843f);
}

TEST(Color, IsDarkColor)
{
  EXPECT_TRUE(is_dark_color(normalize(kBlack)));
  EXPECT_TRUE(is_dark_color(normalize(kDarkBlue)));

  EXPECT_FALSE(is_dark_color(normalize(kWhite)));
  EXPECT_FALSE(is_dark_color(normalize(kLimeGreen)));
  EXPECT_FALSE(is_dark_color(normalize(kHotPink)));
}

TEST(Color, ToStringRGB)
{
  EXPECT_EQ(to_string_rgb(kBlack), "#000000");
  EXPECT_EQ(to_string_rgb(kWhite), "#FFFFFF");
  EXPECT_EQ(to_string_rgb(kLimeGreen), "#32CD32");
  EXPECT_EQ(to_string_rgb(kDarkBlue), "#00008B");
  EXPECT_EQ(to_string_rgb(kHotPink), "#FF69B4");

  EXPECT_EQ(to_string_rgb(UColor {0x1A, 0x2B, 0x3C, 0x4D}), "#1A2B3C");
  EXPECT_EQ(to_string_rgb(UColor {0xAB, 0xCD, 0xEF, 0x42}), "#ABCDEF");
}

TEST(Color, ToStringRGBA)
{
  EXPECT_EQ(to_string_rgba(kBlack), "#000000FF");
  EXPECT_EQ(to_string_rgba(kWhite), "#FFFFFFFF");
  EXPECT_EQ(to_string_rgba(kLimeGreen), "#32CD32FF");
  EXPECT_EQ(to_string_rgba(kDarkBlue), "#00008BFF");
  EXPECT_EQ(to_string_rgba(kHotPink), "#FF69B4FF");

  EXPECT_EQ(to_string_rgba(UColor {0x1A, 0x2B, 0x3C, 0x4D}), "#1A2B3C4D");
  EXPECT_EQ(to_string_rgba(UColor {0xAB, 0xCD, 0xEF, 0x42}), "#ABCDEF42");
}

TEST(Color, ToStringARGB)
{
  EXPECT_EQ(to_string_argb(kBlack), "#FF000000");
  EXPECT_EQ(to_string_argb(kWhite), "#FFFFFFFF");
  EXPECT_EQ(to_string_argb(kLimeGreen), "#FF32CD32");
  EXPECT_EQ(to_string_argb(kDarkBlue), "#FF00008B");
  EXPECT_EQ(to_string_argb(kHotPink), "#FFFF69B4");

  EXPECT_EQ(to_string_argb(UColor {0x1A, 0x2B, 0x3C, 0x4D}), "#4D1A2B3C");
  EXPECT_EQ(to_string_argb(UColor {0xAB, 0xCD, 0xEF, 0x42}), "#42ABCDEF");
}

TEST(Color, RoundTripConversionRGB)
{
  EXPECT_EQ(to_color_rgb(to_string_rgb(kHotPink)), kHotPink);
}

TEST(Color, RoundTripConversionRGBA)
{
  EXPECT_EQ(to_color_rgba(to_string_rgba(kDarkBlue)), kDarkBlue);
}

TEST(Color, RoundTripConversionARGB)
{
  EXPECT_EQ(to_color_argb(to_string_argb(kLimeGreen)), kLimeGreen);
}
