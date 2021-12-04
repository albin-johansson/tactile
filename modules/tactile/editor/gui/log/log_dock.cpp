#include "log_dock.hpp"

#include <deque>    // deque
#include <utility>  // move

#include <tactile_def.hpp>

#include <imgui.h>

#include "editor/gui/common/window.hpp"
#include "editor/gui/icons.hpp"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

constinit bool gHasFocus = false;
constexpr auto gWindowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
constexpr auto gChildFlags = ImGuiWindowFlags_AlwaysVerticalScrollbar |
                             ImGuiWindowFlags_HorizontalScrollbar |
                             ImGuiWindowFlags_AlwaysAutoResize;

constexpr usize gLogCapacity = 256;
inline std::deque<std::string> gLogOutput;

}  // namespace

void UpdateLogDock()
{
  if (!Prefs::GetShowLogDock()) {
    return;
  }

  bool visible = Prefs::GetShowLogDock();

  auto dock = Window{"Log", gWindowFlags, &visible};
  gHasFocus = dock.IsFocused(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock.IsOpen()) {
    if (ImGui::BeginChild("##LogPane", ImVec2{}, true, gChildFlags)) {
      ImGuiListClipper clipper;
      clipper.Begin(static_cast<int>(gLogOutput.size()));
      while (clipper.Step()) {
        for (auto i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
          const auto& msg = gLogOutput.at(static_cast<usize>(i));
          ImGui::TextUnformatted(msg.c_str());
        }
      }
    }

    if (ImGui::BeginPopupContextWindow("#LogDockContext")) {
      if (ImGui::MenuItem(TAC_ICON_CLEAR_HISTORY " Clear Log")) {
        ClearLogEntries();
      }

      ImGui::EndPopup();
    }

    ImGui::EndChild();
  }

  Prefs::SetShowLogDock(visible);
}

void AddLogEntry(std::string msg)
{
  if (gLogOutput.size() >= gLogCapacity - 1) {
    gLogOutput.pop_back();
  }

  gLogOutput.push_back(std::move(msg));
}

void ClearLogEntries()
{
  gLogOutput.clear();
}

auto IsLogDockFocused() noexcept -> bool
{
  return gHasFocus;
}

}  // namespace Tactile
