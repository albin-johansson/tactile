/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include "core/color.hpp"

#include <centurion/color.hpp>
#include <gtest/gtest.h>

namespace tactile::test {

// inline constexpr Color white = {0xFF, 0xFF, 0xFF};
inline constexpr Color lime_green = {0x32, 0xCD, 0x32};
inline constexpr Color dark_blue = {0, 0, 0x8B};
inline constexpr Color hot_pink = {0xFF, 0x69, 0xB4};

TEST(Color, FromNorm)
{
  ASSERT_EQ((Color {0xFF, 0xFF, 0, 0}), Color::from_norm(1.5f, 100, -3.0f, -0.4f));

  ASSERT_EQ((Color {0, 0, 0, 0xFF}), Color::from_norm(0, 0, 0));
  ASSERT_EQ((Color {0, 0, 0, 0}), Color::from_norm(0, 0, 0, 0));
  ASSERT_EQ((Color {0xFF, 0xFF, 0xFF, 0xFF}), Color::from_norm(1, 1, 1));

  ASSERT_EQ((Color {51, 51, 51}), Color::from_norm(0.2f, 0.2f, 0.2f));
}

TEST(Color, FromRGB)
{
  ASSERT_FALSE(Color::from_rgb("").has_value());
  ASSERT_FALSE(Color::from_rgb("#").has_value());
  ASSERT_FALSE(Color::from_rgb("000000").has_value());
  ASSERT_FALSE(Color::from_rgb("#0000000").has_value());
  ASSERT_FALSE(Color::from_rgb("#foobar").has_value());

  ASSERT_EQ("#000000", Color::from_rgb("#000000").value().as_rgb());
  ASSERT_EQ("#123456", Color::from_rgb("#123456").value().as_rgb());
  ASSERT_EQ("#ABCDEF", Color::from_rgb("#ABCDEF").value().as_rgb());

  const auto color = Color::from_rgb("#A1B2C3").value();
  ASSERT_EQ(0xA1, color.red);
  ASSERT_EQ(0xB2, color.green);
  ASSERT_EQ(0xC3, color.blue);
  ASSERT_EQ(0xFF, color.alpha);
}

TEST(Color, FromRGBA)
{
  ASSERT_FALSE(Color::from_rgba("").has_value());
  ASSERT_FALSE(Color::from_rgba("#").has_value());
  ASSERT_FALSE(Color::from_rgba("00000000").has_value());
  ASSERT_FALSE(Color::from_rgba("#000000000").has_value());
  ASSERT_FALSE(Color::from_rgba("#foobar12").has_value());

  ASSERT_EQ("#00000000", Color::from_rgba("#00000000").value().as_rgba());
  ASSERT_EQ("#12345678", Color::from_rgba("#12345678").value().as_rgba());
  ASSERT_EQ("#ABCDEF12", Color::from_rgba("#ABCDEF12").value().as_rgba());

  const auto color = Color::from_rgba("#A1B2C3D4").value();
  ASSERT_EQ(0xA1, color.red);
  ASSERT_EQ(0xB2, color.green);
  ASSERT_EQ(0xC3, color.blue);
  ASSERT_EQ(0xD4, color.alpha);
}

TEST(Color, FromARGB)
{
  ASSERT_FALSE(Color::from_argb("").has_value());
  ASSERT_FALSE(Color::from_argb("#").has_value());
  ASSERT_FALSE(Color::from_argb("00000000").has_value());
  ASSERT_FALSE(Color::from_argb("#000000000").has_value());
  ASSERT_FALSE(Color::from_argb("#foobar12").has_value());

  ASSERT_EQ("#00000000", Color::from_argb("#00000000").value().as_argb());
  ASSERT_EQ("#12345678", Color::from_argb("#12345678").value().as_argb());
  ASSERT_EQ("#ABCDEF12", Color::from_argb("#ABCDEF12").value().as_argb());

  const auto color = Color::from_argb("#A1B2C3D4").value();
  ASSERT_EQ(0xA1, color.alpha);
  ASSERT_EQ(0xB2, color.red);
  ASSERT_EQ(0xC3, color.green);
  ASSERT_EQ(0xD4, color.blue);
}

TEST(Color, NormRed)
{
  ASSERT_EQ(0.0f, (Color {0, 0, 0}).norm_red());
  ASSERT_EQ(1.0f, (Color {0xFF, 0, 0}).norm_red());
  ASSERT_EQ(0.2f, (Color {51, 0, 0}).norm_red());
}

TEST(Color, NormGreen)
{
  ASSERT_EQ(0.0f, (Color {0, 0, 0}).norm_green());
  ASSERT_EQ(1.0f, (Color {0, 0xFF, 0}).norm_green());
  ASSERT_EQ(0.2f, (Color {0, 51, 0}).norm_green());
}

TEST(Color, NormBlue)
{
  ASSERT_EQ(0.0f, (Color {0, 0, 0}).norm_blue());
  ASSERT_EQ(1.0f, (Color {0, 0, 0xFF}).norm_blue());
  ASSERT_EQ(0.2f, (Color {0, 0, 51}).norm_blue());
}

TEST(Color, NormAlpha)
{
  ASSERT_EQ(0.0f, (Color {0, 0, 0, 0}).norm_alpha());
  ASSERT_EQ(1.0f, (Color {0, 0, 0, 0xFF}).norm_alpha());
  ASSERT_EQ(0.2f, (Color {0, 0, 0, 51}).norm_alpha());
}

TEST(Color, GetLuminance)
{
  ASSERT_EQ(1.0f, white.get_luminance());
  ASSERT_EQ(0.0f, black.get_luminance());

  // Based on https://planetcalc.com/7778/
  ASSERT_FLOAT_EQ(0.4457104f, lime_green.get_luminance());
  ASSERT_FLOAT_EQ(0.0186408f, dark_blue.get_luminance());
  ASSERT_FLOAT_EQ(0.3465843f, hot_pink.get_luminance());
}

TEST(Color, IsDark)
{
  ASSERT_FALSE(white.is_dark());
  ASSERT_TRUE(black.is_dark());
}

TEST(Color, AsRGB)
{
  ASSERT_EQ("#000000", (Color {0, 0, 0}).as_rgb());
  ASSERT_EQ("#FF0000", (Color {0xFF, 0, 0}).as_rgb());
  ASSERT_EQ("#FF00FF", (Color {0xFF, 0, 0xFF}).as_rgb());
  ASSERT_EQ("#ABCDEF", (Color {0xAB, 0xCD, 0xEF, 0x12}).as_rgb());
}

TEST(Color, AsRGBA)
{
  ASSERT_EQ("#00000000", (Color {0, 0, 0, 0}).as_rgba());
  ASSERT_EQ("#FF0000FF", (Color {0xFF, 0, 0}).as_rgba());
  ASSERT_EQ("#FF00FFFF", (Color {0xFF, 0, 0xFF}).as_rgba());
  ASSERT_EQ("#ABCDEF12", (Color {0xAB, 0xCD, 0xEF, 0x12}).as_rgba());
}

TEST(Color, AsARGB)
{
  ASSERT_EQ("#00000000", (Color {0, 0, 0, 0}).as_argb());
  ASSERT_EQ("#FF000000", (Color {0, 0, 0, 0xFF}).as_argb());
  ASSERT_EQ("#EE123456", (Color {0x12, 0x34, 0x56, 0xEE}).as_argb());
  ASSERT_EQ("#12ABCDEF", (Color {0xAB, 0xCD, 0xEF, 0x12}).as_argb());
}

TEST(Color, AsFloatArray)
{
  const Color color {10, 45, 83, 232};
  const auto rgba = color.as_float_array();
  ASSERT_EQ(color.norm_red(), rgba.at(0));
  ASSERT_EQ(color.norm_green(), rgba.at(1));
  ASSERT_EQ(color.norm_blue(), rgba.at(2));
  ASSERT_EQ(color.norm_alpha(), rgba.at(3));
}

}  // namespace tactile::test
