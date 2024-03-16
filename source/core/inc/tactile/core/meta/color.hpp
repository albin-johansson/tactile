// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <ostream>  // ostream

#include "tactile/base/container/array.hpp"
#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/span.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/int.hpp"

namespace tactile {

/** Represents an 8-bit RGBA color. */
class Color final {
 public:
  uint8 red {};
  uint8 green {};
  uint8 blue {};
  uint8 alpha {};

  /**
   * Creates a color from floating-point components.
   *
   * \details
   *    The provided intensities are clamped to be within [0, 1],
   *    and subsequently mapped to values in the [0, 255] interval.
   *
   * \param r The red channel intensity.
   * \param g The green channel intensity.
   * \param b The blue channel intensity.
   * \param a The alpha channel intensity.
   *
   * \return
   *    A color.
   */
  [[nodiscard]]
  static auto from(float r, float g, float b, float a) -> Color;

  [[nodiscard]]
  static auto from(Span<const float, 4> rgba) -> Color;

  /**
   * Parses a color from a hexadecimal RGB color code.
   *
   * \param rgb An RGB color string, on the form \c "#RRGGBB".
   *
   * \return
   *    A color if successful; an error code otherwise.
   */
  [[nodiscard]]
  static auto parse_rgb(StringView rgb) -> Result<Color>;

  /**
   * Parses a color from a hexadecimal RGBA color code.
   *
   * \param rgba An RGBA color string, on the form \c "#RRGGBBAA".
   *
   * \return
   *    A color if successful; an error code otherwise.
   */
  [[nodiscard]]
  static auto parse_rgba(StringView rgba) -> Result<Color>;

  /**
   * Parses a color from a hexadecimal ARGB color code.
   *
   * \param argb An ARGB color string, on the form \c "#AARRGGBB".
   *
   * \return
   *    A color if successful; an error code otherwise.
   */
  [[nodiscard]]
  static auto parse_argb(StringView argb) -> Result<Color>;

  /**
   * Converts the color to a hexadecimal RGB color code.
   *
   * \return
   *    An RGB color code.
   */
  [[nodiscard]]
  auto to_string_rgb() const -> String;

  /**
   * Converts the color to a hexadecimal RGBA color code.
   *
   * \return
   *    An RGBA color code.
   */
  [[nodiscard]]
  auto to_string_rgba() const -> String;

  /**
   * Converts the color to a hexadecimal ARGB color code.
   *
   * \return
   *    An ARGB color code.
   */
  [[nodiscard]]
  auto to_string_argb() const -> String;

  [[nodiscard]]
  auto normalized() const -> Array<float, 4>;

  /**
   * Returns the relative luminance of the color.
   *
   * \return
   *    A luminance value in the interval [0, 1].
   *
   * \see https://en.wikipedia.org/wiki/Relative_luminance
   */
  [[nodiscard]]
  auto get_luminance() const -> float;

  /**
   * Indicates whether the color is considered dark.
   *
   * \return
   *    True if the color is dark; false otherwise.
   */
  [[nodiscard]]
  auto is_dark() const -> bool;

  [[nodiscard]] constexpr auto operator==(const Color&) const noexcept -> bool = default;
};

inline constexpr Color kColorBlack {0x00, 0x00, 0x00, 0xFF};
inline constexpr Color kColorWhite {0xFF, 0xFF, 0xFF, 0xFF};
inline constexpr Color kColorYellow {0xFF, 0xFF, 0x00, 0xFF};

auto operator<<(std::ostream& stream, const Color& color) -> std::ostream&;

}  // namespace tactile
