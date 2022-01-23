#include "dock_space.hpp"

#include <filesystem>  // exists

#include <imgui.h>
#include <tactile_def.hpp>

#include "io/preferences.hpp"
#include "load_default_layout.hpp"

namespace tactile {
namespace {

constinit bool gInitialized = false;
constinit Maybe<ImGuiID> gRootId;

}  // namespace

void UpdateDockSpace()
{
  gRootId = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
  if (!gInitialized) {
    const auto size = ImGui::GetMainViewport()->Size;
    if (size.x > 0 && size.y > 0) {
      if (!prefs::GetRestoreLayout() || !std::filesystem::exists("imgui.ini")) {
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
