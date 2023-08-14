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

#include <optional>     // optional
#include <string>       // string
#include <string_view>  // string_view

#include "tactile/core/common/prelude.hpp"
#include "tactile/core/core.hpp"

namespace tactile {

/** An 8-bit precision RGBA color. */
struct TACTILE_CORE_API UColor final {
  uint8 red {};
  uint8 green {};
  uint8 blue {};
  uint8 alpha {0xFF};

  [[nodiscard]] constexpr auto operator==(const UColor&) const -> bool = default;
};

/** A normalized RGBA color. */
struct TACTILE_CORE_API NColor final {
  float red {};
  float green {};
  float blue {};
  float alpha {1.0f};

  [[nodiscard]] constexpr auto operator==(const NColor&) const -> bool = default;
};

[[nodiscard]] TACTILE_CORE_API auto to_color_rgb(std::string_view rgb)
    -> std::optional<UColor>;

[[nodiscard]] TACTILE_CORE_API auto to_color_rgba(std::string_view rgba)
    -> std::optional<UColor>;

[[nodiscard]] TACTILE_CORE_API auto to_color_argb(std::string_view argb)
    -> std::optional<UColor>;

[[nodiscard]] TACTILE_CORE_API auto normalize(const UColor& color) -> NColor;

[[nodiscard]] TACTILE_CORE_API auto unnormalize(const NColor& color) -> UColor;

/**
 * Computes the relative luminance of a color.
 *
 * \details This function is useful when determining appropriate text color for a
 *          background of a certain color.
 *
 * \param color the target color.
 *
 * \return a luminance value in the range [0, 1].
 *
 * \see https://en.wikipedia.org/wiki/Relative_luminance
 */
[[nodiscard]] TACTILE_CORE_API auto get_luminance(const NColor& color) -> float;

/** Indicates whether a color is considered to be dark. */
[[nodiscard]] TACTILE_CORE_API auto is_dark_color(const NColor& color) -> bool;

[[nodiscard]] TACTILE_CORE_API auto to_string_rgb(const UColor& color) -> std::string;

[[nodiscard]] TACTILE_CORE_API auto to_string_rgba(const UColor& color) -> std::string;

[[nodiscard]] TACTILE_CORE_API auto to_string_argb(const UColor& color) -> std::string;

inline constexpr UColor kWhiteColor = UColor {0xFF, 0xFF, 0xFF, 0xFF};
inline constexpr UColor kBlackColor = UColor {0x00, 0x00, 0x00, 0xFF};

}  // namespace tactile