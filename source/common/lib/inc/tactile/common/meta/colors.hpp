// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <optional>
#include <string>
#include <string_view>

#include "tactile/base/meta/color.hpp"

namespace tactile::common {

enum class ColorFormat : std::uint8_t
{
  kRgb,
  kRgba,
  kArgb
};

/// Converts a given color to a hexadecimal color code.
[[nodiscard]]
auto to_string(const UColor& color, ColorFormat format = ColorFormat::kRgba) -> std::string;

/// Parses a color from a hexadecimal RGB color code.
[[nodiscard]]
auto parse_color_rgb(std::string_view rgb) noexcept -> std::optional<UColor>;

/// Parses a color from a hexadecimal RGBA color code.
[[nodiscard]]
auto parse_color_rgba(std::string_view rgba) noexcept -> std::optional<UColor>;

/// Parses a color from a hexadecimal ARGB color code.
[[nodiscard]]
auto parse_color_argb(std::string_view argb) noexcept -> std::optional<UColor>;

}  // namespace tactile::common
