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

#include "core/attributes/color.hpp"

#include <doctest/doctest.h>

using namespace tactile;

namespace {

inline constexpr Color kLimeGreen {0x32, 0xCD, 0x32};
inline constexpr Color kDarkBlue {0, 0, 0x8B};
inline constexpr Color kHotPink {0xFF, 0x69, 0xB4};

}  // namespace

TEST_SUITE("Color")
{
  TEST_CASE("from_norm")
  {
    REQUIRE(Color::from_norm(1.5f, 100, -3.0f, -0.4f) == Color {0xFF, 0xFF, 0, 0});

    REQUIRE(Color::from_norm(0, 0, 0) == Color {0, 0, 0, 0xFF});
    REQUIRE(Color::from_norm(0, 0, 0, 0) == Color {0, 0, 0, 0});
    REQUIRE(Color::from_norm(1, 1, 1) == Color {0xFF, 0xFF, 0xFF, 0xFF});

    REQUIRE(Color::from_norm(0.2f, 0.2f, 0.2f) == Color {51, 51, 51});
  }

  TEST_CASE("from_rgb")
  {
    REQUIRE(!Color::from_rgb("").has_value());
    REQUIRE(!Color::from_rgb("#").has_value());
    REQUIRE(!Color::from_rgb("000000").has_value());
    REQUIRE(!Color::from_rgb("#0000000").has_value());
    REQUIRE(!Color::from_rgb("#foobar").has_value());

    REQUIRE(Color::from_rgb("#000000").value().as_rgb() == "#000000");
    REQUIRE(Color::from_rgb("#123456").value().as_rgb() == "#123456");
    REQUIRE(Color::from_rgb("#ABCDEF").value().as_rgb() == "#ABCDEF");

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

    REQUIRE(Color::from_rgba("#00000000").value().as_rgba() == "#00000000");
    REQUIRE(Color::from_rgba("#12345678").value().as_rgba() == "#12345678");
    REQUIRE(Color::from_rgba("#ABCDEF12").value().as_rgba() == "#ABCDEF12");

    const auto color = Color::from_rgba("#A1B2C3D4").value();
    REQUIRE(color.red == 0xA1);
    REQUIRE(color.green == 0xB2);
    REQUIRE(color.blue == 0xC3);
    REQUIRE(color.alpha == 0xD4);
  }

  TEST_CASE("from_argb")
  {
    REQUIRE(!Color::from_argb("").has_value());
    REQUIRE(!Color::from_argb("#").has_value());
    REQUIRE(!Color::from_argb("00000000").has_value());
    REQUIRE(!Color::from_argb("#000000000").has_value());
    REQUIRE(!Color::from_argb("#foobar12").has_value());

    REQUIRE(Color::from_argb("#00000000").value().as_argb() == "#00000000");
    REQUIRE(Color::from_argb("#12345678").value().as_argb() == "#12345678");
    REQUIRE(Color::from_argb("#ABCDEF12").value().as_argb() == "#ABCDEF12");

    const auto color = Color::from_argb("#A1B2C3D4").value();
    REQUIRE(color.alpha == 0xA1);
    REQUIRE(color.red == 0xB2);
    REQUIRE(color.green == 0xC3);
    REQUIRE(color.blue == 0xD4);
  }

  TEST_CASE("norm_red")
  {
    REQUIRE(Color {0, 0, 0}.norm_red() == 0.0f);
    REQUIRE(Color {0xFF, 0, 0}.norm_red() == 1.0f);
    REQUIRE(Color {51, 0, 0}.norm_red() == 0.2f);
  }

  TEST_CASE("norm_green")
  {
    REQUIRE(Color {0, 0, 0}.norm_green() == 0.0f);
    REQUIRE(Color {0, 0xFF, 0}.norm_green() == 1.0f);
    REQUIRE(Color {0, 51, 0}.norm_green() == 0.2f);
  }

  TEST_CASE("norm_blue")
  {
    REQUIRE(Color {0, 0, 0}.norm_blue() == 0.0f);
    REQUIRE(Color {0, 0, 0xFF}.norm_blue() == 1.0f);
    REQUIRE(Color {0, 0, 51}.norm_blue() == 0.2f);
  }

  TEST_CASE("norm_alpha")
  {
    REQUIRE(Color {0, 0, 0, 0}.norm_alpha() == 0.0f);
    REQUIRE(Color {0, 0, 0, 0xFF}.norm_alpha() == 1.0f);
    REQUIRE(Color {0, 0, 0, 51}.norm_alpha() == 0.2f);
  }

  TEST_CASE("get_luminance")
  {
    REQUIRE(kWhite.get_luminance() == 1.0f);
    REQUIRE(kBlack.get_luminance() == 0.0f);

    // Based on https://planetcalc.com/7778/
    REQUIRE(doctest::Approx {kLimeGreen.get_luminance()} == 0.4457104f);
    REQUIRE(doctest::Approx {kDarkBlue.get_luminance()} == 0.0186408f);
    REQUIRE(doctest::Approx {kHotPink.get_luminance()} == 0.3465843f);
  }

  TEST_CASE("is_dark")
  {
    REQUIRE(!kWhite.is_dark());
    REQUIRE(kBlack.is_dark());
  }

  TEST_CASE("as_rgb")
  {
    REQUIRE(Color {0, 0, 0}.as_rgb() == "#000000");
    REQUIRE(Color {0xFF, 0, 0}.as_rgb() == "#FF0000");
    REQUIRE(Color {0xFF, 0, 0xFF}.as_rgb() == "#FF00FF");
    REQUIRE(Color {0xAB, 0xCD, 0xEF, 0x12}.as_rgb() == "#ABCDEF");
  }

  TEST_CASE("as_rgba")
  {
    REQUIRE(Color {0, 0, 0, 0}.as_rgba() == "#00000000");
    REQUIRE(Color {0xFF, 0, 0}.as_rgba() == "#FF0000FF");
    REQUIRE(Color {0xFF, 0, 0xFF}.as_rgba() == "#FF00FFFF");
    REQUIRE(Color {0xAB, 0xCD, 0xEF, 0x12}.as_rgba() == "#ABCDEF12");
  }

  TEST_CASE("as_argb")
  {
    REQUIRE(Color {0, 0, 0, 0}.as_argb() == "#00000000");
    REQUIRE(Color {0, 0, 0, 0xFF}.as_argb() == "#FF000000");
    REQUIRE(Color {0x12, 0x34, 0x56, 0xEE}.as_argb() == "#EE123456");
    REQUIRE(Color {0xAB, 0xCD, 0xEF, 0x12}.as_argb() == "#12ABCDEF");
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
