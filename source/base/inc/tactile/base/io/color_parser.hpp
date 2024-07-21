// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/string.hpp"
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
constexpr auto parse_color_rgb(const StringView rgb) noexcept -> Optional<UColor>
{
  if (rgb.length() != 7 || rgb.front() != '#') {
    return kNone;
  }

  const auto r = parse<UColor::value_type>(rgb.substr(1, 2), 16);
  const auto g = parse<UColor::value_type>(rgb.substr(3, 2), 16);
  const auto b = parse<UColor::value_type>(rgb.substr(5, 2), 16);

  if (r.has_value() && g.has_value() && b.has_value()) {
    return UColor {static_cast<uint8>(*r),
                   static_cast<uint8>(*g),
                   static_cast<uint8>(*b),
                   0xFF};
  }

  return kNone;
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
constexpr auto parse_color_rgba(const StringView rgba) noexcept -> Optional<UColor>
{
  if (rgba.length() != 9 || rgba.front() != '#') {
    return kNone;
  }

  const auto r = parse<UColor::value_type>(rgba.substr(1, 2), 16);
  const auto g = parse<UColor::value_type>(rgba.substr(3, 2), 16);
  const auto b = parse<UColor::value_type>(rgba.substr(5, 2), 16);
  const auto a = parse<UColor::value_type>(rgba.substr(7, 2), 16);

  if (r.has_value() && g.has_value() && b.has_value() && a.has_value()) {
    return UColor {static_cast<uint8>(*r),
                   static_cast<uint8>(*g),
                   static_cast<uint8>(*b),
                   static_cast<uint8>(*a)};
  }

  return kNone;
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
constexpr auto parse_color_argb(const StringView argb) noexcept -> Optional<UColor>
{
  if (argb.length() != 9 || argb.front() != '#') {
    return kNone;
  }

  const auto a = parse<UColor::value_type>(argb.substr(1, 2), 16);
  const auto r = parse<UColor::value_type>(argb.substr(3, 2), 16);
  const auto g = parse<UColor::value_type>(argb.substr(5, 2), 16);
  const auto b = parse<UColor::value_type>(argb.substr(7, 2), 16);

  if (a.has_value() && r.has_value() && g.has_value() && b.has_value()) {
    return UColor {static_cast<uint8>(*r),
                   static_cast<uint8>(*g),
                   static_cast<uint8>(*b),
                   static_cast<uint8>(*a)};
  }

  return kNone;
}

}  // namespace tactile
