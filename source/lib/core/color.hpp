// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/array.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/int.hpp"

namespace tactile {

class Color final {
 public:
  uint8 red {0};
  uint8 green {0};
  uint8 blue {0};
  uint8 alpha {0xFF};

  [[nodiscard]] static auto from_norm(float r, float g, float b, float a = 1.0f) -> Color;

  [[nodiscard]] static auto from_rgb(StringView rgb) -> Maybe<Color>;
  [[nodiscard]] static auto from_rgba(StringView rgba) -> Maybe<Color>;
  [[nodiscard]] static auto from_argb(StringView argb) -> Maybe<Color>;

  /// Computes the relative luminance of the color.
  ///
  /// \details
  /// This function is useful when determining appropriate text color for a background
  /// of a certain color.
  ///
  /// \see
  /// https://en.wikipedia.org/wiki/Relative_luminance
  ///
  /// \return the color luminance, in the interval [0, 1].
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

  [[nodiscard]] auto as_rgb() const -> String;
  [[nodiscard]] auto as_rgba() const -> String;
  [[nodiscard]] auto as_argb() const -> String;

  /// Returns the color as an array of normalized color channels, in RGBA format.
  [[nodiscard]] auto as_float_array() const -> Array<float, 4>;

  [[nodiscard]] constexpr bool operator==(const Color&) const noexcept = default;
};

inline constexpr Color kWhite = {0xFF, 0xFF, 0xFF};
inline constexpr Color kBlack = {0, 0, 0};
inline constexpr Color kYellow = {0xFF, 0xFF, 0};

}  // namespace tactile
