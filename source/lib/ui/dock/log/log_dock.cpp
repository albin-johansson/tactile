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

#include "common/type/hash_map.hpp"
#include "common/util/assoc.hpp"
#include "core/debug/logging.hpp"
#include "core/debug/panic.hpp"
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

void ui_message_filter_checkboxes()
{
  const auto& lang = get_current_language();

  {
    bool value = log_filter.trace;
    ImGui::Checkbox(lang.misc.log_trace_filter.c_str(), &value);
    log_filter.trace = value;
  }

  ImGui::SameLine();

  {
    bool value = log_filter.debug;
    ImGui::Checkbox(lang.misc.log_debug_filter.c_str(), &value);
    log_filter.debug = value;
  }

  ImGui::SameLine();

  {
    bool value = log_filter.info;
    ImGui::Checkbox(lang.misc.log_info_filter.c_str(), &value);
    log_filter.info = value;
  }

  ImGui::SameLine();

  {
    bool value = log_filter.warn;
    ImGui::Checkbox(lang.misc.log_warn_filter.c_str(), &value);
    log_filter.warn = value;
  }

  ImGui::SameLine();

  {
    bool value = log_filter.error;
    ImGui::Checkbox(lang.misc.log_error_filter.c_str(), &value);
    log_filter.error = value;
  }

  ImGui::SameLine();

  {
    bool value = log_filter.critical;
    ImGui::Checkbox(lang.misc.log_critical_filter.c_str(), &value);
    log_filter.critical = value;
  }
}

void ui_logged_message_legend_overlay(const Strings& lang)
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
    ImGui::TextColored(lookup_in(log_level_colors, LogLevel::trace),
                       "%s",
                       lang.misc.log_trace_filter.c_str());
    ImGui::TextColored(lookup_in(log_level_colors, LogLevel::debug),
                       "%s",
                       lang.misc.log_debug_filter.c_str());
    ImGui::TextColored(lookup_in(log_level_colors, LogLevel::info),
                       "%s",
                       lang.misc.log_info_filter.c_str());
    ImGui::TextColored(lookup_in(log_level_colors, LogLevel::warn),
                       "%s",
                       lang.misc.log_warn_filter.c_str());
    ImGui::TextColored(lookup_in(log_level_colors, LogLevel::err),
                       "%s",
                       lang.misc.log_error_filter.c_str());
    ImGui::TextColored(lookup_in(log_level_colors, LogLevel::critical),
                       "%s",
                       lang.misc.log_critical_filter.c_str());
  }
}

void ui_logged_message_view(const Strings& lang, const usize message_count)
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
                                 static_cast<usize>(clipper.DisplayStart),
                                 static_cast<usize>(clipper.DisplayEnd),
                                 [](const LogLevel level, const String& msg) {
                                   const auto& color = lookup_in(log_level_colors, level);
                                   ImGui::TextColored(color, "%s", msg.c_str());
                                 });
    }

    // Makes the log follow new messages, unless the user explicitly scrolls up
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
      ImGui::SetScrollHereY(1.0f);
    }

    ui_logged_message_legend_overlay(lang);
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
    ui_message_filter_checkboxes();

    const auto message_count = count_matching_log_entries(log_filter);
    if (message_count != 0u) {
      ui_logged_message_view(lang, message_count);
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
