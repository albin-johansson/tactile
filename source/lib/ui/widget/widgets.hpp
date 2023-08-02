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

#include "core/ecs/registry.hpp"
#include "core/events/dispatcher.hpp"

namespace tactile::ui {

/**
 * Shows a button.
 *
 * \param text    the button text.
 * \param tooltip optional tooltip text.
 * \param enabled true to show an enabled button; false otherwise.
 * \param width   the width of the button; 0 for automatic width.
 * \param height  the height of the button; 0 for automatic height.
 *
 * \return true if the button was pressed; false otherwise.
 */
auto push_button(const char* text,
                 const char* tooltip = nullptr,
                 bool enabled = true,
                 float width = 0,
                 float height = 0) -> bool;

/// Shows a square button, intended to be used with icon characters.
auto push_icon_button(const char* icon,
                      const char* tooltip = nullptr,
                      bool enabled = true) -> bool;

/// Shows a button that is horizontally centered in the parent item.
auto push_centered_button(const char* label, const char* tooltip = nullptr) -> bool;

auto push_checkbox(const char* label, bool* value, const char* tooltip = nullptr) -> bool;

/// Shows a tooltip after the last item has been hovered for a while.
void push_lazy_tooltip(const char* id, const char* tooltip);

/// Shows a label that is horizontally centered in the parent item.
void push_centered_label(const char* text);

}  // namespace tactile::ui
