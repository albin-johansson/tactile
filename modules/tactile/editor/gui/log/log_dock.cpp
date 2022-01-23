#include "log_dock.hpp"

#include <imgui.h>
#include <tactile_def.hpp>
#include <tactile_stdlib.hpp>

#include "editor/gui/common/centered_text.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "io/preferences.hpp"
#include "logging.hpp"
#include "throw.hpp"

namespace tactile {
namespace {

constexpr auto gWindowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
constexpr auto gChildFlags = ImGuiWindowFlags_AlwaysVerticalScrollbar |
                             ImGuiWindowFlags_HorizontalScrollbar |
                             ImGuiWindowFlags_AlwaysAutoResize;

// TODO should verbose/debug options be disabled in release builds?
[[nodiscard]] auto ShowLogLevelFilterCombo(const LogLevel currentLevel) -> Maybe<LogLevel>
{
  static constexpr CStr verboseFilter = "Everything";
  static constexpr CStr debugFilter = "Debug / Information / Warnings / Errors";
  static constexpr CStr infoFilter = "Information / Warnings / Errors";
  static constexpr CStr warningFilter = "Warnings / Errors";
  static constexpr CStr errorFilter = "Errors";

  static const auto comboWidth = ImGui::CalcTextSize(debugFilter).x * 1.2f;

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted("Filter:");

  ImGui::SameLine();

  CStr filter = verboseFilter;
  switch (currentLevel) {
    case LogLevel::Verbose:
      filter = verboseFilter;
      break;

    case LogLevel::Debug:
      filter = debugFilter;
      break;

    case LogLevel::Info:
      filter = infoFilter;
      break;

    case LogLevel::Warning:
      filter = warningFilter;
      break;

    case LogLevel::Error:
      filter = errorFilter;
      break;
  }

  ImGui::SetNextItemWidth(comboWidth);
  if (Scoped::Combo filterCombo{"##LogFilterCombo", filter}; filterCombo.IsOpen()) {
    if (ImGui::MenuItem(verboseFilter)) {
      return LogLevel::Verbose;
    }

    if (ImGui::MenuItem(debugFilter)) {
      return LogLevel::Debug;
    }

    if (ImGui::MenuItem(infoFilter)) {
      return LogLevel::Info;
    }

    if (ImGui::MenuItem(warningFilter)) {
      return LogLevel::Warning;
    }

    if (ImGui::MenuItem(errorFilter)) {
      return LogLevel::Error;
    }
  }

  return nothing;
}

[[nodiscard]] auto GetColorForLevel(const LogLevel level) -> ImVec4
{
  switch (level) {
    case LogLevel::Verbose:
      return {0.93f, 0.51f, 0.93f, 1.0f};

    case LogLevel::Debug:
      return {0.5f, 1.0f, 0.7f, 1.0f};

    case LogLevel::Info:
      return {1.0f, 1.0f, 1.0f, 1.00f};

    case LogLevel::Warning:
      return {1.0f, 1.0f, 0.00f, 1.00f};

    case LogLevel::Error:
      return {1.00f, 0.27f, 0.00f, 1.00f};

    default:
      ThrowTraced(TactileError{"Did not recognize log level!"});
  }
}

void ShowColorLegendHint()
{
  ImGui::TextDisabled("(?)");

  if (ImGui::IsItemHovered()) {
    Scoped::StyleColor bg{ImGuiCol_PopupBg, {0.1f, 0.1f, 0.1f, 0.75f}};
    Scoped::Tooltip tooltip;

    static const auto verboseColor = GetColorForLevel(LogLevel::Verbose);
    static const auto debugColor = GetColorForLevel(LogLevel::Debug);
    static const auto infoColor = GetColorForLevel(LogLevel::Info);
    static const auto warningColor = GetColorForLevel(LogLevel::Warning);
    static const auto errorColor = GetColorForLevel(LogLevel::Error);

    ImGui::TextColored(verboseColor, "Verbose message");
    ImGui::TextColored(debugColor, "Debug message");
    ImGui::TextColored(infoColor, "Info message");
    ImGui::TextColored(warningColor, "Warning message");
    ImGui::TextColored(errorColor, "Error message");
  }
}

void ShowLogContents(const LogLevel filter)
{
  Scoped::StyleColor childBg{ImGuiCol_ChildBg, {0.1f, 0.1f, 0.1f, 0.75f}};

  if (Scoped::Child pane{"##LogPane", {}, true, gChildFlags}; pane.IsOpen()) {
    ImGuiListClipper clipper;
    clipper.Begin(static_cast<int>(GetLogSize(filter)));

    while (clipper.Step()) {
      for (auto i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
        const auto& [level, str] = GetFilteredLogEntry(filter, static_cast<usize>(i));
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

  Scoped::Window dock{"Log", gWindowFlags, &visible};
  mHasFocus = dock.IsFocused(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock.IsOpen()) {
    if (const auto level = ShowLogLevelFilterCombo(mLogLevel)) {
      mLogLevel = *level;
    }

    ImGui::SameLine();
    ShowColorLegendHint();

    if (GetLogSize(mLogLevel) != 0u) {
      ShowLogContents(mLogLevel);
    }
    else {
      CenteredText("No logged messages match the current filter.");
    }

    if (auto popup = Scoped::Popup::ForWindow("##LogDockContext"); popup.IsOpen()) {
      if (ImGui::MenuItem(TAC_ICON_CLEAR_HISTORY " Clear Log")) {
        ClearLogHistory();
      }
    }
  }

  prefs::SetShowLogDock(visible);
}

}  // namespace tactile
