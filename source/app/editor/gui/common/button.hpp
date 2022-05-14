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

namespace tactile {

/**
 * \brief Shows a button.
 *
 * \ingroup gui
 *
 * \param text the button text.
 * \param tooltip optional tooltip text.
 * \param enabled `true` to show an enabled button; `false` otherwise.
 * \param width the width of the button; `0` for automatic width.
 * \param height the height of the button; `0` for automatic height.
 *
 * \return `true` if the button was pressed; `false` otherwise.
 */
auto button(const char* text,
            const char* tooltip = nullptr,
            bool enabled = true,
            float width = 0,
            float height = 0) -> bool;

auto icon_button(const char* icon, const char* tooltip = nullptr, bool enabled = true)
    -> bool;

/**
 * \brief Shows a horizontally centered button.
 *
 * \ingroup gui
 *
 * \param label the label text.
 * \param tooltip optional tooltip text.
 *
 * \return `true` if the button was pressed; `false` otherwise.
 */
auto centered_button(const char* label, const char* tooltip = nullptr) -> bool;

}  // namespace tactile
