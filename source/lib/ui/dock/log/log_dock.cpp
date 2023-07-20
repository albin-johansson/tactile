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

#include "common/type/hash_map.hpp"
#include "common/util/lookup.hpp"
#include "model/events/setting_events.hpp"
#include "model/i18n/language_system.hpp"
#include "model/services/service_locator.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile {
namespace {

inline constexpr auto kLogDockContextMenuID = "##LogDockWindowPopup";

const HashMap<LogLevelBits, ImVec4> kLogLevelColors = {
    {LOG_LEVEL_TRACE, ImVec4 {0.93f, 0.51f, 0.93f, 1.00f}},
    {LOG_LEVEL_DEBUG, ImVec4 {0.50f, 1.00f, 0.70f, 1.00f}},
    {LOG_LEVEL_INFO, ImVec4 {1.00f, 1.00f, 1.00f, 1.00f}},
    {LOG_LEVEL_WARNING, ImVec4 {1.00f, 1.00f, 0.00f, 1.00f}},
    {LOG_LEVEL_ERROR, ImVec4 {1.00f, 0.40f, 0.40f, 1.00f}},
    {LOG_LEVEL_CRITICAL, ImVec4 {1.00f, 0.00f, 0.00f, 1.00f}},
};

void _push_filter_options(const Strings& strings, LogDockState& state)
{
  bool include_trace = state.log_filter & LOG_LEVEL_TRACE;
  bool include_debug = state.log_filter & LOG_LEVEL_DEBUG;
  bool include_info = state.log_filter & LOG_LEVEL_INFO;
  bool include_warning = state.log_filter & LOG_LEVEL_WARNING;
  bool include_error = state.log_filter & LOG_LEVEL_ERROR;
  bool include_critical = state.log_filter & LOG_LEVEL_CRITICAL;

  if (const ui::Menu filter_menu {strings.misc.filter.c_str()}; filter_menu.is_open()) {
    ImGui::MenuItem(strings.misc.log_trace_filter.c_str(), nullptr, &include_trace);
    ImGui::MenuItem(strings.misc.log_debug_filter.c_str(), nullptr, &include_debug);
    ImGui::MenuItem(strings.misc.log_info_filter.c_str(), nullptr, &include_info);
    ImGui::MenuItem(strings.misc.log_warn_filter.c_str(), nullptr, &include_warning);
    ImGui::MenuItem(strings.misc.log_error_filter.c_str(), nullptr, &include_error);
    ImGui::MenuItem(strings.misc.log_critical_filter.c_str(), nullptr, &include_critical);
  }

  state.log_filter = 0;

  if (include_trace) {
    state.log_filter |= LOG_LEVEL_TRACE;
  }

  if (include_debug) {
    state.log_filter |= LOG_LEVEL_DEBUG;
  }

  if (include_info) {
    state.log_filter |= LOG_LEVEL_INFO;
  }

  if (include_warning) {
    state.log_filter |= LOG_LEVEL_WARNING;
  }

  if (include_error) {
    state.log_filter |= LOG_LEVEL_ERROR;
  }

  if (include_critical) {
    state.log_filter |= LOG_LEVEL_CRITICAL;
  }
}

void _push_dock_context_menu(const Strings& strings, LogDockState& state)
{
  if (const auto popup = ui::Popup::for_window(kLogDockContextMenuID); popup.is_open()) {
    _push_filter_options(strings, state);

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.clear_log.c_str())) {
      auto& logging_service = ServiceLocator<LoggingService>::get();
      logging_service.clear_history();  // TODO event
    }
  }
}

void _push_logged_message_legend_overlay(const Strings& strings)
{
  const float overlay_opacity = 0.35f;
  const ImGuiWindowFlags overlay_window_flags =
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

  if (const ui::Window overlay {"##LegendOverlay", overlay_window_flags};
      overlay.is_open()) {
    ImGui::TextColored(lookup_in(kLogLevelColors, LOG_LEVEL_TRACE),
                       "%s",
                       strings.misc.log_trace_filter.c_str());
    ImGui::TextColored(lookup_in(kLogLevelColors, LOG_LEVEL_DEBUG),
                       "%s",
                       strings.misc.log_debug_filter.c_str());
    ImGui::TextColored(lookup_in(kLogLevelColors, LOG_LEVEL_INFO),
                       "%s",
                       strings.misc.log_info_filter.c_str());
    ImGui::TextColored(lookup_in(kLogLevelColors, LOG_LEVEL_WARNING),
                       "%s",
                       strings.misc.log_warn_filter.c_str());
    ImGui::TextColored(lookup_in(kLogLevelColors, LOG_LEVEL_ERROR),
                       "%s",
                       strings.misc.log_error_filter.c_str());
    ImGui::TextColored(lookup_in(kLogLevelColors, LOG_LEVEL_CRITICAL),
                       "%s",
                       strings.misc.log_critical_filter.c_str());
  }
}

void _push_logged_message_view(const Strings& strings,
                               const usize message_count,
                               LogDockState& state)
{
  const auto& logging_service = ServiceLocator<LoggingService>::get();

  const ui::StyleColor child_bg {ImGuiCol_ChildBg, {0.1f, 0.1f, 0.1f, 0.75f}};
  const auto child_flags = ImGuiWindowFlags_AlwaysVerticalScrollbar |
                           ImGuiWindowFlags_HorizontalScrollbar |
                           ImGuiWindowFlags_AlwaysAutoResize;

  if (const ui::Child pane {"##LogPane", {}, true, child_flags}; pane.is_open()) {
    ImGuiListClipper clipper;
    clipper.Begin(static_cast<int>(message_count));

    while (clipper.Step()) {
      auto show_colored_message = [](const LogLevelBits level, StringView msg) {
        const auto& color = lookup_in(kLogLevelColors, level);
        ImGui::TextColored(color, "%s", msg.data());
      };

      logging_service.each_logged_message(static_cast<usize>(clipper.DisplayStart),
                                          static_cast<usize>(clipper.DisplayEnd),
                                          state.log_filter,
                                          show_colored_message);
    }

    // Makes the log follow new messages, unless the user explicitly scrolls up
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
      ImGui::SetScrollHereY(1.0f);
    }

    _push_logged_message_legend_overlay(strings);
  }
}

}  // namespace

void push_log_dock_widget(ModelView& model, LogDockState& state)
{
  const auto& strings = model.get_language_strings();
  const auto& settings = model.get_settings();

  if (!settings.test_flag(SETTINGS_SHOW_LOG_DOCK_BIT)) {
    return;
  }

  bool show_log_dock = true;
  const ui::Window dock {strings.window.log_dock.c_str(),
                         ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar,
                         &show_log_dock};

  if (show_log_dock != settings.test_flag(SETTINGS_SHOW_LOG_DOCK_BIT)) {
    model.enqueue<SetFlagSettingEvent>(SETTINGS_SHOW_LOG_DOCK_BIT, show_log_dock);
  }

  state.has_focus = dock.has_focus(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock.is_open()) {
    const auto& logging_service = ServiceLocator<LoggingService>::get();
    const auto message_count = logging_service.count_logged_messages(state.log_filter);

    if (message_count != 0u) {
      _push_logged_message_view(strings, message_count, state);
    }
    else {
      ui::push_centered_label(strings.misc.log_no_messages_match_filter.c_str());
    }

    if (dock.is_hovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
      ImGui::OpenPopup(kLogDockContextMenuID);
    }

    _push_dock_context_menu(strings, state);
  }
}

}  // namespace tactile
