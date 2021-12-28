#include "docking_toolbar.hpp"

#include <cassert>  // assert

#include <imgui.h>
#include <imgui_internal.h>

namespace Tactile {

/**
 * This widget is shamelessly based on an example provided in this issue:
 * https://github.com/ocornut/imgui/issues/2648.
 */

void BeginDockingToolbar(NotNull<CStr> name, int& toolbarAxis)
{
  assert(name);

  const auto requestedSize =
      (toolbarAxis == ImGuiAxis_X) ? ImVec2{-1.0f, 0.0f} : ImVec2{0.0f, -1.0f};
  ImGui::SetNextWindowSize(requestedSize);

  ImGuiWindowClass windowClass;
  windowClass.DockingAllowUnclassed = true;
  windowClass.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoCloseButton;
  windowClass.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_HiddenTabBar;
  windowClass.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingSplitMe;
  windowClass.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingOverMe;
  windowClass.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingOverOther;
  if (toolbarAxis == ImGuiAxis_X) {
    windowClass.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoResizeY;
  }
  else {
    windowClass.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoResizeX;
  }
  ImGui::SetNextWindowClass(&windowClass);

  const auto fontSize = ImGui::GetFontSize();
  const ImVec2 iconSize{ImFloor(fontSize * 1.7f), ImFloor(fontSize * 1.7f)};

  constexpr auto window_flags = ImGuiWindowFlags_NoCollapse |
                                ImGuiWindowFlags_NoTitleBar |
                                ImGuiWindowFlags_NoScrollbar;
  ImGui::Begin(name, nullptr, window_flags);

  if (auto* node = ImGui::GetWindowDockNode()) {
    auto& style = ImGui::GetStyle();

    assert((toolbarAxis ^ 1) >= 0);
    const auto toolbarAxisPerp = static_cast<usize>(toolbarAxis ^ 1);
    const auto windowPadding = style.WindowPadding[toolbarAxisPerp];
    const auto sizeWhenDocked = windowPadding * 2.0f + iconSize[toolbarAxisPerp];

    node->WantLockSizeOnce = true;
    node->SizeRef[toolbarAxisPerp] = sizeWhenDocked;
    node->Size[toolbarAxisPerp] = sizeWhenDocked;

    if (node->ParentNode && node->ParentNode->SplitAxis != ImGuiAxis_None) {
      toolbarAxis = static_cast<ImGuiAxis>(node->ParentNode->SplitAxis ^ 1);
    }
  }
}

void EndDockingToolbar()
{
  ImGui::End();
}

}  // namespace Tactile
