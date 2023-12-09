// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/meta/color.hpp"

#include <gtest/gtest.h>

using namespace tactile;

namespace {

inline constexpr UColor kLimeGreen = {0x32, 0xCD, 0x32, 0xFF};
inline constexpr UColor kDarkBlue = {0x00, 0x00, 0x8B, 0xFF};
inline constexpr UColor kHotPink = {0xFF, 0x69, 0xB4, 0xFF};

}  // namespace

/// \tests tactile::to_color_rgb
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

  EXPECT_EQ(to_color_rgb("#000000"), kColorBlack);
  EXPECT_EQ(to_color_rgb("#FFFFFF"), kColorWhite);
  EXPECT_EQ(to_color_rgb("#32CD32"), kLimeGreen);
  EXPECT_EQ(to_color_rgb("#00008B"), kDarkBlue);
  EXPECT_EQ(to_color_rgb("#FF69B4"), kHotPink);
}

/// \tests tactile::to_color_rgba
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

  EXPECT_EQ(to_color_rgba("#000000FF"), kColorBlack);
  EXPECT_EQ(to_color_rgba("#FFFFFFFF"), kColorWhite);
  EXPECT_EQ(to_color_rgba("#32CD32FF"), kLimeGreen);
  EXPECT_EQ(to_color_rgba("#00008BFF"), kDarkBlue);
  EXPECT_EQ(to_color_rgba("#FF69B4FF"), kHotPink);
}

/// \tests tactile::to_color_argb
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

  EXPECT_EQ(to_color_argb("#FF000000"), kColorBlack);
  EXPECT_EQ(to_color_argb("#FFFFFFFF"), kColorWhite);
  EXPECT_EQ(to_color_argb("#FF32CD32"), kLimeGreen);
  EXPECT_EQ(to_color_argb("#FF00008B"), kDarkBlue);
  EXPECT_EQ(to_color_argb("#FFFF69B4"), kHotPink);
}

/// \tests tactile::normalize
TEST(Color, Normalize)
{
  const UColor color {0xFF, 0x00, 0x33, 0x66};
  const auto normalized_color = normalize(color);

  EXPECT_FLOAT_EQ(normalized_color.red, 1.0f);
  EXPECT_FLOAT_EQ(normalized_color.green, 0.0f);
  EXPECT_FLOAT_EQ(normalized_color.blue, 0.2f);
  EXPECT_FLOAT_EQ(normalized_color.alpha, 0.4f);
}

/// \tests tactile::unnormalize
TEST(Color, Unnormalize)
{
  const NColor color {1.0f, 0.0f, 0.2f, 0.4f};
  const auto unnormalized_color = unnormalize(color);

  EXPECT_EQ(unnormalized_color.red, 0xFF);
  EXPECT_EQ(unnormalized_color.green, 0x00);
  EXPECT_EQ(unnormalized_color.blue, 0x33);
  EXPECT_EQ(unnormalized_color.alpha, 0x66);
}

/// \tests tactile::get_luminance
TEST(Color, GetLuminance)
{
  // Based on https://planetcalc.com/7778/
  EXPECT_EQ(get_luminance(normalize(kColorBlack)), 0.0f);
  EXPECT_EQ(get_luminance(normalize(kColorWhite)), 1.0f);
  EXPECT_FLOAT_EQ(get_luminance(normalize(kLimeGreen)), 0.4457104f);
  EXPECT_FLOAT_EQ(get_luminance(normalize(kDarkBlue)), 0.0186408f);
  EXPECT_FLOAT_EQ(get_luminance(normalize(kHotPink)), 0.3465843f);
}

/// \tests tactile::is_dark
TEST(Color, IsDark)
{
  EXPECT_TRUE(is_dark(normalize(kColorBlack)));
  EXPECT_TRUE(is_dark(normalize(kDarkBlue)));

  EXPECT_FALSE(is_dark(normalize(kColorWhite)));
  EXPECT_FALSE(is_dark(normalize(kLimeGreen)));
  EXPECT_FALSE(is_dark(normalize(kHotPink)));
}

/// \tests tactile::to_string_rgb
TEST(Color, ToStringRGB)
{
  EXPECT_EQ(to_string_rgb(kColorBlack), "#000000");
  EXPECT_EQ(to_string_rgb(kColorWhite), "#FFFFFF");
  EXPECT_EQ(to_string_rgb(kLimeGreen), "#32CD32");
  EXPECT_EQ(to_string_rgb(kDarkBlue), "#00008B");
  EXPECT_EQ(to_string_rgb(kHotPink), "#FF69B4");

  EXPECT_EQ(to_string_rgb(UColor {0x1A, 0x2B, 0x3C, 0x4D}), "#1A2B3C");
  EXPECT_EQ(to_string_rgb(UColor {0xAB, 0xCD, 0xEF, 0x42}), "#ABCDEF");
}

/// \tests tactile::to_string_rgba
TEST(Color, ToStringRGBA)
{
  EXPECT_EQ(to_string_rgba(kColorBlack), "#000000FF");
  EXPECT_EQ(to_string_rgba(kColorWhite), "#FFFFFFFF");
  EXPECT_EQ(to_string_rgba(kLimeGreen), "#32CD32FF");
  EXPECT_EQ(to_string_rgba(kDarkBlue), "#00008BFF");
  EXPECT_EQ(to_string_rgba(kHotPink), "#FF69B4FF");

  EXPECT_EQ(to_string_rgba(UColor {0x1A, 0x2B, 0x3C, 0x4D}), "#1A2B3C4D");
  EXPECT_EQ(to_string_rgba(UColor {0xAB, 0xCD, 0xEF, 0x42}), "#ABCDEF42");
}

/// \tests tactile::to_string_argb
TEST(Color, ToStringARGB)
{
  EXPECT_EQ(to_string_argb(kColorBlack), "#FF000000");
  EXPECT_EQ(to_string_argb(kColorWhite), "#FFFFFFFF");
  EXPECT_EQ(to_string_argb(kLimeGreen), "#FF32CD32");
  EXPECT_EQ(to_string_argb(kDarkBlue), "#FF00008B");
  EXPECT_EQ(to_string_argb(kHotPink), "#FFFF69B4");

  EXPECT_EQ(to_string_argb(UColor {0x1A, 0x2B, 0x3C, 0x4D}), "#4D1A2B3C");
  EXPECT_EQ(to_string_argb(UColor {0xAB, 0xCD, 0xEF, 0x42}), "#42ABCDEF");
}

/// \tests tactile::to_string_rgb
/// \tests tactile::to_color_rgb
TEST(Color, RoundTripConversionRGB)
{
  EXPECT_EQ(to_color_rgb(to_string_rgb(kHotPink)), kHotPink);
}

/// \tests tactile::to_string_rgba
/// \tests tactile::to_color_rgba
TEST(Color, RoundTripConversionRGBA)
{
  EXPECT_EQ(to_color_rgba(to_string_rgba(kDarkBlue)), kDarkBlue);
}

/// \tests tactile::to_string_argb
/// \tests tactile::to_color_argb
TEST(Color, RoundTripConversionARGB)
{
  EXPECT_EQ(to_color_argb(to_string_argb(kLimeGreen)), kLimeGreen);
}
