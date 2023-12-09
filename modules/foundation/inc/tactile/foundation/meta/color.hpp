// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <ostream>  // ostream

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

struct TACTILE_FOUNDATION_API UColor final {
  uint8 red {};
  uint8 green {};
  uint8 blue {};
  uint8 alpha {};

  [[nodiscard]] constexpr auto operator==(const UColor&) const noexcept -> bool = default;
};

struct TACTILE_FOUNDATION_API NColor final {
  float red {};
  float green {};
  float blue {};
  float alpha {};

  [[nodiscard]] constexpr auto operator==(const NColor&) const noexcept -> bool = default;
};

inline constexpr UColor kColorWhite = {0xFF, 0xFF, 0xFF, 0xFF};
inline constexpr UColor kColorBlack = {0x00, 0x00, 0x00, 0xFF};

[[nodiscard]]
TACTILE_FOUNDATION_API auto to_color_rgb(StringView rgb) -> Maybe<UColor>;

[[nodiscard]]
TACTILE_FOUNDATION_API auto to_color_rgba(StringView rgba) -> Maybe<UColor>;

[[nodiscard]]
TACTILE_FOUNDATION_API auto to_color_argb(StringView argb) -> Maybe<UColor>;

[[nodiscard]]
TACTILE_FOUNDATION_API auto normalize(const UColor& color) -> NColor;

[[nodiscard]]
TACTILE_FOUNDATION_API auto unnormalize(const NColor& color) -> UColor;

/**
 * Computes the relative luminance of a color.
 *
 * \details This function is particularly useful for determining the appropriate
 *          foreground text color given a background of a specific color.
 *
 * \param color a normalized color.
 *
 * \return a luminance value in the interval [0, 1].
 *
 * \see https://en.wikipedia.org/wiki/Relative_luminance
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto get_luminance(const NColor& color) -> float;

/** Indicates whether a color is considered to be dark. */
[[nodiscard]]
TACTILE_FOUNDATION_API auto is_dark(const NColor& color) -> bool;

[[nodiscard]]
TACTILE_FOUNDATION_API auto to_string_rgb(const UColor& color) -> String;

[[nodiscard]]
TACTILE_FOUNDATION_API auto to_string_rgba(const UColor& color) -> String;

[[nodiscard]]
TACTILE_FOUNDATION_API auto to_string_argb(const UColor& color) -> String;

TACTILE_FOUNDATION_API
auto operator<<(std::ostream& stream, const UColor& color) -> std::ostream&;

TACTILE_FOUNDATION_API
auto operator<<(std::ostream& stream, const NColor& color) -> std::ostream&;

}  // namespace tactile
