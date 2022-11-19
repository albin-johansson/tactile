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

#include "tileset_menu.hpp"

#include "editor/app_context.hpp"
#include "editor/ui/widget/menu_item.hpp"
#include "editor/ui/widget/scoped.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/model.hpp"

namespace tactile::ui {

void update_tileset_menu()
{
  const auto& model = get_model();
  const auto& lang = get_current_language();

  Disable disable {!model.is_tileset_active()};
  if (Menu menu {lang.menu.tileset.c_str()}; menu.is_open()) {
    menu_item(MenuAction::InspectTileset);
  }
}

}  // namespace tactile::ui