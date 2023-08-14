/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "tactile/core/containers/array.hpp"
#include "tactile/core/containers/string.hpp"
#include "tactile/core/functional/maybe.hpp"
#include "core/prelude.hpp"

namespace tactile {

/// Represents an 8-bit RGBA color.
class Color final {
 public:
  uint8 red {0};
  uint8 green {0};
  uint8 blue {0};
  uint8 alpha {0xFF};

  /**
   * Creates a color from normalized color components.
   *
   * \details The channel values are clamped within [0, 1].
   *
   * \param r the red channel value.
   * \param g the green channel value.
   * \param b the blue channel value.
   * \param a the alpha channel value.
   *
   * \return a color.
   */
  [[nodiscard]] static auto from_norm(float r, float g, float b, float a = 1.0f) -> Color;

  /**
   * Creates a color from a hexadecimal RGB color string, such as "#AABBCC".
   *
   * \param rgb an RGB color string.
   *
   * \return a potentially null color.
   */
  [[nodiscard]] static auto from_rgb(StringView rgb) -> Maybe<Color>;

  /**
   * Creates a color from a hexadecimal RGBA color string, such as "#AABBCCDD".
   *
   * \param rgba an RGBA color string.
   *
   * \return a potentially null color.
   */
  [[nodiscard]] static auto from_rgba(StringView rgba) -> Maybe<Color>;

  /**
   * Creates a color from a hexadecimal ARGB color string, such as "#AABBCCDD".
   *
   * \param argb an ARGB color string.
   *
   * \return a potentially null color.
   */
  [[nodiscard]] static auto from_argb(StringView argb) -> Maybe<Color>;

  /**
   * Computes the relative luminance of the color.
   *
   * \details This function is useful when determining appropriate text color for a
   *          background of a certain color.
   *
   * \see https://en.wikipedia.org/wiki/Relative_luminance
   *
   * \return the color luminance in the range [0, 1].
   */
  [[nodiscard]] auto get_luminance() const -> float;

  /// Indicates whether the color is considered to be dark.
  [[nodiscard]] auto is_dark() const -> bool;

  [[nodiscard]] constexpr auto norm_red() const noexcept -> float
  {
    return static_cast<float>(red) / 255.0f;
  }

  [[nodiscard]] constexpr auto norm_green() const noexcept -> float
  {
    return static_cast<float>(green) / 255.0f;
  }

  [[nodiscard]] constexpr auto norm_blue() const noexcept -> float
  {
    return static_cast<float>(blue) / 255.0f;
  }

  [[nodiscard]] constexpr auto norm_alpha() const noexcept -> float
  {
    return static_cast<float>(alpha) / 255.0f;
  }

  /// Creates a hexadecimal RGB color string that represents the color.
  [[nodiscard]] auto as_rgb() const -> String;

  /// Creates a hexadecimal RGBA color string that represents the color.
  [[nodiscard]] auto as_rgba() const -> String;

  /// Creates a hexadecimal ARGB color string that represents the color.
  [[nodiscard]] auto as_argb() const -> String;

  /// Returns the color as an array of normalized color channels, in RGBA format.
  [[nodiscard]] auto as_float_array() const -> Array<float, 4>;

  [[nodiscard]] constexpr auto operator==(const Color&) const noexcept -> bool = default;
};

inline constexpr Color kWhite = {0xFF, 0xFF, 0xFF};
inline constexpr Color kBlack = {0, 0, 0};
inline constexpr Color kYellow = {0xFF, 0xFF, 0};

}  // namespace tactile
