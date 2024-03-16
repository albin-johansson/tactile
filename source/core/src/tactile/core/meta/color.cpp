// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/meta/color.hpp"

#include <algorithm>  // clamp
#include <cmath>      // pow

#include <fmt/format.h>

#include "tactile/core/debug/generic_error.hpp"
#include "tactile/core/numeric/narrow.hpp"
#include "tactile/core/util/string_conv.hpp"

namespace tactile {

auto Color::from(const float r, const float g, const float b, const float a) -> Color
{
  const auto to_u8 = [](const float val) {
    return static_cast<uint8>(std::clamp(val, 0.0f, 1.0f) * 255.0f);
  };

  return {to_u8(r), to_u8(g), to_u8(b), to_u8(a)};
}

auto Color::from(const Span<const float, 4> rgba) -> Color
{
  return from(rgba[0], rgba[1], rgba[2], rgba[3]);
}

auto Color::parse_rgb(const StringView rgb) -> Result<Color>
{
  if (rgb.length() != 7 || rgb.front() != '#') {
    return unexpected(make_error(GenericError::kInvalidParam));
  }

  const auto r = parse_uint(rgb.substr(1, 2), 16);
  const auto g = parse_uint(rgb.substr(3, 2), 16);
  const auto b = parse_uint(rgb.substr(5, 2), 16);

  if (r && g && b) {
    return Color {narrow_cast<uint8>(*r),
                  narrow_cast<uint8>(*g),
                  narrow_cast<uint8>(*b),
                  0xFF};
  }

  return unexpected(make_error(GenericError::kInvalidParam));
}

auto Color::parse_rgba(const StringView rgba) -> Result<Color>
{
  if (rgba.length() != 9 || rgba.front() != '#') {
    return unexpected(make_error(GenericError::kInvalidParam));
  }

  const auto r = parse_uint(rgba.substr(1, 2), 16);
  const auto g = parse_uint(rgba.substr(3, 2), 16);
  const auto b = parse_uint(rgba.substr(5, 2), 16);
  const auto a = parse_uint(rgba.substr(7, 2), 16);

  if (r && g && b && a) {
    return Color {narrow_cast<uint8>(*r),
                  narrow_cast<uint8>(*g),
                  narrow_cast<uint8>(*b),
                  narrow_cast<uint8>(*a)};
  }

  return unexpected(make_error(GenericError::kInvalidParam));
}

auto Color::parse_argb(const StringView argb) -> Result<Color>
{
  if (argb.length() != 9 || argb.front() != '#') {
    return unexpected(make_error(GenericError::kInvalidParam));
  }

  const auto a = parse_uint(argb.substr(1, 2), 16);
  const auto r = parse_uint(argb.substr(3, 2), 16);
  const auto g = parse_uint(argb.substr(5, 2), 16);
  const auto b = parse_uint(argb.substr(7, 2), 16);

  if (a && r && g && b) {
    return Color {narrow_cast<uint8>(*r),
                  narrow_cast<uint8>(*g),
                  narrow_cast<uint8>(*b),
                  narrow_cast<uint8>(*a)};
  }

  return unexpected(make_error(GenericError::kInvalidParam));
}

auto Color::to_string_rgb() const -> String
{
  return fmt::format("#{:02X}{:02X}{:02X}", red, green, blue);
}

auto Color::to_string_rgba() const -> String
{
  return fmt::format("#{:02X}{:02X}{:02X}{:02X}", red, green, blue, alpha);
}

auto Color::to_string_argb() const -> String
{
  return fmt::format("#{:02X}{:02X}{:02X}{:02X}", alpha, red, green, blue);
}

auto Color::normalized() const -> Array<float, 4>
{
  const auto to_f = [](const uint8 val) { return static_cast<float>(val) / 255.0f; };
  return {to_f(red), to_f(green), to_f(blue), to_f(alpha)};
}

auto Color::get_luminance() const -> float
{
  // https://en.wikipedia.org/wiki/SRGB#From_sRGB_to_CIE_XYZ
  auto linearize = [](const float channel) {
    if (channel <= 0.04045f) {
      return channel / 12.92f;
    }

    return std::pow((channel + 0.055f) / 1.055f, 2.4f);
  };

  const auto norm_values = normalized();
  const auto lr = linearize(norm_values[0]);
  const auto lg = linearize(norm_values[1]);
  const auto lb = linearize(norm_values[2]);

  return (0.2126f * lr) + (0.7152f * lg) + (0.0722f * lb);
}

auto Color::is_dark() const -> bool
{
  return get_luminance() < 0.3f;
}

auto operator<<(std::ostream& stream, const Color& color) -> std::ostream&
{
  return stream << color.to_string_rgba();
}

}  // namespace tactile
