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

#include "tactile/core/prelude.hpp"
#include "tactile/core/type/array.hpp"
#include "tactile/core/type/string.hpp"

struct ImGuiStyle;

namespace tactile::ui {

/// Represents the different available themes.
/// The enumerator values cannot change, and should mirror those of proto::Theme.
enum class EditorTheme {
  DearDark = 0,   ///< The standard Dear ImGui dark theme.
  DearLight = 1,  ///< The standard Dear ImGui light theme.
  Ruby = 2,
  Sapphire = 3,
  Emerald = 4,
  Amethyst = 5,
  Amber = 6,
  Nocturnal = 7,
  Ash = 8,
  Diamond = 9,  // Unused
  Joker = 10,
  Raspberry = 11,
  Stealth = 12,
  Vanilla = 13,  // Unused
  Gasoline = 14,
  Bumblebee = 15,
  Lavender = 16,
  Frost = 17,
  Rose = 18,
};

inline constexpr Array<EditorTheme, 4> kLightThemes [[maybe_unused]] = {
    EditorTheme::DearLight,
    EditorTheme::Lavender,
    EditorTheme::Frost,
    EditorTheme::Rose,
};

inline constexpr Array<EditorTheme, 13> kDarkThemes [[maybe_unused]] = {
    EditorTheme::DearDark,
    EditorTheme::Ruby,
    EditorTheme::Sapphire,
    EditorTheme::Emerald,
    EditorTheme::Amethyst,
    EditorTheme::Amber,
    EditorTheme::Joker,
    EditorTheme::Raspberry,
    EditorTheme::Nocturnal,
    EditorTheme::Ash,
    EditorTheme::Stealth,
    EditorTheme::Gasoline,
    EditorTheme::Bumblebee,
};

[[nodiscard]] auto human_readable_name(EditorTheme theme) -> StringView;

/// Applies the styling used by Tactile to a style instance.
///
/// Note, this function does not affect the colors of the style.
///
/// \param style the style that will be updated.
void apply_style(ImGuiStyle& style);

/// Applies a theme to a style.
///
/// \param style the style that will be affected.
/// \param theme the theme that will be applied to the style.
/// \param saturation the accent color saturation, clamped to [0, 100].
void apply_theme(ImGuiStyle& style, EditorTheme theme, int32 saturation);

}  // namespace tactile::ui
