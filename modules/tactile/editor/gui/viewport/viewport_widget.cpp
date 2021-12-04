#include "viewport_widget.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "document_tabs.hpp"
#include "editor/gui/scoped.hpp"
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
  Scoped::StyleVar padding{ImGuiStyleVar_WindowPadding, {4, 4}};
  RemoveTabBarFromNextWindow();

  Scoped::Window window{"Viewport", gWindowFlags};
  if (window.IsOpen()) {
    padding.Pop();
    gHasFocus = ImGui::IsWindowFocused();
    gMouseWithinWindow = Scoped::Window::CurrentWindowContainsMouse();

    if (model.HasActiveDocument()) {
      UpdateDocumentTabs(model, dispatcher);
    }
    else {
      UpdateHomePageContent(icons, dispatcher);
    }
  }
  else {
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