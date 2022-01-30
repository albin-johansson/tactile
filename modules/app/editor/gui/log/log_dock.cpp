#include "log_dock.hpp"

#include <imgui.h>

#include "editor/gui/common/centered_text.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "io/preferences.hpp"
#include "logging.hpp"
#include "tactile_def.hpp"
#include "tactile_stdlib.hpp"
#include "throw.hpp"

namespace tactile {
namespace {

constexpr auto gWindowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
constexpr auto gChildFlags = ImGuiWindowFlags_AlwaysVerticalScrollbar |
                             ImGuiWindowFlags_HorizontalScrollbar |
                             ImGuiWindowFlags_AlwaysAutoResize;

// TODO should verbose/debug options be disabled in release builds?
[[nodiscard]] auto ShowLogLevelFilterCombo(const log_level currentLevel)
    -> maybe<log_level>
{
  static constexpr c_str verboseFilter = "Everything";
  static constexpr c_str debugFilter = "Debug / Information / Warnings / Errors";
  static constexpr c_str infoFilter = "Information / Warnings / Errors";
  static constexpr c_str warningFilter = "Warnings / Errors";
  static constexpr c_str errorFilter = "Errors";

  static const auto comboWidth = ImGui::CalcTextSize(debugFilter).x * 1.2f;

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted("Filter:");

  ImGui::SameLine();

  c_str filter = verboseFilter;
  switch (currentLevel) {
    case log_level::verbose:
      filter = verboseFilter;
      break;

    case log_level::debug:
      filter = debugFilter;
      break;

    case log_level::info:
      filter = infoFilter;
      break;

    case log_level::warning:
      filter = warningFilter;
      break;

    case log_level::error:
      filter = errorFilter;
      break;
  }

  ImGui::SetNextItemWidth(comboWidth);
  if (scoped::Combo filterCombo{"##LogFilterCombo", filter}; filterCombo.IsOpen()) {
    if (ImGui::MenuItem(verboseFilter)) {
      return log_level::verbose;
    }

    if (ImGui::MenuItem(debugFilter)) {
      return log_level::debug;
    }

    if (ImGui::MenuItem(infoFilter)) {
      return log_level::info;
    }

    if (ImGui::MenuItem(warningFilter)) {
      return log_level::warning;
    }

    if (ImGui::MenuItem(errorFilter)) {
      return log_level::error;
    }
  }

  return nothing;
}

[[nodiscard]] auto GetColorForLevel(const log_level level) -> ImVec4
{
  switch (level) {
    case log_level::verbose:
      return {0.93f, 0.51f, 0.93f, 1.0f};

    case log_level::debug:
      return {0.5f, 1.0f, 0.7f, 1.0f};

    case log_level::info:
      return {1.0f, 1.0f, 1.0f, 1.00f};

    case log_level::warning:
      return {1.0f, 1.0f, 0.00f, 1.00f};

    case log_level::error:
      return {1.00f, 0.27f, 0.00f, 1.00f};

    default:
      throw_traced(tactile_error{"Did not recognize log level!"});
  }
}

void ShowColorLegendHint()
{
  ImGui::TextDisabled("(?)");

  if (ImGui::IsItemHovered()) {
    scoped::StyleColor bg{ImGuiCol_PopupBg, {0.1f, 0.1f, 0.1f, 0.75f}};
    scoped::Tooltip tooltip;

    static const auto verboseColor = GetColorForLevel(log_level::verbose);
    static const auto debugColor = GetColorForLevel(log_level::debug);
    static const auto infoColor = GetColorForLevel(log_level::info);
    static const auto warningColor = GetColorForLevel(log_level::warning);
    static const auto errorColor = GetColorForLevel(log_level::error);

    ImGui::TextColored(verboseColor, "Verbose message");
    ImGui::TextColored(debugColor, "Debug message");
    ImGui::TextColored(infoColor, "Info message");
    ImGui::TextColored(warningColor, "Warning message");
    ImGui::TextColored(errorColor, "Error message");
  }
}

void ShowLogContents(const log_level filter)
{
  scoped::StyleColor childBg{ImGuiCol_ChildBg, {0.1f, 0.1f, 0.1f, 0.75f}};

  if (scoped::Child pane{"##LogPane", {}, true, gChildFlags}; pane.IsOpen()) {
    ImGuiListClipper clipper;
    clipper.Begin(static_cast<int>(log_size(filter)));

    while (clipper.Step()) {
      for (auto i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
        const auto& [level, str] = get_filtered_log_entry(filter, static_cast<usize>(i));
        const auto color = GetColorForLevel(level);
        ImGui::TextColored(color, "%s", str.c_str());
      }
    }
  }
}

}  // namespace

void LogDock::Update()
{
  if (!prefs::GetShowLogDock()) {
    return;
  }

  bool visible = prefs::GetShowLogDock();

  scoped::Window dock{"Log", gWindowFlags, &visible};
  mHasFocus = dock.IsFocused(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock.IsOpen()) {
    if (const auto level = ShowLogLevelFilterCombo(mLogLevel)) {
      mLogLevel = *level;
    }

    ImGui::SameLine();
    ShowColorLegendHint();

    if (log_size(mLogLevel) != 0u) {
      ShowLogContents(mLogLevel);
    }
    else {
      CenteredText("No logged messages match the current filter.");
    }

    if (auto popup = scoped::Popup::ForWindow("##LogDockContext"); popup.IsOpen()) {
      if (ImGui::MenuItem(TAC_ICON_CLEAR_HISTORY " Clear Log")) {
        clear_log_history();
      }
    }
  }

  prefs::SetShowLogDock(visible);
}

}  // namespace tactile
