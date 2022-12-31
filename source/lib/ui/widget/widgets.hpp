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

#include <entt/signal/fwd.hpp>

#include "ui/menu/menu.hpp"

namespace tactile::ui {

/// Shows a button.
///
/// \param text the button text.
/// \param tooltip optional tooltip text.
/// \param enabled true to show an enabled button; false otherwise.
/// \param width the width of the button; 0 for automatic width.
/// \param height the height of the button; 0 for automatic height.
///
/// \return true if the button was pressed; false otherwise.
auto ui_button(const char* text,
               const char* tooltip = nullptr,
               bool enabled = true,
               float width = 0,
               float height = 0) -> bool;

/// Shows a square button, intended to be used with icon characters.
auto ui_icon_button(const char* icon, const char* tooltip = nullptr, bool enabled = true)
    -> bool;

/// Shows a button that is horizontally centered in the parent item.
auto ui_centered_button(const char* label, const char* tooltip = nullptr) -> bool;

auto ui_checkbox(const char* label, bool* value, const char* tooltip = nullptr) -> bool;

/// Shows a tooltip after the last item has been hovered for a while.
void ui_lazy_tooltip(const char* id, const char* tooltip);

/// Shows a label that is horizontally centered in the parent item.
void ui_centered_label(const char* text);

/// Displays a menu item for a specific action.
/// The menu item label and state is automatically determined from the menu action.
/// The shortcut parameter is the shortcut hint displayed next to the item.
void ui_menu_item(entt::dispatcher& dispatcher,
                  MenuAction action,
                  const char* shortcut = nullptr);

}  // namespace tactile::ui
