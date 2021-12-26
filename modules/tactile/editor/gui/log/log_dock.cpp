#include "log_dock.hpp"

#include <deque>    // deque
#include <utility>  // move

#include <imgui.h>
#include <tactile_def.hpp>

#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "io/preferences.hpp"
#include "logging.hpp"

namespace Tactile {
namespace {

constinit bool gHasFocus = false;
constexpr auto gWindowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
constexpr auto gChildFlags = ImGuiWindowFlags_AlwaysVerticalScrollbar |
                             ImGuiWindowFlags_HorizontalScrollbar |
                             ImGuiWindowFlags_AlwaysAutoResize;

}  // namespace

void UpdateLogDock()
{
  if (!Prefs::GetShowLogDock()) {
    return;
  }

  bool visible = Prefs::GetShowLogDock();

  Scoped::Window dock{"Log", gWindowFlags, &visible};
  gHasFocus = dock.IsFocused(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock.IsOpen()) {
    Scoped::Child pane{"##LogPane", ImVec2{}, true, gChildFlags};
    if (pane.IsOpen()) {
      ImGuiListClipper clipper;
      clipper.Begin(static_cast<int>(GetLogSize()));
      while (clipper.Step()) {
        for (auto i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
          const auto& msg = GetLoggedString(static_cast<usize>(i));
          ImGui::TextUnformatted(msg.c_str());
        }
      }
    }

    if (auto popup = Scoped::Popup::ForWindow("##LogDockContext"); popup.IsOpen()) {
      if (ImGui::MenuItem(TAC_ICON_CLEAR_HISTORY " Clear Log")) {
        ClearLogHistory();
      }
    }
  }

  Prefs::SetShowLogDock(visible);
}

auto IsLogDockFocused() noexcept -> bool
{
  return gHasFocus;
}

}  // namespace Tactile
