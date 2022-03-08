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

#include "dock_widget.hpp"

#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"
#include "misc/throw.hpp"

namespace tactile {

dock_widget::dock_widget(const char* title, const ImGuiWindowFlags flags)
    : mTitle{title}
    , mWindowFlags{flags}
{
  if (!mTitle) {
    throw_traced(tactile_error{"Invalid null dock widget title!"});
  }
}

void dock_widget::update(const document_model& model, entt::dispatcher& dispatcher)
{
  bool visible = is_visible();

  if (!visible) {
    return;
  }

  bool* show = mHasCloseButton ? &visible : nullptr;

  scoped::window dock{mTitle, mWindowFlags, show};
  mHasFocus = dock.has_focus();

  if (dock.is_open()) {
    on_update(model, dispatcher);
  }

  set_visible(visible);
}

void dock_widget::set_close_button_enabled(const bool enabled)
{
  mHasCloseButton = enabled;
}

void dock_widget::set_focus_flags(const ImGuiFocusedFlags flags)
{
  mFocusFlags = flags;
}

}  // namespace tactile