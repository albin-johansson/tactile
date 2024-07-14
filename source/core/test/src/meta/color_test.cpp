// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/meta/color.hpp"

#include <gtest/gtest.h>

#include "tactile/base/container/array.hpp"

namespace tactile {
namespace color_test {

inline constexpr UColor kLimeGreen {0x32, 0xCD, 0x32, 0xFF};
inline constexpr UColor kDarkBlue {0x00, 0x00, 0x8B, 0xFF};
inline constexpr UColor kHotPink {0xFF, 0x69, 0xB4, 0xFF};

}  // namespace color_test

// tactile::UColor::from
TEST(UColor, From)
{
  const Array<float, 4> values {1.0f, 0.0f, 0.2f, 0.4f};
  const auto color = make_color(values);

  EXPECT_EQ(color.red, 0xFF);
  EXPECT_EQ(color.green, 0x00);
  EXPECT_EQ(color.blue, 0x33);
  EXPECT_EQ(color.alpha, 0x66);
}

// tactile::UColor::parse_rgb
TEST(UColor, ParseRGB)
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

  EXPECT_EQ(parse_color_rgb("#000000"), kColorBlack);
  EXPECT_EQ(parse_color_rgb("#FFFFFF"), kColorWhite);
  EXPECT_EQ(parse_color_rgb("#32CD32"), color_test::kLimeGreen);
  EXPECT_EQ(parse_color_rgb("#00008B"), color_test::kDarkBlue);
  EXPECT_EQ(parse_color_rgb("#FF69B4"), color_test::kHotPink);
}

// tactile::UColor::parse_rgba
TEST(UColor, ParseRGBA)
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

  EXPECT_EQ(parse_color_rgba("#000000FF"), kColorBlack);
  EXPECT_EQ(parse_color_rgba("#FFFFFFFF"), kColorWhite);
  EXPECT_EQ(parse_color_rgba("#32CD32FF"), color_test::kLimeGreen);
  EXPECT_EQ(parse_color_rgba("#00008BFF"), color_test::kDarkBlue);
  EXPECT_EQ(parse_color_rgba("#FF69B4FF"), color_test::kHotPink);
}

// tactile::UColor::parse_argb
TEST(UColor, ParseARGB)
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

  EXPECT_EQ(parse_color_argb("#FF000000"), kColorBlack);
  EXPECT_EQ(parse_color_argb("#FFFFFFFF"), kColorWhite);
  EXPECT_EQ(parse_color_argb("#FF32CD32"), color_test::kLimeGreen);
  EXPECT_EQ(parse_color_argb("#FF00008B"), color_test::kDarkBlue);
  EXPECT_EQ(parse_color_argb("#FFFF69B4"), color_test::kHotPink);
}

// tactile::UColor::to_string_rgb
TEST(UColor, ToStringRGB)
{
  EXPECT_EQ(to_string_rgb(kColorBlack), "#000000");
  EXPECT_EQ(to_string_rgb(kColorWhite), "#FFFFFF");
  EXPECT_EQ(to_string_rgb(color_test::kLimeGreen), "#32CD32");
  EXPECT_EQ(to_string_rgb(color_test::kDarkBlue), "#00008B");
  EXPECT_EQ(to_string_rgb(color_test::kHotPink), "#FF69B4");

  EXPECT_EQ(to_string_rgb(UColor {0x1A, 0x2B, 0x3C, 0x4D}), "#1A2B3C");
  EXPECT_EQ(to_string_rgb(UColor {0xAB, 0xCD, 0xEF, 0x42}), "#ABCDEF");
}

// tactile::UColor::to_string_rgba
TEST(UColor, ToStringRGBA)
{
  EXPECT_EQ(to_string_rgba(kColorBlack), "#000000FF");
  EXPECT_EQ(to_string_rgba(kColorWhite), "#FFFFFFFF");
  EXPECT_EQ(to_string_rgba(color_test::kLimeGreen), "#32CD32FF");
  EXPECT_EQ(to_string_rgba(color_test::kDarkBlue), "#00008BFF");
  EXPECT_EQ(to_string_rgba(color_test::kHotPink), "#FF69B4FF");

  EXPECT_EQ(to_string_rgba(UColor {0x1A, 0x2B, 0x3C, 0x4D}), "#1A2B3C4D");
  EXPECT_EQ(to_string_rgba(UColor {0xAB, 0xCD, 0xEF, 0x42}), "#ABCDEF42");
}

