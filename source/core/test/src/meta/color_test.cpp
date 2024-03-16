// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/meta/color.hpp"

#include <gtest/gtest.h>

#include "tactile/base/container/array.hpp"

namespace tactile {
namespace {

inline constexpr Color kLimeGreen {0x32, 0xCD, 0x32, 0xFF};
inline constexpr Color kDarkBlue {0x00, 0x00, 0x8B, 0xFF};
inline constexpr Color kHotPink {0xFF, 0x69, 0xB4, 0xFF};

}  // namespace

/** \trace tactile::Color::from */
TEST(Color, From)
{
  const Array<float, 4> values {1.0f, 0.0f, 0.2f, 0.4f};
  const auto color = Color::from(values);

  EXPECT_EQ(color.red, 0xFF);
  EXPECT_EQ(color.green, 0x00);
  EXPECT_EQ(color.blue, 0x33);
  EXPECT_EQ(color.alpha, 0x66);
}

/** \trace tactile::Color::parse_rgb */
TEST(Color, ParseRGB)
{
  EXPECT_FALSE(Color::parse_rgb("").has_value());
  EXPECT_FALSE(Color::parse_rgb("000000").has_value());    // no hashtag
  EXPECT_FALSE(Color::parse_rgb("#0000000").has_value());  // too long
  EXPECT_FALSE(Color::parse_rgb("#G00000").has_value());   // invalid digit
  EXPECT_FALSE(Color::parse_rgb("#00G000").has_value());   // invalid digit
  EXPECT_FALSE(Color::parse_rgb("#0000G0").has_value());   // invalid digit

  EXPECT_EQ(Color::parse_rgb("#F00000"), (Color {0xF0, 0x00, 0x00, 0xFF}));
  EXPECT_EQ(Color::parse_rgb("#00F000"), (Color {0x00, 0xF0, 0x00, 0xFF}));
  EXPECT_EQ(Color::parse_rgb("#0000F0"), (Color {0x00, 0x00, 0xF0, 0xFF}));

  EXPECT_EQ(Color::parse_rgb("#000000"), kColorBlack);
  EXPECT_EQ(Color::parse_rgb("#FFFFFF"), kColorWhite);
  EXPECT_EQ(Color::parse_rgb("#32CD32"), kLimeGreen);
  EXPECT_EQ(Color::parse_rgb("#00008B"), kDarkBlue);
  EXPECT_EQ(Color::parse_rgb("#FF69B4"), kHotPink);
}

/** \trace tactile::Color::parse_rgba */
TEST(Color, ParseRGBA)
{
  EXPECT_FALSE(Color::parse_rgba("").has_value());
  EXPECT_FALSE(Color::parse_rgba("00000000").has_value());    // no hashtag
  EXPECT_FALSE(Color::parse_rgba("#000000000").has_value());  // too long
  EXPECT_FALSE(Color::parse_rgba("#G0000000").has_value());   // invalid digit
  EXPECT_FALSE(Color::parse_rgba("#00G00000").has_value());   // invalid digit
  EXPECT_FALSE(Color::parse_rgba("#0000G000").has_value());   // invalid digit
  EXPECT_FALSE(Color::parse_rgba("#000000G0").has_value());   // invalid digit

  EXPECT_EQ(Color::parse_rgba("#F00000FF"), (Color {0xF0, 0x00, 0x00, 0xFF}));
  EXPECT_EQ(Color::parse_rgba("#00F000FF"), (Color {0x00, 0xF0, 0x00, 0xFF}));
  EXPECT_EQ(Color::parse_rgba("#0000F0FF"), (Color {0x00, 0x00, 0xF0, 0xFF}));

  EXPECT_EQ(Color::parse_rgba("#000000FF"), kColorBlack);
  EXPECT_EQ(Color::parse_rgba("#FFFFFFFF"), kColorWhite);
  EXPECT_EQ(Color::parse_rgba("#32CD32FF"), kLimeGreen);
  EXPECT_EQ(Color::parse_rgba("#00008BFF"), kDarkBlue);
  EXPECT_EQ(Color::parse_rgba("#FF69B4FF"), kHotPink);
}

/** \trace tactile::Color::parse_argb */
TEST(Color, ParseARGB)
{
  EXPECT_FALSE(Color::parse_argb("").has_value());
  EXPECT_FALSE(Color::parse_argb("00000000").has_value());    // no hashtag
  EXPECT_FALSE(Color::parse_argb("#000000000").has_value());  // too long
  EXPECT_FALSE(Color::parse_argb("#G0000000").has_value());   // invalid digit
  EXPECT_FALSE(Color::parse_argb("#00G00000").has_value());   // invalid digit
  EXPECT_FALSE(Color::parse_argb("#0000G000").has_value());   // invalid digit
  EXPECT_FALSE(Color::parse_argb("#000000G0").has_value());   // invalid digit

  EXPECT_EQ(Color::parse_argb("#FFF00000"), (Color {0xF0, 0x00, 0x00, 0xFF}));
  EXPECT_EQ(Color::parse_argb("#FF00F000"), (Color {0x00, 0xF0, 0x00, 0xFF}));
  EXPECT_EQ(Color::parse_argb("#FF0000F0"), (Color {0x00, 0x00, 0xF0, 0xFF}));

  EXPECT_EQ(Color::parse_argb("#FF000000"), kColorBlack);
  EXPECT_EQ(Color::parse_argb("#FFFFFFFF"), kColorWhite);
  EXPECT_EQ(Color::parse_argb("#FF32CD32"), kLimeGreen);
  EXPECT_EQ(Color::parse_argb("#FF00008B"), kDarkBlue);
  EXPECT_EQ(Color::parse_argb("#FFFF69B4"), kHotPink);
}

/** \trace tactile::Color::to_string_rgb */
TEST(Color, ToStringRGB)
{
  EXPECT_EQ(kColorBlack.to_string_rgb(), "#000000");
  EXPECT_EQ(kColorWhite.to_string_rgb(), "#FFFFFF");
  EXPECT_EQ(kLimeGreen.to_string_rgb(), "#32CD32");
  EXPECT_EQ(kDarkBlue.to_string_rgb(), "#00008B");
  EXPECT_EQ(kHotPink.to_string_rgb(), "#FF69B4");

  EXPECT_EQ((Color {0x1A, 0x2B, 0x3C, 0x4D}).to_string_rgb(), "#1A2B3C");
  EXPECT_EQ((Color {0xAB, 0xCD, 0xEF, 0x42}).to_string_rgb(), "#ABCDEF");
}

/** \trace tactile::Color::to_string_rgba */
TEST(Color, ToStringRGBA)
{
  EXPECT_EQ(kColorBlack.to_string_rgba(), "#000000FF");
  EXPECT_EQ(kColorWhite.to_string_rgba(), "#FFFFFFFF");
  EXPECT_EQ(kLimeGreen.to_string_rgba(), "#32CD32FF");
  EXPECT_EQ(kDarkBlue.to_string_rgba(), "#00008BFF");
  EXPECT_EQ(kHotPink.to_string_rgba(), "#FF69B4FF");

  EXPECT_EQ((Color {0x1A, 0x2B, 0x3C, 0x4D}).to_string_rgba(), "#1A2B3C4D");
  EXPECT_EQ((Color {0xAB, 0xCD, 0xEF, 0x42}).to_string_rgba(), "#ABCDEF42");
}

/** \trace tactile::Color::to_string_argb */
TEST(Color, ToStringARGB)
{
  EXPECT_EQ(kColorBlack.to_string_argb(), "#FF000000");
  EXPECT_EQ(kColorWhite.to_string_argb(), "#FFFFFFFF");
  EXPECT_EQ(kLimeGreen.to_string_argb(), "#FF32CD32");
  EXPECT_EQ(kDarkBlue.to_string_argb(), "#FF00008B");
  EXPECT_EQ(kHotPink.to_string_argb(), "#FFFF69B4");

  EXPECT_EQ((Color {0x1A, 0x2B, 0x3C, 0x4D}).to_string_argb(), "#4D1A2B3C");
  EXPECT_EQ((Color {0xAB, 0xCD, 0xEF, 0x42}).to_string_argb(), "#42ABCDEF");
}

/** \trace tactile::Color::normalized */
TEST(Color, Normalized)
{
  const Color color {0xFF, 0x00, 0x33, 0x66};
  const auto normalized_color = color.normalized();

  EXPECT_FLOAT_EQ(normalized_color[0], 1.0f);
  EXPECT_FLOAT_EQ(normalized_color[1], 0.0f);
  EXPECT_FLOAT_EQ(normalized_color[2], 0.2f);
  EXPECT_FLOAT_EQ(normalized_color[3], 0.4f);
}

/** \trace tactile::Color::get_luminance */
TEST(Color, GetLuminance)
{
  // Based on https://planetcalc.com/7778/
  EXPECT_FLOAT_EQ(kColorBlack.get_luminance(), 0.0f);
  EXPECT_FLOAT_EQ(kColorWhite.get_luminance(), 1.0f);
  EXPECT_FLOAT_EQ(kLimeGreen.get_luminance(), 0.4457104f);
  EXPECT_FLOAT_EQ(kDarkBlue.get_luminance(), 0.0186408f);
  EXPECT_FLOAT_EQ(kHotPink.get_luminance(), 0.3465843f);
}

/** \trace tactile::Color::is_dark */
TEST(Color, IsDark)
{
  EXPECT_TRUE(kColorBlack.is_dark());
  EXPECT_TRUE(kDarkBlue.is_dark());

  EXPECT_FALSE(kColorWhite.is_dark());
  EXPECT_FALSE(kLimeGreen.is_dark());
  EXPECT_FALSE(kHotPink.is_dark());
}

/**
 * \trace tactile::Color::parse_rgb
 * \trace tactile::Color::to_string_rgb
 */
TEST(Color, RoundTripConversionRGB)
{
  EXPECT_EQ(Color::parse_rgb(kHotPink.to_string_rgb()), kHotPink);
}

/**
 * \trace tactile::Color::parse_rgba
 * \trace tactile::Color::to_string_rgba
 */
TEST(Color, RoundTripConversionRGBA)
{
  EXPECT_EQ(Color::parse_rgba(kDarkBlue.to_string_rgba()), kDarkBlue);
}

/**
 * \trace tactile::Color::parse_argb
 * \trace tactile::Color::to_string_argb
 */
TEST(Color, RoundTripConversionARGB)
{
  EXPECT_EQ(Color::parse_argb(kLimeGreen.to_string_argb()), kLimeGreen);
}

}  // namespace tactile
