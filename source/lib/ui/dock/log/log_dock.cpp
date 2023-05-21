/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "log_dock.hpp"

#include <imgui.h>

#include "common/debug/logging.hpp"
#include "common/type/hash_map.hpp"
#include "common/util/assoc.hpp"
#include "model/context.hpp"
#include "model/event/setting_events.hpp"
#include "systems/language_system.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

const HashMap<LogLevel, ImVec4> kLogLevelColors = {
    {LogLevel::trace, ImVec4 {0.93f, 0.51f, 0.93f, 1.00f}},
    {LogLevel::debug, ImVec4 {0.50f, 1.00f, 0.70f, 1.00f}},
    {LogLevel::info, ImVec4 {1.00f, 1.00f, 1.00f, 1.00f}},
    {LogLevel::warn, ImVec4 {1.00f, 1.00f, 0.00f, 1.00f}},
    {LogLevel::err, ImVec4 {1.00f, 0.40f, 0.40f, 1.00f}},
    {LogLevel::critical, ImVec4 {1.00f, 0.00f, 0.00f, 1.00f}},
};

struct LogDockState final {
  LogFilter log_filter;
  bool has_focus {};
};

inline LogDockState gDockState;

void _push_message_filter_checkboxes(const Strings& strings)
{
  {
    bool value = gDockState.log_filter.trace;
    ImGui::Checkbox(strings.misc.log_trace_filter.c_str(), &value);
    gDockState.log_filter.trace = value;
  }

  ImGui::SameLine();

  {
    bool value = gDockState.log_filter.debug;
    ImGui::Checkbox(strings.misc.log_debug_filter.c_str(), &value);
    gDockState.log_filter.debug = value;
  }

  ImGui::SameLine();

  {
    bool value = gDockState.log_filter.info;
    ImGui::Checkbox(strings.misc.log_info_filter.c_str(), &value);
    gDockState.log_filter.info = value;
  }

  ImGui::SameLine();

  {
    bool value = gDockState.log_filter.warn;
    ImGui::Checkbox(strings.misc.log_warn_filter.c_str(), &value);
    gDockState.log_filter.warn = value;
  }

  ImGui::SameLine();

  {
    bool value = gDockState.log_filter.error;
    ImGui::Checkbox(strings.misc.log_error_filter.c_str(), &value);
    gDockState.log_filter.error = value;
  }

  ImGui::SameLine();

  {
    bool value = gDockState.log_filter.critical;
    ImGui::Checkbox(strings.misc.log_critical_filter.c_str(), &value);
    gDockState.log_filter.critical = value;
  }
}

void _push_logged_message_legend_overlay(const Strings& strings)
{
  constexpr float overlay_opacity = 0.35f;
  constexpr ImGuiWindowFlags overlay_window_flags =
      ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
      ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
      ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
      ImGuiWindowFlags_NoMove;

  const auto& style = ImGui::GetStyle();
  const auto window_pos = ImGui::GetWindowPos();
  const auto window_size = ImGui::GetWindowSize();

  const ImVec2 overlay_pos {window_pos.x + window_size.x - style.ScrollbarSize - 6,
                            window_pos.y + 6};
  const ImVec2 overlay_pivot {1.0f, 0.0f};

  ImGui::SetNextWindowPos(overlay_pos, ImGuiCond_Always, overlay_pivot);
  ImGui::SetNextWindowBgAlpha(overlay_opacity);

  if (const Window overlay {"##LegendOverlay", overlay_window_flags}; overlay.is_open()) {
    ImGui::TextColored(lookup_in(kLogLevelColors, LogLevel::trace),
                       "%s",
                       strings.misc.log_trace_filter.c_str());
    ImGui::TextColored(lookup_in(kLogLevelColors, LogLevel::debug),
                       "%s",
                       strings.misc.log_debug_filter.c_str());
    ImGui::TextColored(lookup_in(kLogLevelColors, LogLevel::info),
                       "%s",
                       strings.misc.log_info_filter.c_str());
    ImGui::TextColored(lookup_in(kLogLevelColors, LogLevel::warn),
                       "%s",
                       strings.misc.log_warn_filter.c_str());
    ImGui::TextColored(lookup_in(kLogLevelColors, LogLevel::err),
                       "%s",
                       strings.misc.log_error_filter.c_str());
    ImGui::TextColored(lookup_in(kLogLevelColors, LogLevel::critical),
                       "%s",
                       strings.misc.log_critical_filter.c_str());
  }
}

void _push_logged_message_view(const Strings& strings, const usize message_count)
{
  const StyleColor child_bg {ImGuiCol_ChildBg, {0.1f, 0.1f, 0.1f, 0.75f}};

  const auto child_flags = ImGuiWindowFlags_AlwaysVerticalScrollbar |
                           ImGuiWindowFlags_HorizontalScrollbar |
                           ImGuiWindowFlags_AlwaysAutoResize;

  if (const Child pane {"##LogPane", {}, true, child_flags}; pane.is_open()) {
    ImGuiListClipper clipper;
    clipper.Begin(static_cast<int>(message_count));

    while (clipper.Step()) {
      visit_logged_message_range(gDockState.log_filter,
                                 static_cast<usize>(clipper.DisplayStart),
                                 static_cast<usize>(clipper.DisplayEnd),
                                 [](const LogLevel level, const String& msg) {
                                   const auto& color = lookup_in(kLogLevelColors, level);
                                   ImGui::TextColored(color, "%s", msg.c_str());
                                 });
    }

    // Makes the log follow new messages, unless the user explicitly scrolls up
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
      ImGui::SetScrollHereY(1.0f);
    }

    _push_logged_message_legend_overlay(strings);
  }
}

}  // namespace

void show_log_dock(const Model& model, Entity, Dispatcher& dispatcher)
{
  const auto& strings = sys::get_current_language_strings(model);
  const auto& settings = model.get<Settings>();

  if (!settings.test_flag(SETTINGS_SHOW_LOG_DOCK_BIT)) {
    return;
  }

  bool show_log_dock = true;
  const Window dock {strings.window.log_dock.c_str(),
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar,
                     &show_log_dock};

  if (show_log_dock != settings.test_flag(SETTINGS_SHOW_LOG_DOCK_BIT)) {
    dispatcher.enqueue<SetSettingFlagEvent>(SETTINGS_SHOW_LOG_DOCK_BIT, show_log_dock);
  }

  gDockState.has_focus = dock.has_focus(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock.is_open()) {
    _push_message_filter_checkboxes(strings);

    const auto message_count = count_matching_log_entries(gDockState.log_filter);
    if (message_count != 0u) {
      _push_logged_message_view(strings, message_count);
    }
    else {
      ui_centered_label(strings.misc.log_no_messages_match_filter.c_str());
    }

    if (auto popup = Popup::for_window("##LogDockPopup"); popup.is_open()) {
      if (ImGui::MenuItem(strings.action.clear_log.c_str())) {
        clear_log_history();
      }
    }
  }
}

auto is_log_dock_focused() -> bool
{
  return gDockState.has_focus;
}

}  // namespace tactile::ui