// tactile::UColor::to_string_argb
TEST(UColor, ToStringARGB)
{
  EXPECT_EQ(to_string_argb(kColorBlack), "#FF000000");
  EXPECT_EQ(to_string_argb(kColorWhite), "#FFFFFFFF");
  EXPECT_EQ(to_string_argb(color_test::kLimeGreen), "#FF32CD32");
  EXPECT_EQ(to_string_argb(color_test::kDarkBlue), "#FF00008B");
  EXPECT_EQ(to_string_argb(color_test::kHotPink), "#FFFF69B4");

  EXPECT_EQ(to_string_argb(UColor {0x1A, 0x2B, 0x3C, 0x4D}), "#4D1A2B3C");
  EXPECT_EQ(to_string_argb(UColor {0xAB, 0xCD, 0xEF, 0x42}), "#42ABCDEF");
}

// tactile::UColor::to_uint32_abgr
TEST(UColor, ToUInt32ABGR)
{
  EXPECT_EQ(to_uint32_abgr(kColorBlack), uint32 {0xFF000000});
  EXPECT_EQ(to_uint32_abgr(kColorWhite), uint32 {0xFFFFFFFF});
  EXPECT_EQ(to_uint32_abgr(color_test::kLimeGreen), uint32 {0xFF32CD32});
  EXPECT_EQ(to_uint32_abgr(color_test::kDarkBlue), uint32 {0xFF8B0000});
  EXPECT_EQ(to_uint32_abgr(color_test::kHotPink), uint32 {0xFFB469FF});
}

// tactile::UColor::normalized
TEST(UColor, Normalized)
{
  const UColor color {0xFF, 0x00, 0x33, 0x66};
  const auto normalized_color = normalize(color);

  EXPECT_FLOAT_EQ(normalized_color.red, 1.0f);
  EXPECT_FLOAT_EQ(normalized_color.green, 0.0f);
  EXPECT_FLOAT_EQ(normalized_color.blue, 0.2f);
  EXPECT_FLOAT_EQ(normalized_color.alpha, 0.4f);
}

// tactile::Color::get_luminance
TEST(Color, GetLuminance)
{
  // Based on https://planetcalc.com/7778/
  EXPECT_FLOAT_EQ(get_luminance(kColorBlack), 0.0f);
  EXPECT_FLOAT_EQ(get_luminance(kColorWhite), 1.0f);
  EXPECT_FLOAT_EQ(get_luminance(color_test::kLimeGreen), 0.4457104f);
  EXPECT_FLOAT_EQ(get_luminance(color_test::kDarkBlue), 0.0186408f);
  EXPECT_FLOAT_EQ(get_luminance(color_test::kHotPink), 0.3465843f);
}

// tactile::Color::is_dark
TEST(Color, IsDark)
{
  EXPECT_TRUE(is_dark(kColorBlack));
  EXPECT_TRUE(is_dark(color_test::kDarkBlue));

  EXPECT_FALSE(is_dark(kColorWhite));
  EXPECT_FALSE(is_dark(color_test::kLimeGreen));
  EXPECT_FALSE(is_dark(color_test::kHotPink));
}

// tactile::Color::parse_rgb
// tactile::Color::to_string_rgb
TEST(Color, RoundTripConversionRGB)
{
  EXPECT_EQ(parse_color_rgb(to_string_rgb(color_test::kHotPink)),
            color_test::kHotPink);
}

// tactile::Color::parse_rgba
// tactile::Color::to_string_rgba
TEST(Color, RoundTripConversionRGBA)
{
  EXPECT_EQ(parse_color_rgba(to_string_rgba(color_test::kDarkBlue)),
            color_test::kDarkBlue);
}

// tactile::Color::parse_argb
// tactile::Color::to_string_argb
TEST(Color, RoundTripConversionARGB)
{
  EXPECT_EQ(parse_color_argb(to_string_argb(color_test::kLimeGreen)),
            color_test::kLimeGreen);
}

}  // namespace tactile
