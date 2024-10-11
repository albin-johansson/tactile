// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <algorithm>    // clamp
#include <cstdint>      // uint8_t, uint32_t
#include <format>       // format
#include <optional>     // optional, nullopt
#include <ostream>      // ostream
#include <stdexcept>    // invalid_argument
#include <string>       // string
#include <string_view>  // string_view

#include "tactile/base/io/int_parser.hpp"

namespace tactile {

enum class ColorFormat : std::uint8_t
{
  kRgb,
  kRgba,
  kArgb
};

/**
 * Represents an 8-bit RGBA color.
 *
 * \see FColor
 */
struct UColor final
{
  using value_type = std::uint8_t;

  value_type red;
  value_type green;
  value_type blue;
  value_type alpha;

  [[nodiscard]]
  constexpr auto operator==(const UColor&) const noexcept -> bool = default;
};

/**
 * Represents a normalized floating-point RGBA color.
 *
 * \see UColor
 */
struct FColor final
{
  using value_type = float;

  value_type red;
  value_type green;
  value_type blue;
  value_type alpha;

  [[nodiscard]]
  constexpr auto operator==(const FColor&) const noexcept -> bool = default;
};

[[nodiscard]]
constexpr auto to_ucolor(const FColor& color) -> UColor
{
  const auto to_u8 = [](const float value) {
    return static_cast<std::uint8_t>(std::clamp(value, 0.0f, 1.0f) * 255.0f);
  };

  return {to_u8(color.red), to_u8(color.green), to_u8(color.blue), to_u8(color.alpha)};
}

[[nodiscard]]
constexpr auto to_fcolor(const UColor& color) -> FColor
{
  const auto to_f = [](const std::uint8_t value) {
    return static_cast<float>(value) / 255.0f;
  };

  return {to_f(color.red), to_f(color.green), to_f(color.blue), to_f(color.alpha)};
}

/**
 * Encodes the color as an ABGR color packed into a 32-bit integer.
 *
 * \return
 * An 32-bit integer, where each octet represents a different color channel.
 */
[[nodiscard]]
constexpr auto to_uint32_abgr(const UColor& color) -> std::uint32_t
{
  const auto a = static_cast<std::uint32_t>(color.alpha) << std::uint32_t {24};
  const auto b = static_cast<std::uint32_t>(color.blue) << std::uint32_t {16};
  const auto g = static_cast<std::uint32_t>(color.green) << std::uint32_t {8};
  const auto r = static_cast<std::uint32_t>(color.red) << std::uint32_t {0};
  return a | b | g | r;
}

/**
 * Converts the color to a hexadecimal color code.
 *
 * \param color  The color to convert.
 * \param format The output format to use.
 *
 * \return
 * A color code string.
 */
[[nodiscard]]
inline auto to_string(const UColor& color,
                      const ColorFormat format = ColorFormat::kRgba) -> std::string
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

/**
 * Parses a color from a hexadecimal RGB color code.
 *
 * \param rgb An RGB color string, on the form \c "#RRGGBB".
 *
 * \return
 * A color if successful; an empty optional otherwise.
 */
[[nodiscard]]
constexpr auto parse_color_rgb(const std::string_view rgb) noexcept -> std::optional<UColor>
{
  if (rgb.length() != 7 || rgb.front() != '#') {
    return std::nullopt;
  }

  const auto r = parse<UColor::value_type>(rgb.substr(1, 2), 16);
  const auto g = parse<UColor::value_type>(rgb.substr(3, 2), 16);
  const auto b = parse<UColor::value_type>(rgb.substr(5, 2), 16);

  if (r.has_value() && g.has_value() && b.has_value()) {
    return UColor {static_cast<std::uint8_t>(*r),
                   static_cast<std::uint8_t>(*g),
                   static_cast<std::uint8_t>(*b),
                   0xFF};
  }

  return std::nullopt;
}

/**
 * Parses a color from a hexadecimal RGBA color code.
 *
 * \param rgba An RGBA color string, on the form \c "#RRGGBBAA".
 *
 * \return
 * A color if successful; an empty optional otherwise.
 */
[[nodiscard]]
constexpr auto parse_color_rgba(const std::string_view rgba) noexcept -> std::optional<UColor>
{
  if (rgba.length() != 9 || rgba.front() != '#') {
    return std::nullopt;
  }

  const auto r = parse<UColor::value_type>(rgba.substr(1, 2), 16);
  const auto g = parse<UColor::value_type>(rgba.substr(3, 2), 16);
  const auto b = parse<UColor::value_type>(rgba.substr(5, 2), 16);
  const auto a = parse<UColor::value_type>(rgba.substr(7, 2), 16);

  if (r.has_value() && g.has_value() && b.has_value() && a.has_value()) {
    return UColor {static_cast<std::uint8_t>(*r),
                   static_cast<std::uint8_t>(*g),
                   static_cast<std::uint8_t>(*b),
                   static_cast<std::uint8_t>(*a)};
  }

  return std::nullopt;
}

/**
 * Parses a color from a hexadecimal ARGB color code.
 *
 * \param argb An ARGB color string, on the form \c "#AARRGGBB".
 *
 * \return
 * A color if successful; an empty optional otherwise.
 */
[[nodiscard]]
constexpr auto parse_color_argb(const std::string_view argb) noexcept -> std::optional<UColor>
{
  if (argb.length() != 9 || argb.front() != '#') {
    return std::nullopt;
  }

  const auto a = parse<UColor::value_type>(argb.substr(1, 2), 16);
  const auto r = parse<UColor::value_type>(argb.substr(3, 2), 16);
  const auto g = parse<UColor::value_type>(argb.substr(5, 2), 16);
  const auto b = parse<UColor::value_type>(argb.substr(7, 2), 16);

  if (a.has_value() && r.has_value() && g.has_value() && b.has_value()) {
    return UColor {static_cast<std::uint8_t>(*r),
                   static_cast<std::uint8_t>(*g),
                   static_cast<std::uint8_t>(*b),
                   static_cast<std::uint8_t>(*a)};
  }

  return std::nullopt;
}

inline auto operator<<(std::ostream& stream, const UColor& color) -> std::ostream&
{
  return stream << to_string(color);
}

inline constexpr UColor kColorBlack {0x00, 0x00, 0x00, 0xFF};
inline constexpr UColor kColorWhite {0xFF, 0xFF, 0xFF, 0xFF};

}  // namespace tactile
