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

#include "viewport_area.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "core/common/maybe.hpp"
#include "core/mouse_info.hpp"

namespace tactile::ui {
namespace {

[[nodiscard]] auto check_for(auto&& query) -> Maybe<cen::mouse_button>
{
  const auto left = query(ImGuiMouseButton_Left);
  const auto mid = query(ImGuiMouseButton_Middle);
  const auto right = query(ImGuiMouseButton_Right);

  if (left) {
    return cen::mouse_button::left;
  }
  else if (mid) {
    return cen::mouse_button::middle;
  }
  else if (right) {
    return cen::mouse_button::right;
  }
  else {
    return nothing;
  }
}

}  // namespace

auto ViewportArea::get_size() const -> ImVec2
{
  const auto pos = ImGui::GetWindowPos();
  return (pos + ImGui::GetWindowSize()) - pos;
}

void ViewportArea::check_mouse_events(const entt::registry& registry,
                                      entt::dispatcher& dispatcher)
{
  mContainsMouse = false;

  if (!ImGui::IsWindowFocused() && !ImGui::IsWindowHovered()) {
    return;
  }

  ImGui::InvisibleButton("InvisibleViewportAreaButton", get_size());

  if (ImGui::IsItemHovered()) {
    mContainsMouse = true;

    check_clicked(registry, dispatcher);
    check_dragged(registry, dispatcher);
    check_released(registry, dispatcher);
  }
}

auto ViewportArea::contains_mouse() const -> bool
{
  return mContainsMouse;
}

void ViewportArea::check_clicked(const entt::registry& registry,
                                 entt::dispatcher& dispatcher)
{
  const auto button = check_for(
      [](const ImGuiMouseButton button) { return ImGui::IsMouseClicked(button); });

  if (button) {
    on_clicked(*button, registry, dispatcher);
  }
}

void ViewportArea::check_dragged(const entt::registry& registry,
                                 entt::dispatcher& dispatcher)
{
  const auto button = check_for(
      [](const ImGuiMouseButton button) { return ImGui::IsMouseDragging(button); });

  if (button) {
    on_dragged(*button, registry, dispatcher);
  }
}

void ViewportArea::check_released(const entt::registry& registry,
                                  entt::dispatcher& dispatcher)
{
  const auto button = check_for(
      [](const ImGuiMouseButton button) { return ImGui::IsMouseReleased(button); });

  if (button) {
    on_released(*button, registry, dispatcher);
  }
}

}  // namespace tactile::ui