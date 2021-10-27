#include "log_dock.hpp"

#include <deque>    // deque
#include <utility>  // move

#include <tactile-base/tactile_std.hpp>

#include <imgui.h>

#include "editor/gui/icons.hpp"
#include "editor/gui/widgets/common/window.hpp"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

constinit bool has_focus = false;
constexpr auto window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
constexpr auto child_flags = ImGuiWindowFlags_AlwaysVerticalScrollbar |
                             ImGuiWindowFlags_HorizontalScrollbar |
                             ImGuiWindowFlags_AlwaysAutoResize;

constexpr usize log_capacity = 256;
inline std::deque<std::string> log_output;

}  // namespace

void UpdateLogDock()
{
  if (!Prefs::GetShowLogDock()) {
    return;
  }

  bool visible = Prefs::GetShowLogDock();

  auto dock = Window{"Log", window_flags, &visible};
  has_focus = dock.IsFocused(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock) {
    if (ImGui::BeginChild("##LogPane", ImVec2{}, true, child_flags)) {
      ImGuiListClipper clipper;
      clipper.Begin(static_cast<int>(log_output.size()));
      while (clipper.Step()) {
        for (auto i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
          const auto& msg = log_output.at(static_cast<usize>(i));
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
  if (log_output.size() >= log_capacity - 1) {
    log_output.pop_back();
  }

  log_output.push_back(std::move(msg));
}

void ClearLogEntries()
{
  log_output.clear();
}

auto IsLogDockFocused() noexcept -> bool
{
  return has_focus;
}

}  // namespace Tactile
