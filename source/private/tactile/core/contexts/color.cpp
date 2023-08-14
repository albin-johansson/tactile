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

#include <algorithm>  // clamp
#include <cmath>      // pow

#include <fmt/format.h>

#include "tactile/core/common/string_util.hpp"

namespace tactile {
namespace {

// https://en.wikipedia.org/wiki/SRGB#From_sRGB_to_CIE_XYZ
[[nodiscard]] auto _to_linear(const float channel) -> float
{
  if (channel <= 0.04045f) {
    return channel / 12.92f;
  }
  else {
    return std::pow((channel + 0.055f) / 1.055f, 2.4f);
  }
}

}  // namespace

auto to_color_rgb(std::string_view rgb) -> std::optional<UColor>
{
  if (rgb.length() != 7 || rgb.at(0) != '#') {
    return kNone;
  }

  const auto no_hash = rgb.substr(1);

  const auto red = str_to_u32(no_hash.substr(0, 2), 16);
  const auto green = str_to_u32(no_hash.substr(2, 2), 16);
  const auto blue = str_to_u32(no_hash.substr(4, 2), 16);

  if (red && green && blue) {
    return UColor {static_cast<uint8>(*red),
                   static_cast<uint8>(*green),
                   static_cast<uint8>(*blue),
                   0xFF};
  }

  return kNone;
}

auto to_color_rgba(std::string_view rgba) -> std::optional<UColor>
{
  if (rgba.length() != 9 || rgba.at(0) != '#') {
    return kNone;
  }

  const auto no_hash = rgba.substr(1);

  const auto red = str_to_u32(no_hash.substr(0, 2), 16);
  const auto green = str_to_u32(no_hash.substr(2, 2), 16);
  const auto blue = str_to_u32(no_hash.substr(4, 2), 16);
  const auto alpha = str_to_u32(no_hash.substr(6, 2), 16);

  if (red && green && blue && alpha) {
    return UColor {static_cast<uint8>(*red),
                   static_cast<uint8>(*green),
                   static_cast<uint8>(*blue),
                   static_cast<uint8>(*alpha)};
  }

  return kNone;
}

auto to_color_argb(std::string_view argb) -> std::optional<UColor>
{
  if (argb.length() != 9 || argb.at(0) != '#') {
    return kNone;
  }

  const auto no_hash = argb.substr(1);

  const auto alpha = str_to_u32(no_hash.substr(0, 2), 16);
  const auto red = str_to_u32(no_hash.substr(2, 2), 16);
  const auto green = str_to_u32(no_hash.substr(4, 2), 16);
  const auto blue = str_to_u32(no_hash.substr(6, 2), 16);

  if (red && green && blue && alpha) {
    return UColor {static_cast<uint8>(*red),
                   static_cast<uint8>(*green),
                   static_cast<uint8>(*blue),
                   static_cast<uint8>(*alpha)};
  }

  return kNone;
}

auto normalize(const UColor& color) -> NColor
{
  return {
      .red = static_cast<float>(color.red) / 255.0f,
      .green = static_cast<float>(color.green) / 255.0f,
      .blue = static_cast<float>(color.blue) / 255.0f,
      .alpha = static_cast<float>(color.alpha) / 255.0f,
  };
}

auto unnormalize(const NColor& color) -> UColor
{
  auto convert_channel = [](const float channel) {
    const auto adjusted = std::clamp(channel * 255.0f, 0.0f, 255.0f);
    return static_cast<uint8>(adjusted);
  };

  return {
      .red = convert_channel(color.red),
      .green = convert_channel(color.green),
      .blue = convert_channel(color.blue),
      .alpha = convert_channel(color.alpha),
  };
}

auto get_luminance(const NColor& color) -> float
{
  const auto r_lin = _to_linear(color.red);
  const auto g_lin = _to_linear(color.green);
  const auto b_lin = _to_linear(color.blue);
  return (0.2126f * r_lin) + (0.7152f * g_lin) + (0.0722f * b_lin);
}

auto is_dark_color(const NColor& color) -> bool
{
  return get_luminance(color) < 0.3f;
}

auto to_string_rgb(const UColor& color) -> std::string
{
  return fmt::format("#{:02X}{:02X}{:02X}", +color.red, +color.green, +color.blue);
}

auto to_string_rgba(const UColor& color) -> std::string
{
  return fmt::format("#{:02X}{:02X}{:02X}{:02X}",
                     +color.red,
                     +color.green,
                     +color.blue,
                     +color.alpha);
}

auto to_string_argb(const UColor& color) -> std::string
{
  return fmt::format("#{:02X}{:02X}{:02X}{:02X}",
                     +color.alpha,
                     +color.red,
                     +color.green,
                     +color.blue);
}

}  // namespace tactile
