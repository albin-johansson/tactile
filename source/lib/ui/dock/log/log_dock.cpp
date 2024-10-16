// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "log_dock.hpp"

#include <imgui.h>

#include "common/debug/logging.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/settings.hpp"
#include "tactile/base/container/hash_map.hpp"
#include "tactile/core/debug/exception.hpp"
#include "tactile/core/util/lookup.hpp"
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

void ui_message_filter_checkboxes()
{
  const auto& lang = get_current_language();

  {
    bool value = gDockState.log_filter.trace;
    ImGui::Checkbox(lang.misc.log_trace_filter.c_str(), &value);
    gDockState.log_filter.trace = value;
  }

  ImGui::SameLine();

  {
    bool value = gDockState.log_filter.debug;
    ImGui::Checkbox(lang.misc.log_debug_filter.c_str(), &value);
    gDockState.log_filter.debug = value;
  }

  ImGui::SameLine();

  {
    bool value = gDockState.log_filter.info;
    ImGui::Checkbox(lang.misc.log_info_filter.c_str(), &value);
    gDockState.log_filter.info = value;
  }

  ImGui::SameLine();

  {
    bool value = gDockState.log_filter.warn;
    ImGui::Checkbox(lang.misc.log_warn_filter.c_str(), &value);
    gDockState.log_filter.warn = value;
  }

  ImGui::SameLine();

  {
    bool value = gDockState.log_filter.error;
    ImGui::Checkbox(lang.misc.log_error_filter.c_str(), &value);
    gDockState.log_filter.error = value;
  }

  ImGui::SameLine();

  {
    bool value = gDockState.log_filter.critical;
    ImGui::Checkbox(lang.misc.log_critical_filter.c_str(), &value);
    gDockState.log_filter.critical = value;
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
    ImGui::TextColored(lookup_in(kLogLevelColors, LogLevel::trace),
                       "%s",
                       lang.misc.log_trace_filter.c_str());
    ImGui::TextColored(lookup_in(kLogLevelColors, LogLevel::debug),
                       "%s",
                       lang.misc.log_debug_filter.c_str());
    ImGui::TextColored(lookup_in(kLogLevelColors, LogLevel::info),
                       "%s",
                       lang.misc.log_info_filter.c_str());
    ImGui::TextColored(lookup_in(kLogLevelColors, LogLevel::warn),
                       "%s",
                       lang.misc.log_warn_filter.c_str());
    ImGui::TextColored(lookup_in(kLogLevelColors, LogLevel::err),
                       "%s",
                       lang.misc.log_error_filter.c_str());
    ImGui::TextColored(lookup_in(kLogLevelColors, LogLevel::critical),
                       "%s",
                       lang.misc.log_critical_filter.c_str());
  }
}

void ui_logged_message_view(const Strings& lang, const usize message_count)
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

    ui_logged_message_legend_overlay(lang);
  }
}

}  // namespace

void update_log_dock()
{
  auto& settings = get_settings();

  if (!settings.test_flag(SETTINGS_SHOW_LOG_DOCK_BIT)) {
    return;
  }

  const auto& lang = get_current_language();

  bool show_log_dock = true;
  const Window dock {lang.window.log_dock.c_str(),
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar,
                     &show_log_dock};
  settings.set_flag(SETTINGS_SHOW_LOG_DOCK_BIT, show_log_dock);

  gDockState.has_focus = dock.has_focus(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock.is_open()) {
    ui_message_filter_checkboxes();

    const auto message_count = count_matching_log_entries(gDockState.log_filter);
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
  return gDockState.has_focus;
}

}  // namespace tactile::ui
