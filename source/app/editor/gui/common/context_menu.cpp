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

#include "editor/gui/scoped.hpp"
#include "misc/throw.hpp"

namespace tactile {

context_menu::context_menu(const char* name) : mName{name}
{
  if (!mName) {
    throw_traced(tactile_error{"Invalid null context menu name!"});
  }
}

void context_menu::update(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (scoped::Popup popup{mName}; popup.is_open()) {
    on_update(model, dispatcher);
  }

  if (mShow) {
    ImGui::OpenPopup(mName);
    mShow = false;
  }
}

void context_menu::show()
{
  mShow = true;
}

}  // namespace tactile