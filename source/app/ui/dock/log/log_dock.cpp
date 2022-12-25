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

#include "core/type/maybe.hpp"
#include "core/debug/logging.hpp"
#include "core/debug/panic.hpp"
#include "io/proto/preferences.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

constexpr auto window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
constexpr auto child_flags = ImGuiWindowFlags_AlwaysVerticalScrollbar |
                             ImGuiWindowFlags_HorizontalScrollbar |
                             ImGuiWindowFlags_AlwaysAutoResize;

constinit LogLevel log_filter = LogLevel::debug;
constinit bool is_dock_focused = false;

[[nodiscard]] auto show_log_level_filter_combo(const LogLevel current_level)
    -> Maybe<LogLevel>
{
  const auto& lang = get_current_language();

  static const auto combo_width =
      ImGui::CalcTextSize(lang.misc.log_info_filter.c_str()).x * 2.0f;

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(lang.misc.filter.c_str());

  ImGui::SameLine();

  const char* filter = lang.misc.log_trace_filter.c_str();
  switch (current_level) {
    case LogLevel::trace:
      filter = lang.misc.log_trace_filter.c_str();
      break;

    case LogLevel::debug:
      filter = lang.misc.log_debug_filter.c_str();
      break;

    case LogLevel::info:
      filter = lang.misc.log_info_filter.c_str();
      break;

    case LogLevel::warn:
      filter = lang.misc.log_warn_filter.c_str();
      break;

    case LogLevel::err:
      filter = lang.misc.log_error_filter.c_str();
      break;

    case LogLevel::critical:
      filter = lang.misc.log_critical_filter.c_str();
      break;

    default:
      break;
  }

  ImGui::SetNextItemWidth(combo_width);
  if (Combo filter_combo {"##LogFilterCombo", filter}; filter_combo.is_open()) {
    if (ImGui::MenuItem(lang.misc.log_trace_filter.c_str())) {
      return LogLevel::trace;
    }

    if (ImGui::MenuItem(lang.misc.log_debug_filter.c_str())) {
      return LogLevel::debug;
    }

    if (ImGui::MenuItem(lang.misc.log_info_filter.c_str())) {
      return LogLevel::info;
    }

    if (ImGui::MenuItem(lang.misc.log_warn_filter.c_str())) {
      return LogLevel::warn;
    }

    if (ImGui::MenuItem(lang.misc.log_error_filter.c_str())) {
      return LogLevel::err;
    }

    if (ImGui::MenuItem(lang.misc.log_critical_filter.c_str())) {
      return LogLevel::critical;
    }
  }

  return nothing;
}

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

void update_color_legend_hint()
{
  ImGui::TextDisabled("(?)");

  if (ImGui::IsItemHovered()) {
    StyleColor bg {ImGuiCol_PopupBg, {0.1f, 0.1f, 0.1f, 0.75f}};
    Tooltip tooltip;

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

void update_log_contents(const LogLevel filter)
{
  StyleColor child_bg {ImGuiCol_ChildBg, {0.1f, 0.1f, 0.1f, 0.75f}};

  if (Child pane {"##LogPane", {}, true, child_flags}; pane.is_open()) {
    ImGuiListClipper clipper;
    clipper.Begin(static_cast<int>(log_size(filter)));

    while (clipper.Step()) {
      for (auto i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
        const auto& [level, str] = get_log_entry(filter, static_cast<usize>(i));
        const auto color = color_for_level(level);
        ImGui::TextColored(color, "%s", str.c_str());
      }
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

  Window dock {lang.window.log_dock.c_str(), window_flags, &prefs.show_log_dock};
  is_dock_focused = dock.has_focus(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock.is_open()) {
    if (const auto level = show_log_level_filter_combo(log_filter)) {
      log_filter = *level;
    }

    ImGui::SameLine();
    update_color_legend_hint();

    if (log_size(log_filter) != 0u) {
      update_log_contents(log_filter);
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
