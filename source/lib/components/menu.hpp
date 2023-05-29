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

#include "common/enum/menu_action.hpp"
#include "common/type/func.hpp"
#include "common/type/hash_map.hpp"
#include "common/type/string.hpp"
#include "model/model.hpp"

namespace tactile {

using MenuItemEnabledFn = Func<bool(const Model&)>;

struct MenuItem final {
  String label;         ///< The label text, in the current language.
  String shortcut;      ///< Optional shortcut label (language independent).
  bool enabled : 1 {};  ///< Can the menu item be activated?
  bool checked : 1 {};  ///< Should the menu item be rendered with a checkmark?
};

struct MenuItemCallbacks final {
  MenuItemEnabledFn enabled_fn;  ///< Used to determine when the menu item is enabled.
};

/// Context component that tracks all menu items.
struct MenuItems final {
  HashMap<MenuAction, Entity> items;
};

}  // namespace tactile
