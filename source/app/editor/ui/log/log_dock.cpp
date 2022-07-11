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

#include "core/common/maybe.hpp"
#include "editor/ui/common/labels.hpp"
#include "editor/ui/icons.hpp"
#include "editor/ui/scoped.hpp"
#include "io/persistence/preferences.hpp"
#include "misc/logging.hpp"
#include "misc/panic.hpp"

namespace tactile::ui {
namespace {

constexpr auto _window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
constexpr auto _child_flags = ImGuiWindowFlags_AlwaysVerticalScrollbar |
                              ImGuiWindowFlags_HorizontalScrollbar |
                              ImGuiWindowFlags_AlwaysAutoResize;

constinit LogLevel _log_filter = LogLevel::debug;
constinit bool     _is_dock_focused = false;

[[nodiscard]] auto _show_log_level_filter_combo(const LogLevel currentLevel)
    -> Maybe<LogLevel>
{
  static constexpr auto traceFilter = "Everything";
  static constexpr auto debugFilter = "Debug";
  static constexpr auto infoFilter = "Information";
  static constexpr auto warningFilter = "Warnings";
  static constexpr auto errorFilter = "Errors";
  static constexpr auto criticalFilter = "Critical";

  static const auto comboWidth = ImGui::CalcTextSize(infoFilter).x * 2.0f;

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted("Filter:");

  ImGui::SameLine();

  const char* filter = traceFilter;
  switch (currentLevel) {
    case LogLevel::trace:
      filter = traceFilter;
      break;

    case LogLevel::debug:
      filter = debugFilter;
      break;

    case LogLevel::info:
      filter = infoFilter;
      break;

    case LogLevel::warn:
      filter = warningFilter;
      break;

    case LogLevel::err:
      filter = errorFilter;
      break;

    case LogLevel::critical:
      filter = criticalFilter;
      break;

    default:
      break;
  }

  ImGui::SetNextItemWidth(comboWidth);
  if (Combo filterCombo {"##LogFilterCombo", filter}; filterCombo.is_open()) {
    if (ImGui::MenuItem(traceFilter)) {
      return LogLevel::trace;
    }

    if (ImGui::MenuItem(debugFilter)) {
      return LogLevel::debug;
    }

    if (ImGui::MenuItem(infoFilter)) {
      return LogLevel::info;
    }

    if (ImGui::MenuItem(warningFilter)) {
      return LogLevel::warn;
    }

    if (ImGui::MenuItem(errorFilter)) {
      return LogLevel::err;
    }

    if (ImGui::MenuItem(criticalFilter)) {
      return LogLevel::critical;
    }
  }

  return nothing;
}

[[nodiscard]] auto _color_for_level(const LogLevel level) -> ImVec4
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
      throw TactileError("Did not recognize log level!");
  }
}

void _update_color_legend_hint()
{
  ImGui::TextDisabled("(?)");

  if (ImGui::IsItemHovered()) {
    StyleColor bg {ImGuiCol_PopupBg, {0.1f, 0.1f, 0.1f, 0.75f}};
    Tooltip    tooltip;

    static const auto verboseColor = _color_for_level(LogLevel::trace);
    static const auto debugColor = _color_for_level(LogLevel::debug);
    static const auto infoColor = _color_for_level(LogLevel::info);
    static const auto warningColor = _color_for_level(LogLevel::warn);
    static const auto errorColor = _color_for_level(LogLevel::err);
    static const auto criticalColor = _color_for_level(LogLevel::critical);

    ImGui::TextColored(verboseColor, "Verbose message");
    ImGui::TextColored(debugColor, "Debug message");
    ImGui::TextColored(infoColor, "Info message");
    ImGui::TextColored(warningColor, "Warning message");
    ImGui::TextColored(errorColor, "Error message");
    ImGui::TextColored(criticalColor, "Critical message");
  }
}

void _update_log_contents(const LogLevel filter)
{
  StyleColor childBg {ImGuiCol_ChildBg, {0.1f, 0.1f, 0.1f, 0.75f}};

  if (Child pane {"##LogPane", {}, true, _child_flags}; pane.is_open()) {
    ImGuiListClipper clipper;
    clipper.Begin(static_cast<int>(log_size(filter)));

    while (clipper.Step()) {
      for (auto i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
        const auto& [level, str] = get_log_entry(filter, static_cast<usize>(i));
        const auto color = _color_for_level(level);
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

  Window dock {"Log", _window_flags, &prefs.show_log_dock};

  _is_dock_focused = dock.has_focus(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock.is_open()) {
    if (const auto level = _show_log_level_filter_combo(_log_filter)) {
      _log_filter = *level;
    }

    ImGui::SameLine();
    _update_color_legend_hint();

    if (log_size(_log_filter) != 0u) {
      _update_log_contents(_log_filter);
    }
    else {
      centered_label("No logged messages match the current filter.");
    }

    if (auto popup = Popup::for_window("##LogDockContext"); popup.is_open()) {
      if (ImGui::MenuItem(TAC_ICON_CLEAR_HISTORY " Clear Log")) {
        clear_log_history();
      }
    }
  }
}

auto is_log_dock_focused() -> bool
{
  return _is_dock_focused;
}

}  // namespace tactile::ui
