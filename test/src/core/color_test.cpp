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

#include <doctest/doctest.h>

namespace tactile::test {

inline constexpr Color lime_green {0x32, 0xCD, 0x32};
inline constexpr Color dark_blue {0, 0, 0x8B};
inline constexpr Color hot_pink {0xFF, 0x69, 0xB4};

TEST_SUITE("Color")
{
  TEST_CASE("from_norm")
  {
    REQUIRE(Color {0xFF, 0xFF, 0, 0} == Color::from_norm(1.5f, 100, -3.0f, -0.4f));

    REQUIRE(Color {0, 0, 0, 0xFF} == Color::from_norm(0, 0, 0));
    REQUIRE(Color {0, 0, 0, 0} == Color::from_norm(0, 0, 0, 0));
    REQUIRE(Color {0xFF, 0xFF, 0xFF, 0xFF} == Color::from_norm(1, 1, 1));

    REQUIRE(Color {51, 51, 51} == Color::from_norm(0.2f, 0.2f, 0.2f));
  }

  TEST_CASE("from_rgb")
  {
    REQUIRE(!Color::from_rgb("").has_value());
    REQUIRE(!Color::from_rgb("#").has_value());
    REQUIRE(!Color::from_rgb("000000").has_value());
    REQUIRE(!Color::from_rgb("#0000000").has_value());
    REQUIRE(!Color::from_rgb("#foobar").has_value());

    REQUIRE("#000000" == Color::from_rgb("#000000").value().as_rgb());
    REQUIRE("#123456" == Color::from_rgb("#123456").value().as_rgb());
    REQUIRE("#ABCDEF" == Color::from_rgb("#ABCDEF").value().as_rgb());

    const auto color = Color::from_rgb("#A1B2C3").value();
    REQUIRE(0xA1 == color.red);
    REQUIRE(0xB2 == color.green);
    REQUIRE(0xC3 == color.blue);
    REQUIRE(0xFF == color.alpha);
  }

  TEST_CASE("from_rgba")
  {
    REQUIRE(!Color::from_rgba("").has_value());
    REQUIRE(!Color::from_rgba("#").has_value());
    REQUIRE(!Color::from_rgba("00000000").has_value());
    REQUIRE(!Color::from_rgba("#000000000").has_value());
    REQUIRE(!Color::from_rgba("#foobar12").has_value());

    REQUIRE("#00000000" == Color::from_rgba("#00000000").value().as_rgba());
    REQUIRE("#12345678" == Color::from_rgba("#12345678").value().as_rgba());
    REQUIRE("#ABCDEF12" == Color::from_rgba("#ABCDEF12").value().as_rgba());

    const auto color = Color::from_rgba("#A1B2C3D4").value();
    REQUIRE(0xA1 == color.red);
    REQUIRE(0xB2 == color.green);
    REQUIRE(0xC3 == color.blue);
    REQUIRE(0xD4 == color.alpha);
  }

  TEST_CASE("from_argb")
  {
    REQUIRE(!Color::from_argb("").has_value());
    REQUIRE(!Color::from_argb("#").has_value());
    REQUIRE(!Color::from_argb("00000000").has_value());
    REQUIRE(!Color::from_argb("#000000000").has_value());
    REQUIRE(!Color::from_argb("#foobar12").has_value());

    REQUIRE("#00000000" == Color::from_argb("#00000000").value().as_argb());
    REQUIRE("#12345678" == Color::from_argb("#12345678").value().as_argb());
    REQUIRE("#ABCDEF12" == Color::from_argb("#ABCDEF12").value().as_argb());

    const auto color = Color::from_argb("#A1B2C3D4").value();
    REQUIRE(0xA1 == color.alpha);
    REQUIRE(0xB2 == color.red);
    REQUIRE(0xC3 == color.green);
    REQUIRE(0xD4 == color.blue);
  }

  TEST_CASE("norm_red")
  {
    REQUIRE(0.0f == Color {0, 0, 0}.norm_red());
    REQUIRE(1.0f == Color {0xFF, 0, 0}.norm_red());
    REQUIRE(0.2f == Color {51, 0, 0}.norm_red());
  }

  TEST_CASE("norm_green")
  {
    REQUIRE(0.0f == Color {0, 0, 0}.norm_green());
    REQUIRE(1.0f == Color {0, 0xFF, 0}.norm_green());
    REQUIRE(0.2f == Color {0, 51, 0}.norm_green());
  }

  TEST_CASE("norm_blue")
  {
    REQUIRE(0.0f == Color {0, 0, 0}.norm_blue());
    REQUIRE(1.0f == Color {0, 0, 0xFF}.norm_blue());
    REQUIRE(0.2f == Color {0, 0, 51}.norm_blue());
  }

  TEST_CASE("norm_alpha")
  {
    REQUIRE(0.0f == Color {0, 0, 0, 0}.norm_alpha());
    REQUIRE(1.0f == Color {0, 0, 0, 0xFF}.norm_alpha());
    REQUIRE(0.2f == Color {0, 0, 0, 51}.norm_alpha());
  }

  TEST_CASE("get_luminance")
  {
    REQUIRE(1.0f == kWhite.get_luminance());
    REQUIRE(0.0f == kBlack.get_luminance());

    // Based on https://planetcalc.com/7778/
    REQUIRE(0.4457104f == doctest::Approx {lime_green.get_luminance()});
    REQUIRE(0.0186408f == doctest::Approx {dark_blue.get_luminance()});
    REQUIRE(0.3465843f == doctest::Approx {hot_pink.get_luminance()});
  }

  TEST_CASE("is_dark")
  {
    REQUIRE(!kWhite.is_dark());
    REQUIRE(kBlack.is_dark());
  }

  TEST_CASE("as_rgb")
  {
    REQUIRE("#000000" == Color {0, 0, 0}.as_rgb());
    REQUIRE("#FF0000" == Color {0xFF, 0, 0}.as_rgb());
    REQUIRE("#FF00FF" == Color {0xFF, 0, 0xFF}.as_rgb());
    REQUIRE("#ABCDEF" == Color {0xAB, 0xCD, 0xEF, 0x12}.as_rgb());
  }

  TEST_CASE("as_rgba")
  {
    REQUIRE("#00000000" == Color {0, 0, 0, 0}.as_rgba());
    REQUIRE("#FF0000FF" == Color {0xFF, 0, 0}.as_rgba());
    REQUIRE("#FF00FFFF" == Color {0xFF, 0, 0xFF}.as_rgba());
    REQUIRE("#ABCDEF12" == Color {0xAB, 0xCD, 0xEF, 0x12}.as_rgba());
  }

  TEST_CASE("as_argb")
  {
    REQUIRE("#00000000" == Color {0, 0, 0, 0}.as_argb());
    REQUIRE("#FF000000" == Color {0, 0, 0, 0xFF}.as_argb());
    REQUIRE("#EE123456" == Color {0x12, 0x34, 0x56, 0xEE}.as_argb());
    REQUIRE("#12ABCDEF" == Color {0xAB, 0xCD, 0xEF, 0x12}.as_argb());
  }

  TEST_CASE("as_float_array")
  {
    const Color color {10, 45, 83, 232};
    const auto rgba = color.as_float_array();
    REQUIRE(color.norm_red() == rgba.at(0));
    REQUIRE(color.norm_green() == rgba.at(1));
    REQUIRE(color.norm_blue() == rgba.at(2));
    REQUIRE(color.norm_alpha() == rgba.at(3));
  }
}

}  // namespace tactile::test
