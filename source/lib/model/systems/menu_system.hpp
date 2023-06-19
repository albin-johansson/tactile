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

#include "common/type/dispatcher.hpp"
#include "model/components/language.hpp"
#include "model/components/menu.hpp"
#include "model/model.hpp"

namespace tactile::sys {

void init_menus(Model& model);

void update_menu_items(Model& model, Dispatcher& dispatcher);

void translate_menus(Model& model, const Strings& strings);

void retranslate_menus(Model& model);

[[nodiscard]] auto get_menu_item(const Model& model, MenuAction action)
    -> const MenuItem&;

}  // namespace tactile::sys
