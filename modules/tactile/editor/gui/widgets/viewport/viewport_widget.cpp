#include "viewport_widget.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "document_tabs.hpp"
#include "editor/model.hpp"
#include "home_page_content.hpp"

namespace Tactile {
namespace {

constexpr auto gWindowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
constinit bool gHasFocus = false;

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
  if (ImGui::Begin("Viewport", nullptr, gWindowFlags)) {
    ImGui::PopStyleVar();
    gHasFocus = ImGui::IsWindowFocused();

    if (model.HasActiveDocument()) {
      UpdateDocumentTabs(model, dispatcher);
    }
    else {
      UpdateHomePageContent(icons);
    }
  }
  else {
    ImGui::PopStyleVar();
    gHasFocus = false;
  }

  ImGui::End();
}

void CenterViewport()
{
  CenterMapViewport();
}

auto IsViewportFocused() noexcept -> bool
{
  return gHasFocus;
}

}  // namespace Tactile