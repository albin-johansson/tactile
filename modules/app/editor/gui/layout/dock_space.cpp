#include "dock_space.hpp"

#include <filesystem>  // exists

#include <imgui.h>

#include "io/preferences.hpp"
#include "load_default_layout.hpp"
#include "tactile_def.hpp"

namespace tactile {
namespace {

constinit bool gInitialized = false;
constinit maybe<ImGuiID> gRootId;

}  // namespace

void UpdateDockSpace()
{
  gRootId = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
  if (!gInitialized) {
    const auto size = ImGui::GetMainViewport()->Size;
    if (size.x > 0 && size.y > 0) {
      const auto& prefs = get_preferences();
      if (!prefs.will_restore_layout() || !std::filesystem::exists("imgui.ini")) {
        LoadDefaultLayout(gRootId.value(), false);
      }
      gInitialized = true;
    }
  }
}

void ResetLayout()
{
  LoadDefaultLayout(gRootId.value(), true);
}

}  // namespace tactile
