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

#include <imgui.h>
#include <imgui_internal.h>

#include "document_tabs.hpp"
#include "editor/events/tool_events.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"
#include "home_page_content.hpp"

namespace tactile {
namespace {

constexpr auto _window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
constinit bool _has_focus = false;
constinit bool _mouse_within_window = false;

void _remove_tab_bar_from_next_window()
{
  ImGuiWindowClass wc{};
  wc.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
  ImGui::SetNextWindowClass(&wc);
}

}  // namespace

void update_viewport_widget(const DocumentModel& model,
                            const IconManager& icons,
                            entt::dispatcher& dispatcher)
{
  scoped::StyleVar padding{ImGuiStyleVar_WindowPadding, {4, 4}};
  _remove_tab_bar_from_next_window();

  scoped::Window window{"Viewport", _window_flags};
  if (window.is_open()) {
    padding.pop();
    _has_focus = ImGui::IsWindowFocused();
    _mouse_within_window = scoped::Window::current_window_contains_mouse();

    if (model.has_active_document()) {
      update_document_tabs(model, dispatcher);

      if (window.mouse_entered()) {
        dispatcher.enqueue<ToolEnteredEvent>();
      }

      if (window.mouse_exited()) {
        dispatcher.enqueue<ToolExitedEvent>();
      }
    }
    else {
      UpdateHomePageContent(icons, dispatcher);
    }
  }
  else {
    _has_focus = false;
    _mouse_within_window = false;
  }
}

void center_viewport()
{
  center_map_viewport();
}

auto is_viewport_focused() noexcept -> bool
{
  return _has_focus;
}

auto is_mouse_within_viewport() noexcept -> bool
{
  return _mouse_within_window;
}

}  // namespace tactile