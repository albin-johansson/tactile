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

#include "core/model.hpp"
#include "editor/ui/scoped.hpp"
#include "misc/panic.hpp"

namespace tactile::ui {

struct ADockWidget::Data final
{
  const char* title{};
  ImGuiWindowFlags window_flags{};
  ImGuiFocusedFlags focus_flags{};
  bool has_close_button{};
  bool has_focus{};
  bool contains_mouse{};
};

ADockWidget::ADockWidget(const char* title, const ImGuiWindowFlags flags)
    : mData{std::make_unique<Data>()}
{
  mData->title = title;
  mData->window_flags = flags;
  if (!mData->title) {
    throw TactileError("Invalid null dock widget title!");
  }
}

ADockWidget::~ADockWidget() noexcept = default;

void ADockWidget::update(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  bool visible = is_visible();

  if (!visible) {
    return;
  }

  bool* show = mData->has_close_button ? &visible : nullptr;

  scoped::Window dock{mData->title, mData->window_flags, show};
  mData->has_focus = dock.has_focus(mData->focus_flags);
  mData->contains_mouse = ImGui::IsWindowHovered(mData->focus_flags);

  if (dock.is_open()) {
    on_update(model, dispatcher);
  }

  set_visible(visible);
}

void ADockWidget::set_close_button_enabled(const bool enabled)
{
  mData->has_close_button = enabled;
}

void ADockWidget::set_focus_flags(const ImGuiFocusedFlags flags)
{
  mData->focus_flags = flags;
}

auto ADockWidget::has_focus() const -> bool
{
  return mData->has_focus;
}

auto ADockWidget::has_mouse_hover() const -> bool
{
  return mData->contains_mouse;
}

}  // namespace tactile::ui