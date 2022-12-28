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
#include "core/type/maybe.hpp"
#include "io/proto/preferences.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "ui/constants.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

constexpr auto window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
constexpr auto child_flags = ImGuiWindowFlags_AlwaysVerticalScrollbar |
                             ImGuiWindowFlags_HorizontalScrollbar |
                             ImGuiWindowFlags_AlwaysAutoResize;

constinit LogFilter log_filter;
constinit bool is_dock_focused = false;

[[nodiscard]] constexpr auto color_for_level(const LogLevel level) -> ImVec4
{
  switch (level) {
    case LogLevel::trace:
      return {0.93f, 0.51f, 0.93f, 1.0f};

    case LogLevel::debug:
      return {0.5f, 1.0f, 0.7f, 1.0f};

    case LogLevel::info:
      return {1.0f, 1.0f, 1.0f, 1.00f};

    case LogLevel::warn:
      return {1.0f, 1.0f, 0.00f, 1.00f};

    case LogLevel::err:
      return {1.00f, 0.40f, 0.40f, 1.00f};

    case LogLevel::critical:
      return {1.00f, 0.00f, 0.00f, 1.00f};

    default:
      throw TactileError {"Did not recognize log level!"};
  }
}

void ui_message_filter_selector()
{
  const auto& lang = get_current_language();

  if (ImGui::Button(lang.misc.filter.c_str())) {
    ImGui::OpenPopup("##FilterPopup");
  }

  if (const Popup popup {"##FilterPopup"}; popup.is_open()) {
    if (ImGui::MenuItem(lang.misc.log_trace_filter.c_str(), nullptr, log_filter.trace)) {
      log_filter.trace = !log_filter.trace;
    }

    if (ImGui::MenuItem(lang.misc.log_debug_filter.c_str(), nullptr, log_filter.debug)) {
      log_filter.debug = !log_filter.debug;
    }

    if (ImGui::MenuItem(lang.misc.log_info_filter.c_str(), nullptr, log_filter.info)) {
      log_filter.info = !log_filter.info;
    }

    if (ImGui::MenuItem(lang.misc.log_warn_filter.c_str(), nullptr, log_filter.warn)) {
      log_filter.warn = !log_filter.warn;
    }

    if (ImGui::MenuItem(lang.misc.log_error_filter.c_str(), nullptr, log_filter.error)) {
      log_filter.error = !log_filter.error;
    }

    if (ImGui::MenuItem(lang.misc.log_critical_filter.c_str(),
                        nullptr,
                        log_filter.critical)) {
      log_filter.critical = !log_filter.critical;
    }
  }
}

void ui_logged_message_color_legend()
{
  ImGui::TextDisabled("(?)");

  if (ImGui::IsItemHovered()) {
    const StyleColor bg {ImGuiCol_PopupBg, {0.1f, 0.1f, 0.1f, 0.75f}};
    const Tooltip tooltip;

    constexpr auto verbose_color = color_for_level(LogLevel::trace);
    constexpr auto debug_color = color_for_level(LogLevel::debug);
    constexpr auto info_color = color_for_level(LogLevel::info);
    constexpr auto warning_color = color_for_level(LogLevel::warn);
    constexpr auto error_color = color_for_level(LogLevel::err);
    constexpr auto critical_color = color_for_level(LogLevel::critical);

    const auto& lang = get_current_language();
    ImGui::TextColored(verbose_color, "%s", lang.tooltip.log_verbose_msg.c_str());
    ImGui::TextColored(debug_color, "%s", lang.tooltip.log_debug_msg.c_str());
    ImGui::TextColored(info_color, "%s", lang.tooltip.log_info_msg.c_str());
    ImGui::TextColored(warning_color, "%s", lang.tooltip.log_warn_msg.c_str());
    ImGui::TextColored(error_color, "%s", lang.tooltip.log_error_msg.c_str());
    ImGui::TextColored(critical_color, "%s", lang.tooltip.log_critical_msg.c_str());
  }
}

void ui_logged_message_view(const usize message_count)
{
  const StyleColor child_bg {ImGuiCol_ChildBg, {0.1f, 0.1f, 0.1f, 0.75f}};

  if (const Child pane {"##LogPane", {}, true, child_flags}; pane.is_open()) {
    ImGuiListClipper clipper;
    clipper.Begin(static_cast<int>(message_count));

    while (clipper.Step()) {
      visit_logged_message_range(log_filter,
                                 clipper.DisplayStart,
                                 clipper.DisplayEnd,
                                 [](const LogLevel level, const String& msg) {
                                   const auto color = color_for_level(level);
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

  const Window dock {lang.window.log_dock.c_str(), window_flags, &prefs.show_log_dock};
  is_dock_focused = dock.has_focus(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock.is_open()) {
    ui_message_filter_selector();


    ImGui::SameLine();
    ui_logged_message_color_legend();

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
