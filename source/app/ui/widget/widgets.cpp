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

#include "widgets.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "app/app_context.hpp"
#include "core/debug/assert.hpp"
#include "core/type/chrono.hpp"
#include "core/type/hash_map.hpp"
#include "core/type/maybe.hpp"
#include "model/event/menu_events.hpp"
#include "ui/style/alignment.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {

auto ui_button(const char* text,
               const char* tooltip,
               const bool enabled,
               const float width,
               const float height) -> bool
{
  TACTILE_ASSERT(text);

  const Disable disable {!enabled};

  const auto result = ImGui::Button(text, {width, height});

  if (tooltip) {
    ui_lazy_tooltip(text, tooltip);
  }

  if (ImGui::IsItemActive() || ImGui::IsItemHovered()) {
    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
  }

  return result;
}

auto ui_icon_button(const char* icon, const char* tooltip, const bool enabled) -> bool
{
  const auto& style = ImGui::GetStyle();
  const float height = ImGui::GetFontSize() + style.ItemSpacing.y * 2.0f;
  const float width = height;
  return ui_button(icon, tooltip, enabled, width, height);
}

auto ui_centered_button(const char* text, const char* tooltip) -> bool
{
  TACTILE_ASSERT(text);

  const auto width = standard_component_width(text);
  center_next_item_horizontally(width);

  return ui_button(text, tooltip);
}

auto ui_checkbox(const char* label, bool* value, const char* tooltip) -> bool
{
  TACTILE_ASSERT(label);
  TACTILE_ASSERT(value);
  const auto changed = ImGui::Checkbox(label, value);

  if (tooltip) {
    ui_lazy_tooltip(label, tooltip);
  }

  return changed;
}

void ui_lazy_tooltip(const char* id, const char* tooltip)
{
  TACTILE_ASSERT(id);
  TACTILE_ASSERT(tooltip);

  static HashMap<ImGuiID, Maybe<TimePoint>> state;

  const auto hashed_id = ImGui::GetID(id);
  auto& last_hover = state[hashed_id];

  if (ImGui::IsItemActive() || ImGui::IsItemHovered()) {
    if (!last_hover) {
      last_hover = Clock::now();
    }

    using namespace std::chrono_literals;
    if (Clock::now() - last_hover.value() > 1s) {
      ImGui::SetTooltip("%s", tooltip);
    }
  }
  else {
    last_hover.reset();
  }
}

void ui_centered_label(const char* text)
{
  TACTILE_ASSERT(text);
  const auto text_size = ImGui::CalcTextSize(text);

  const auto region = ImGui::GetContentRegionAvail();
  const auto half_region = region * ImVec2 {0.5f, 0.5f};

  const auto cursor_pos = ImGui::GetCursorPos();
  ImGui::SetCursorPos(cursor_pos + ImVec2 {half_region.x - (text_size.x * 0.5f), 0});
  ImGui::TextUnformatted(text);
}

void ui_menu_item(const MenuAction action, const char* shortcut)
{
  const auto& item = get_menu_item(action);
  if (ImGui::MenuItem(item.text.c_str(), shortcut, item.selected, item.enabled)) {
    get_dispatcher().enqueue<MenuItemEvent>(action);
  }
}

}  // namespace tactile::ui
