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

#include "viewport_widget.hpp"

#include <centurion/keyboard.hpp>
#include <centurion/mouse_events.hpp>
#include <entt/signal/dispatcher.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "core/components/viewport.hpp"
#include "core/events/map_events.hpp"
#include "core/events/tool_events.hpp"
#include "core/events/viewport_events.hpp"
#include "core/model.hpp"
#include "core/systems/viewport_system.hpp"
#include "editor/shortcuts/mappings.hpp"
#include "editor/ui/alignment.hpp"
#include "editor/ui/common/button.hpp"
#include "editor/ui/common/style.hpp"
#include "editor/ui/icons.hpp"
#include "editor/ui/scoped.hpp"
#include "editor/ui/textures.hpp"
#include "editor/ui/viewport/document_tab_widget.hpp"

namespace tactile::ui {
namespace {

constexpr auto _window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
constinit bool _has_focus = false;
constinit bool _mouse_within_window = false;

void _update_start_page(entt::dispatcher& dispatcher)
{
  prepare_vertical_alignment_center(4);

  ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2{0, 64});

  center_next_item_horizontally(128);
  ImGui::Image(to_texture_id(get_tactile_icon()), {128, 128});

  ImGui::Spacing();
  ImGui::Spacing();

  if (centered_button("Create new map")) {
    dispatcher.enqueue<ShowNewMapDialogEvent>();
  }

  ImGui::Spacing();
  if (centered_button("Open existing map")) {
    dispatcher.enqueue<ShowOpenMapDialogEvent>();
  }
}

}  // namespace

void update_viewport_widget(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  StyleVar padding{ImGuiStyleVar_WindowPadding, {4, 4}};
  remove_tab_bar_from_next_window();

  Window window{"Viewport", _window_flags};
  _has_focus = window.has_focus();
  _mouse_within_window = window.is_hovered();

  if (window.is_open()) {
    padding.pop();

    if (model.has_active_document()) {
      update_document_tabs(model, dispatcher);

      const auto& document = model.get_active_document();
      if (document.is_map()) {
        if (window.mouse_entered()) {
          dispatcher.enqueue<ToolEnteredEvent>();
        }

        if (window.mouse_exited()) {
          dispatcher.enqueue<ToolExitedEvent>();
        }
      }
    }
    else {
      _update_start_page(dispatcher);
    }
  }
}

void viewport_widget_mouse_wheel_event_handler(const entt::registry& registry,
                                               entt::dispatcher& dispatcher,
                                               const cen::mouse_wheel_event& event)
{
  constexpr float scaling = 4.0f;

  const auto& viewport = ctx_get<comp::Viewport>(registry);
  if (cen::is_active(primary_modifier)) {
    const auto y = event.precise_y();
    if (y > 0) {
      dispatcher.enqueue<IncreaseZoomEvent>();
    }
    else if (y < 0 && sys::can_decrease_viewport_zoom(registry)) {
      dispatcher.enqueue<DecreaseZoomEvent>();
    }
  }
  else {
    const auto dx = event.precise_x() * (viewport.tile_size.x / scaling);
    const auto dy = event.precise_y() * (viewport.tile_size.y / scaling);
    dispatcher.enqueue<OffsetViewportEvent>(entt::null, -dx, dy);
  }
}

auto is_viewport_focused() noexcept -> bool
{
  return _has_focus;
}

auto is_mouse_within_viewport() noexcept -> bool
{
  return _mouse_within_window;
}

}  // namespace tactile::ui