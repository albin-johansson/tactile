// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "widgets.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "model/event/menu_events.hpp"
#include "tactile/base/container/hash_map.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/util/chrono.hpp"
#include "tactile/core/debug/assert.hpp"
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

  static HashMap<ImGuiID, Maybe<SystemClockInstant>> state;

  const auto hashed_id = ImGui::GetID(id);
  auto& last_hover = state[hashed_id];

  if (ImGui::IsItemActive() || ImGui::IsItemHovered()) {
    if (!last_hover) {
      last_hover = SystemClock::now();
    }

    using namespace std::chrono_literals;
    if (SystemClock::now() - last_hover.value() > 1s) {
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

void ui_menu_item(entt::dispatcher& dispatcher,
                  const MenuAction action,
                  const char* shortcut)
{
  const auto& item = get_menu_item(action);
  if (ImGui::MenuItem(item.text.c_str(), shortcut, item.selected, item.enabled)) {
    dispatcher.enqueue<MenuItemEvent>(action);
  }
}

}  // namespace tactile::ui
