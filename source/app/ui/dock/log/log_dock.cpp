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

#include "log_dock.hpp"

#include <imgui.h>

#include "core/debug/logging.hpp"
#include "core/debug/panic.hpp"
#include "core/type/hash_map.hpp"
#include "core/util/assoc.hpp"
#include "io/proto/preferences.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

constinit LogFilter log_filter;
constinit bool is_dock_focused = false;

const HashMap<LogLevel, ImVec4> log_level_colors = {
    {LogLevel::trace, ImVec4 {0.93f, 0.51f, 0.93f, 1.00f}},
    {LogLevel::debug, ImVec4 {0.50f, 1.00f, 0.70f, 1.00f}},
    {LogLevel::info, ImVec4 {1.00f, 1.00f, 1.00f, 1.00f}},
    {LogLevel::warn, ImVec4 {1.00f, 1.00f, 0.00f, 1.00f}},
    {LogLevel::err, ImVec4 {1.00f, 0.40f, 0.40f, 1.00f}},
    {LogLevel::critical, ImVec4 {1.00f, 0.00f, 0.00f, 1.00f}},
};

void ui_message_filter_selector()
{
  const auto& lang = get_current_language();

  if (ImGui::Button(lang.misc.filter.c_str())) {
    ImGui::OpenPopup("##FilterPopup");
  }

  if (const Popup popup {"##FilterPopup"}; popup.is_open()) {
    if (const StyleColor text {ImGuiCol_Text,
                               lookup_in(log_level_colors, LogLevel::trace)};
        ImGui::MenuItem(lang.misc.log_trace_filter.c_str(), nullptr, log_filter.trace)) {
      log_filter.trace = !log_filter.trace;
    }

    if (const StyleColor text {ImGuiCol_Text,
                               lookup_in(log_level_colors, LogLevel::debug)};
        ImGui::MenuItem(lang.misc.log_debug_filter.c_str(), nullptr, log_filter.debug)) {
      log_filter.debug = !log_filter.debug;
    }

    if (const StyleColor text {ImGuiCol_Text,
                               lookup_in(log_level_colors, LogLevel::info)};
        ImGui::MenuItem(lang.misc.log_info_filter.c_str(), nullptr, log_filter.info)) {
      log_filter.info = !log_filter.info;
    }

    if (const StyleColor text {ImGuiCol_Text,
                               lookup_in(log_level_colors, LogLevel::warn)};
        ImGui::MenuItem(lang.misc.log_warn_filter.c_str(), nullptr, log_filter.warn)) {
      log_filter.warn = !log_filter.warn;
    }

    if (const StyleColor text {ImGuiCol_Text, lookup_in(log_level_colors, LogLevel::err)};
        ImGui::MenuItem(lang.misc.log_error_filter.c_str(), nullptr, log_filter.error)) {
      log_filter.error = !log_filter.error;
    }

    if (const StyleColor text {ImGuiCol_Text,
                               lookup_in(log_level_colors, LogLevel::critical)};
        ImGui::MenuItem(lang.misc.log_critical_filter.c_str(),
                        nullptr,
                        log_filter.critical)) {
      log_filter.critical = !log_filter.critical;
    }
  }
}

void ui_logged_message_view(const usize message_count)
{
  const StyleColor child_bg {ImGuiCol_ChildBg, {0.1f, 0.1f, 0.1f, 0.75f}};

  constexpr auto child_flags = ImGuiWindowFlags_AlwaysVerticalScrollbar |
                               ImGuiWindowFlags_HorizontalScrollbar |
                               ImGuiWindowFlags_AlwaysAutoResize;

  if (const Child pane {"##LogPane", {}, true, child_flags}; pane.is_open()) {
    ImGuiListClipper clipper;
    clipper.Begin(static_cast<int>(message_count));

    while (clipper.Step()) {
      visit_logged_message_range(log_filter,
                                 clipper.DisplayStart,
                                 clipper.DisplayEnd,
                                 [](const LogLevel level, const String& msg) {
                                   const auto& color = lookup_in(log_level_colors, level);
                                   ImGui::TextColored(color, "%s", msg.c_str());
                                 });
    }

    // Makes the log follow new messages, unless the user explicitly scrolls up
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
      ImGui::SetScrollHereY(1.0f);
    }
  }
}

}  // namespace

void update_log_dock()
{
  auto& prefs = io::get_preferences();

  if (!prefs.show_log_dock) {
    return;
  }

  const auto& lang = get_current_language();

  const Window dock {lang.window.log_dock.c_str(),
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar,
                     &prefs.show_log_dock};
  is_dock_focused = dock.has_focus(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock.is_open()) {
    ui_message_filter_selector();

    const auto message_count = count_matching_log_entries(log_filter);
    if (message_count != 0u) {
      ui_logged_message_view(message_count);
    }
    else {
      ui_centered_label(lang.misc.log_no_messages_match_filter.c_str());
    }

    if (auto popup = Popup::for_window("##LogDockPopup"); popup.is_open()) {
      if (ImGui::MenuItem(lang.action.clear_log.c_str())) {
        clear_log_history();
      }
    }
  }
}

auto is_log_dock_focused() -> bool
{
  return is_dock_focused;
}

}  // namespace tactile::ui
