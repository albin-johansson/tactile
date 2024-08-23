// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <array>         // array
#include <expected>      // expected
#include <ostream>       // ostream
#include <span>          // span
#include <system_error>  // error_code

#include "tactile/base/container/string.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/meta/color.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Creates a color from floating-point components.
 *
 * \details
 * The provided intensities are clamped to be within [0, 1], and subsequently
 * mapped to values in the [0, 255] interval.
 *
 * \param r The red channel intensity.
 * \param g The green channel intensity.
 * \param b The blue channel intensity.
 * \param a The alpha channel intensity.
 *
 * \return
 * A color.
 */
[[nodiscard]]
auto make_color(float r, float g, float b, float a) -> UColor;

[[nodiscard]]
auto make_color(std::span<const float, 4> rgba) -> UColor;

[[nodiscard]]
auto make_color(const FColor& fcolor) -> UColor;

/**
 * Converts the color to a hexadecimal RGB color code.
 *
 * \return
 * An RGB color code.
 */
[[nodiscard]]
auto to_string_rgb(const UColor& color) -> String;

/**
 * Converts the color to a hexadecimal RGBA color code.
 *
 * \return
 * An RGBA color code.
 */
[[nodiscard]]
auto to_string_rgba(const UColor& color) -> String;

/**
 * Converts the color to a hexadecimal ARGB color code.
 *
 * \return
 * An ARGB color code.
 */
[[nodiscard]]
auto to_string_argb(const UColor& color) -> String;

/**
 * Encodes the color as an ABGR color packed into a 32-bit integer.
 *
 * \return
 * An 32-bit integer, where each octet represents a different color channel.
 */
[[nodiscard]]
auto to_uint32_abgr(const UColor& color) -> uint32;

[[nodiscard]]
auto normalize(const UColor& color) -> FColor;

/**
 * Returns the relative luminance of the color.
 *
 * \return
 * A luminance value in the interval [0, 1].
 *
 * \see https://en.wikipedia.org/wiki/Relative_luminance
 */
[[nodiscard]]
auto get_luminance(const UColor& color) -> float;

/**
 * Indicates whether the color is considered dark.
 *
 * \return
 * True if the color is dark; false otherwise.
 */
[[nodiscard]]
auto is_dark(const UColor& color) -> bool;

inline constexpr UColor kColorBlack {0x00, 0x00, 0x00, 0xFF};
inline constexpr UColor kColorWhite {0xFF, 0xFF, 0xFF, 0xFF};
inline constexpr UColor kColorYellow {0xFF, 0xFF, 0x00, 0xFF};

auto operator<<(std::ostream& stream, const UColor& color) -> std::ostream&;

}  // namespace tactile
