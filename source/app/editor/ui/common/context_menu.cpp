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

#include "context_menu.hpp"

#include <entt/signal/dispatcher.hpp>

#include "editor/ui/scoped.hpp"
#include "misc/panic.hpp"

namespace tactile::ui {

ContextMenu::ContextMenu(const char* name) : mName{name}
{
  if (!mName) {
    throw TactileError("Invalid null context menu name!");
  }
}

void ContextMenu::update(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (scoped::Popup popup{mName}; popup.is_open()) {
    on_update(model, dispatcher);
  }

  if (mShow) {
    ImGui::OpenPopup(mName);
    mShow = false;
  }
}

void ContextMenu::show()
{
  mShow = true;
}

}  // namespace tactile::ui