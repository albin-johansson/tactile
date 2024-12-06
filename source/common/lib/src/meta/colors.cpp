// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/common/meta/colors.hpp"

#include <format>
#include <stdexcept>

#include "tactile/common/numeric/charconv.hpp"

namespace tactile::common {

auto to_string(const UColor& color, const ColorFormat format) -> std::string
{
  switch (format) {
    case ColorFormat::kRgb:
      return std::format("#{:02X}{:02X}{:02X}", color.red, color.green, color.blue);

    case ColorFormat::kRgba:
      return std::format("#{:02X}{:02X}{:02X}{:02X}",
                         color.red,
                         color.green,
                         color.blue,
                         color.alpha);

    case ColorFormat::kArgb:
      return std::format("#{:02X}{:02X}{:02X}{:02X}",
                         color.alpha,
                         color.red,
                         color.green,
                         color.blue);
  }

  throw std::invalid_argument {"bad color format"};
}

auto parse_color_rgb(const std::string_view rgb) noexcept -> std::optional<UColor>
{
  if (rgb.length() != 7 || rgb.front() != '#') {
    return std::nullopt;
  }

  const auto r = parse_uint(rgb.substr(1, 2), 16);
  const auto g = parse_uint(rgb.substr(3, 2), 16);
  const auto b = parse_uint(rgb.substr(5, 2), 16);

  if (r.has_value() && g.has_value() && b.has_value()) {
    return UColor {static_cast<std::uint8_t>(*r),
                   static_cast<std::uint8_t>(*g),
                   static_cast<std::uint8_t>(*b),
                   0xFF};
  }

  return std::nullopt;
}

auto parse_color_rgba(const std::string_view rgba) noexcept -> std::optional<UColor>
{
  if (rgba.length() != 9 || rgba.front() != '#') {
    return std::nullopt;
  }

  const auto r = parse_uint(rgba.substr(1, 2), 16);
  const auto g = parse_uint(rgba.substr(3, 2), 16);
  const auto b = parse_uint(rgba.substr(5, 2), 16);
  const auto a = parse_uint(rgba.substr(7, 2), 16);

  if (r.has_value() && g.has_value() && b.has_value() && a.has_value()) {
    return UColor {static_cast<std::uint8_t>(*r),
                   static_cast<std::uint8_t>(*g),
                   static_cast<std::uint8_t>(*b),
                   static_cast<std::uint8_t>(*a)};
  }

  return std::nullopt;
}

auto parse_color_argb(const std::string_view argb) noexcept -> std::optional<UColor>
{
  if (argb.length() != 9 || argb.front() != '#') {
    return std::nullopt;
  }

  const auto a = parse_uint(argb.substr(1, 2), 16);
  const auto r = parse_uint(argb.substr(3, 2), 16);
  const auto g = parse_uint(argb.substr(5, 2), 16);
  const auto b = parse_uint(argb.substr(7, 2), 16);

  if (a.has_value() && r.has_value() && g.has_value() && b.has_value()) {
    return UColor {static_cast<std::uint8_t>(*r),
                   static_cast<std::uint8_t>(*g),
                   static_cast<std::uint8_t>(*b),
                   static_cast<std::uint8_t>(*a)};
  }

  return std::nullopt;
}

}  // namespace tactile::common
