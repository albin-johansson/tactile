// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/context/color.hpp"

#include <algorithm>  // clamp
#include <cmath>      // pow

#include <fmt/format.h>

namespace tactile {

using namespace int_literals;

auto to_color_rgb(const StringView rgb) -> Maybe<UColor>
{
  if (rgb.length() != 7_uz || rgb.front() != '#') {
    return kNone;
  }

  const auto red = str_to_u32(rgb.substr(1, 2), 16);
  const auto green = str_to_u32(rgb.substr(3, 2), 16);
  const auto blue = str_to_u32(rgb.substr(5, 2), 16);

  if (red && green && blue) {
    return UColor {static_cast<uint8>(*red),
                   static_cast<uint8>(*green),
                   static_cast<uint8>(*blue),
                   0xFF};
  }

  return kNone;
}

auto to_color_rgba(const StringView rgba) -> Maybe<UColor>
{
  if (rgba.length() != 9_uz || rgba.front() != '#') {
    return kNone;
  }

  const auto red = str_to_u32(rgba.substr(1, 2), 16);
  const auto green = str_to_u32(rgba.substr(3, 2), 16);
  const auto blue = str_to_u32(rgba.substr(5, 2), 16);
  const auto alpha = str_to_u32(rgba.substr(7, 2), 16);

  if (red && green && blue && alpha) {
    return UColor {static_cast<uint8>(*red),
                   static_cast<uint8>(*green),
                   static_cast<uint8>(*blue),
                   static_cast<uint8>(*alpha)};
  }

  return kNone;
}

auto to_color_argb(const StringView argb) -> Maybe<UColor>
{
  if (argb.length() != 9_uz || argb.front() != '#') {
    return kNone;
  }

  const auto alpha = str_to_u32(argb.substr(1, 2), 16);
  const auto red = str_to_u32(argb.substr(3, 2), 16);
  const auto green = str_to_u32(argb.substr(5, 2), 16);
  const auto blue = str_to_u32(argb.substr(7, 2), 16);

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
  auto unnormalize_channel = [](const float channel) {
    const auto clamped_ch = std::clamp(channel, 0.0f, 1.0f);
    return static_cast<uint8>(clamped_ch * 255.0f);
  };

  return {
    .red = unnormalize_channel(color.red),
    .green = unnormalize_channel(color.green),
    .blue = unnormalize_channel(color.blue),
    .alpha = unnormalize_channel(color.alpha),
  };
}

auto get_luminance(const NColor& color) -> float
{
  // https://en.wikipedia.org/wiki/SRGB#From_sRGB_to_CIE_XYZ
  auto make_linear = [](const float channel) {
    if (channel <= 0.04045f) {
      return channel / 12.92f;
    }
    else {
      return std::pow((channel + 0.055f) / 1.055f, 2.4f);
    }
  };

  const auto l_red = make_linear(color.red);
  const auto l_green = make_linear(color.green);
  const auto l_blue = make_linear(color.blue);

  return (0.2126f * l_red) + (0.7152f * l_green) + (0.0722f * l_blue);
}

auto is_dark(const NColor& color) -> bool
{
  return get_luminance(color) < 0.3f;
}

auto to_string_rgb(const UColor& color) -> String
{
  return fmt::format("#{:02X}{:02X}{:02X}", color.red, color.green, color.blue);
}

auto to_string_rgba(const UColor& color) -> String
{
  return fmt::format("#{:02X}{:02X}{:02X}{:02X}",
                     color.red,
                     color.green,
                     color.blue,
                     color.alpha);
}

auto to_string_argb(const UColor& color) -> String
{
  return fmt::format("#{:02X}{:02X}{:02X}{:02X}",
                     color.alpha,
                     color.red,
                     color.green,
                     color.blue);
}

auto operator<<(std::ostream& stream, const UColor& color) -> std::ostream&
{
  return stream << to_string_rgba(color);
}

auto operator<<(std::ostream& stream, const NColor& color) -> std::ostream&
{
  return stream << to_string_rgba(unnormalize(color));
}

}  // namespace tactile