#include "dock_space.hpp"

#include <filesystem>  // exists

#include <imgui.h>

#include <tactile-base/tactile_std.hpp>

#include "load_default_layout.hpp"
#include "tactile/io/preferences.hpp"

namespace Tactile {
namespace {

constinit bool initialized = false;
constinit Maybe<ImGuiID> root_id;

}  // namespace

void UpdateDockSpace()
{
  root_id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
  if (!initialized) {
    const auto size = ImGui::GetMainViewport()->Size;
    if (size.x > 0 && size.y > 0) {
      if (!Prefs::GetRestoreLayout() || !std::filesystem::exists("imgui.ini")) {
        LoadDefaultLayout(root_id.value(), false);
      }
      initialized = true;
    }
  }
}

void ResetLayout()
{
  LoadDefaultLayout(root_id.value(), true);
}

}  // namespace Tactile
