// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <optional>     // optional, nullopt
#include <string>       // string
#include <string_view>  // string_view

#include "tactile/base/io/int_parser.hpp"
#include "tactile/base/meta/color.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

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

}  // namespace tactile
