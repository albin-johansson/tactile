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

constexpr auto gWindowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
constinit bool gHasFocus = false;
constinit bool gMouseWithinWindow = false;

void RemoveTabBarFromNextWindow()
{
  ImGuiWindowClass wc{};
  wc.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
  ImGui::SetNextWindowClass(&wc);
}

}  // namespace

void UpdateViewportWidget(const DocumentModel& model,
                          const icon_manager& icons,
                          entt::dispatcher& dispatcher)
{
  scoped::StyleVar padding{ImGuiStyleVar_WindowPadding, {4, 4}};
  RemoveTabBarFromNextWindow();

  scoped::Window window{"Viewport", gWindowFlags};
  if (window.is_open()) {
    padding.pop();
    gHasFocus = ImGui::IsWindowFocused();
    gMouseWithinWindow = scoped::Window::current_window_contains_mouse();

    if (model.has_active_document()) {
      UpdateDocumentTabs(model, dispatcher);

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
    gHasFocus = false;
    gMouseWithinWindow = false;
  }
}

void CenterViewport()
{
  CenterMapViewport();
}

auto IsViewportFocused() noexcept -> bool
{
  return gHasFocus;
}

auto IsMouseWithinViewport() noexcept -> bool
{
  return gMouseWithinWindow;
}

}  // namespace tactile