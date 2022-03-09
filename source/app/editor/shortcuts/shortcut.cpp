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

#include "shortcut.hpp"

namespace tactile {

void AShortcut::Poll(const DocumentModel& model,
                     const widget_manager& widgets,
                     const cen::keyboard_event& event,
                     entt::dispatcher& dispatcher)
{
  if (IsEnabled(model, widgets)) {
    if (event.pressed() && event.scan() == mKey && event.is_only_active(mModifiers)) {
      Activate(dispatcher);
    }
  }
}

}  // namespace tactile
