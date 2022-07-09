/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include <array>  // array

struct ImGuiStyle;

namespace tactile::ui {

/// \addtogroup gui
/// \{

/// \name Theme API
/// \{

/**
 * Represents the different available themes.
 *
 * \warning Do not change the enumerator values!
 *
 * \see proto::Theme
 */
enum class EditorTheme
{
  DearDark = 0,   ///< The standard Dear ImGui dark theme.
  DearLight = 1,  ///< The standard Dear ImGui light theme.
  Ruby = 2,
  Sapphire = 3,
  Emerald = 4,
  Amethyst = 5,
  Amber = 6,
  Nocturnal = 7,
  Ash = 8,
  Diamond = 9,
  Joker = 10,
  Raspberry = 11
};

constexpr std::array themes = {EditorTheme::DearDark,
                               EditorTheme::DearLight,
                               EditorTheme::Ruby,
                               EditorTheme::Sapphire,
                               EditorTheme::Emerald,
                               EditorTheme::Amethyst,
                               EditorTheme::Amber,
                               EditorTheme::Diamond,
                               EditorTheme::Joker,
                               EditorTheme::Raspberry,
                               EditorTheme::Nocturnal,
                               EditorTheme::Ash};

[[nodiscard]] auto human_readable_name(EditorTheme theme) -> std::string_view;

/**
 * Applies the styling used by Tactile to a style instance.
 *
 * Note, this function does not affect the colors of the style.
 *
 * \param style the style that will be updated.
 */
void apply_style(ImGuiStyle& style);

/**
 * Applies a theme to a style.
 *
 * \param style the style that will be affected.
 * \param theme the theme that will be applied to the style.
 */
void apply_theme(ImGuiStyle& style, EditorTheme theme);

/// \} End of theme API

/// \} End of group gui

}  // namespace tactile::ui
