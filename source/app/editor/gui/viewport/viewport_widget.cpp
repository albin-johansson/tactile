#include "viewport_widget.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "document_tabs.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"
#include "home_page_content.hpp"

namespace tactile {
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
                          const icon_manager& icons,
                          entt::dispatcher& dispatcher)
{
  scoped::style_var padding{ImGuiStyleVar_WindowPadding, {4, 4}};
  RemoveTabBarFromNextWindow();

  scoped::window window{"Viewport", gWindowFlags};
  if (window.is_open()) {
    padding.pop();
    gHasFocus = ImGui::IsWindowFocused();
    gMouseWithinWindow = scoped::window::current_window_contains_mouse();

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

}  // namespace tactile