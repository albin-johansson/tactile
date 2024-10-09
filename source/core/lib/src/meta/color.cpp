// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/meta/color.hpp"

#include <algorithm>  // clamp
#include <cmath>      // pow
#include <format>     // format

#include "tactile/base/io/int_parser.hpp"
#include "tactile/core/util/string_conv.hpp"

namespace tactile::core {

auto make_color(const float r, const float g, const float b, const float a) -> UColor
{
  const auto to_u8 = [](const float val) {
    return static_cast<std::uint8_t>(std::clamp(val, 0.0f, 1.0f) * 255.0f);
  };

  return {to_u8(r), to_u8(g), to_u8(b), to_u8(a)};
}

auto make_color(const std::span<const float, 4> rgba) -> UColor
{
  return make_color(rgba[0], rgba[1], rgba[2], rgba[3]);
}

auto make_color(const FColor& fcolor) -> UColor
{
  return make_color(fcolor.red, fcolor.green, fcolor.blue, fcolor.alpha);
}

auto to_string_rgb(const UColor& color) -> std::string
{
  return std::format("#{:02X}{:02X}{:02X}", color.red, color.green, color.blue);
}

auto to_string_rgba(const UColor& color) -> std::string
{
  return std::format("#{:02X}{:02X}{:02X}{:02X}",
                     color.red,
                     color.green,
                     color.blue,
                     color.alpha);
}

auto to_string_argb(const UColor& color) -> std::string
{
  return std::format("#{:02X}{:02X}{:02X}{:02X}",
                     color.alpha,
                     color.red,
                     color.green,
                     color.blue);
}

auto to_uint32_abgr(const UColor& color) -> std::uint32_t
{
  const auto a = static_cast<std::uint32_t>(color.alpha) << std::uint32_t {24};
  const auto b = static_cast<std::uint32_t>(color.blue) << std::uint32_t {16};
  const auto g = static_cast<std::uint32_t>(color.green) << std::uint32_t {8};
  const auto r = static_cast<std::uint32_t>(color.red) << std::uint32_t {0};
  return a | b | g | r;
}

auto normalize(const UColor& color) -> FColor
{
  const auto to_f = [](const std::uint8_t val) { return static_cast<float>(val) / 255.0f; };

  return {to_f(color.red), to_f(color.green), to_f(color.blue), to_f(color.alpha)};
}

auto get_luminance(const UColor& color) -> float
{
  // https://en.wikipedia.org/wiki/SRGB#From_sRGB_to_CIE_XYZ
  auto linearize = [](const float channel) {
    if (channel <= 0.04045f) {
      return channel / 12.92f;
    }

    return std::pow((channel + 0.055f) / 1.055f, 2.4f);
  };

  const auto norm_values = normalize(color);
  const auto lr = linearize(norm_values.red);
  const auto lg = linearize(norm_values.green);
  const auto lb = linearize(norm_values.blue);

  return (0.2126f * lr) + (0.7152f * lg) + (0.0722f * lb);
}

auto is_dark(const UColor& color) -> bool
{
  return get_luminance(color) < 0.3f;
}

auto operator<<(std::ostream& stream, const UColor& color) -> std::ostream&
{
  return stream << to_string_rgba(color);
}

}  // namespace tactile::core
