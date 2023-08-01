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

#include "common/enum/theme.hpp"
#include "common/type/array.hpp"
#include "common/type/string.hpp"
#include "core/prelude.hpp"

TACTILE_FWD_DECLARE_STRUCT(ImGuiStyle)

namespace tactile {

inline constexpr Array<Theme, 4> kLightThemes [[maybe_unused]] = {
    Theme::DearLight,
    Theme::Lavender,
    Theme::Frost,
    Theme::Rose,
};

inline constexpr Array<Theme, 13> kDarkThemes [[maybe_unused]] = {
    Theme::DearDark,
    Theme::Ruby,
    Theme::Sapphire,
    Theme::Emerald,
    Theme::Amethyst,
    Theme::Amber,
    Theme::Joker,
    Theme::Raspberry,
    Theme::Nocturnal,
    Theme::Ash,
    Theme::Stealth,
    Theme::Gasoline,
    Theme::Bumblebee,
};

[[nodiscard]] auto human_readable_name(Theme theme) -> StringView;

/**
 * Applies the styling used by Tactile to a style instance.
 *
 * \note This function does not affect the colors of the style.
 *
 * \param style the style that will be updated.
 */
void apply_style(ImGuiStyle& style);

/**
 * Applies a theme to a style.
 *
 * \param style      the style that will be affected.
 * \param theme      the theme that will be applied to the style.
 * \param saturation the accent color saturation, clamped to [0, 100].
 */
void apply_theme(ImGuiStyle& style, Theme theme, int32 saturation);

}  // namespace tactile::ui
