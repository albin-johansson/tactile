#include "viewport_widget.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "document_tabs.hpp"
#include "editor/gui/common/window.hpp"
#include "editor/model.hpp"
#include "home_page_content.hpp"

namespace Tactile {
namespace {

constexpr auto gWindowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
constinit bool gHasFocus = false;
constinit bool gMouseWithinWindow = false;

void RemoveTabBarFromNextWindow()
{
  ImGuiWindowClass wc{};
  wc.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
  ImGui::SetNextWindowClass(&wc);
}

}  // namespace

void UpdateViewportWidget(const Model& model,
                          const Icons& icons,
                          entt::dispatcher& dispatcher)
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{4, 4});
  RemoveTabBarFromNextWindow();

  auto window = Window{"Viewport", gWindowFlags};
  if (window.IsOpen()) {
    ImGui::PopStyleVar();
    gHasFocus = ImGui::IsWindowFocused();
    gMouseWithinWindow = Window::CurrentWindowContainsMouse();

    if (model.HasActiveDocument()) {
      UpdateDocumentTabs(model, dispatcher);
    }
    else {
      UpdateHomePageContent(icons, dispatcher);
    }
  }
  else {
    ImGui::PopStyleVar();
    gHasFocus = false;
    gMouseWithinWindow = false;
  }
}

void CenterViewport()
{
  CenterMapViewport();
}

auto IsViewportFocused() noexcept -> bool
{
  return gHasFocus;
}

auto IsMouseWithinViewport() noexcept -> bool
{
  return gMouseWithinWindow;
}

}  // namespace Tactile