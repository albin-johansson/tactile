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

namespace tactile {

/// \addtogroup gui
/// \{

/// \name Theme API
/// \{

/**
 * \brief Represents the different available themes.
 *
 * \warning Do not change the enumerator values!
 *
 * \see proto::Theme
 */
enum class EditorTheme
{
  dear_dark = 0,   ///< The standard Dear ImGui dark theme.
  dear_light = 1,  ///< The standard Dear ImGui light theme.
  ruby = 2,
  sapphire = 3,
  emerald = 4,
  amethyst = 5,
  amber = 6,
  nocturnal = 7,
  ash = 8,
  diamond = 9,
  joker = 10,
  raspberry = 11
};

constexpr std::array themes = {EditorTheme::dear_dark,
                               EditorTheme::dear_light,
                               EditorTheme::ruby,
                               EditorTheme::sapphire,
                               EditorTheme::emerald,
                               EditorTheme::amethyst,
                               EditorTheme::amber,
                               EditorTheme::diamond,
                               EditorTheme::joker,
                               EditorTheme::raspberry,
                               EditorTheme::nocturnal,
                               EditorTheme::ash};

[[nodiscard]] auto human_readable_name(EditorTheme theme) -> std::string_view;

/**
 * \brief Applies the styling used by Tactile to a style instance.
 *
 * \note This function does not affect the colors of the style.
 *
 * \param style the style that will be updated.
 */
void apply_style(ImGuiStyle& style);

/**
 * \brief Applies a theme to a style.
 *
 * \param style the style that will be affected.
 * \param theme the theme that will be applied to the style.
 */
void apply_theme(ImGuiStyle& style, EditorTheme theme);

/// \} End of theme API

/// \} End of group gui

}  // namespace tactile
