// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "color.hpp"

#include <algorithm>  // clamp
#include <cmath>      // round, pow

#include <fmt/format.h>

#include "tactile/base/prelude.hpp"
#include "tactile/core/util/string_conv.hpp"

namespace tactile {
namespace {

inline constexpr float kIsDarkThreshold = 0.3f;

// https://en.wikipedia.org/wiki/SRGB#From_sRGB_to_CIE_XYZ
[[nodiscard]] auto to_linear(const float channel) noexcept(TACTILE_COMPILER_MSVC) -> float
{
  if (channel <= 0.04045f) {
    return channel / 12.92f;
  }
  else {
    return std::pow(((channel + 0.055f) / 1.055f), 2.4f);
  }
}

}  // namespace

auto Color::from_norm(float r, float g, float b, float a) -> Color
{
  r = std::clamp(r, 0.0f, 1.0f);
  g = std::clamp(g, 0.0f, 1.0f);
  b = std::clamp(b, 0.0f, 1.0f);
  a = std::clamp(a, 0.0f, 1.0f);

  return Color {
      .red = static_cast<uint8>(std::round(r * 255.0f)),
      .green = static_cast<uint8>(std::round(g * 255.0f)),
      .blue = static_cast<uint8>(std::round(b * 255.0f)),
      .alpha = static_cast<uint8>(std::round(a * 255.0f)),
  };
}

auto Color::from_rgb(StringView rgb) -> Maybe<Color>
{
  if (rgb.length() != 7 || rgb.at(0) != '#') {
    return nothing;
  }

  const auto no_hash = rgb.substr(1);

  const auto rr = no_hash.substr(0, 2);
  const auto gg = no_hash.substr(2, 2);
  const auto bb = no_hash.substr(4, 2);

  const auto red = parse_uint(rr, 16);
  const auto green = parse_uint(gg, 16);
  const auto blue = parse_uint(bb, 16);

  if (red && green && blue) {
    return Color {static_cast<uint8>(*red),
                  static_cast<uint8>(*green),
                  static_cast<uint8>(*blue),
                  0xFF};
  }
  else {
    return nothing;
  }
}

auto Color::from_rgba(StringView rgba) -> Maybe<Color>
{
  if (rgba.length() != 9 || rgba.at(0) != '#') {
    return nothing;
  }

  const auto no_hash = rgba.substr(1);

  const auto rr = no_hash.substr(0, 2);
  const auto gg = no_hash.substr(2, 2);
  const auto bb = no_hash.substr(4, 2);
  const auto aa = no_hash.substr(6, 2);

  const auto red = parse_uint(rr, 16);
  const auto green = parse_uint(gg, 16);
  const auto blue = parse_uint(bb, 16);
  const auto alpha = parse_uint(aa, 16);

  if (red && green && blue && alpha) {
    return Color {static_cast<uint8>(*red),
                  static_cast<uint8>(*green),
                  static_cast<uint8>(*blue),
                  static_cast<uint8>(*alpha)};
  }
  else {
    return nothing;
  }
}

auto Color::from_argb(StringView argb) -> Maybe<Color>
{
  if (argb.length() != 9 || argb.at(0) != '#') {
    return nothing;
  }

  const auto no_hash = argb.substr(1);

  const auto aa = no_hash.substr(0, 2);
  const auto rr = no_hash.substr(2, 2);
  const auto gg = no_hash.substr(4, 2);
  const auto bb = no_hash.substr(6, 2);

  const auto alpha = parse_uint(aa, 16);
  const auto red = parse_uint(rr, 16);
  const auto green = parse_uint(gg, 16);
  const auto blue = parse_uint(bb, 16);

  if (alpha && red && green && blue) {
    return Color {static_cast<uint8>(*red),
                  static_cast<uint8>(*green),
                  static_cast<uint8>(*blue),
                  static_cast<uint8>(*alpha)};
  }
  else {
    return nothing;
  }
}

auto Color::get_luminance() const -> float
{
  const auto r_lin = to_linear(norm_red());
  const auto g_lin = to_linear(norm_green());
  const auto b_lin = to_linear(norm_blue());
  return (0.2126f * r_lin) + (0.7152f * g_lin) + (0.0722f * b_lin);
}

auto Color::is_dark() const -> bool
{
  return get_luminance() < kIsDarkThreshold;
}

auto Color::as_rgb() const -> String
{
  return fmt::format("#{:02X}{:02X}{:02X}", +red, +green, +blue);
}

auto Color::as_rgba() const -> String
{
  return fmt::format("#{:02X}{:02X}{:02X}{:02X}", +red, +green, +blue, +alpha);
}

auto Color::as_argb() const -> String
{
  return fmt::format("#{:02X}{:02X}{:02X}{:02X}", +alpha, +red, +green, +blue);
}

auto Color::as_float_array() const -> Array<float, 4>
{
  return {norm_red(), norm_green(), norm_blue(), norm_alpha()};
}

}  // namespace tactile
