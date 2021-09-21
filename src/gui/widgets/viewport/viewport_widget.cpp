#include "viewport_widget.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "core/model.hpp"
#include "document_tabs.hpp"
#include "home_page_content.hpp"

namespace Tactile {
namespace {

constexpr auto window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
constinit bool has_focus = false;

void RemoveTabBarFromNextWindow()
{
  ImGuiWindowClass wc{};
  wc.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
  ImGui::SetNextWindowClass(&wc);
}

}  // namespace

void UpdateViewportWidget(const Model& model, entt::dispatcher& dispatcher)
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{4, 4});

  RemoveTabBarFromNextWindow();
  if (ImGui::Begin("Viewport", nullptr, window_flags)) {
    ImGui::PopStyleVar();
    has_focus = ImGui::IsWindowFocused();

    if (model.HasActiveDocument()) {
      UpdateDocumentTabs(model, dispatcher);
    }
    else {
      UpdateHomePageContent();
    }
  }
  else {
    ImGui::PopStyleVar();
    has_focus = false;
  }

  ImGui::End();
}

void CenterViewport()
{
  CenterMapViewport();
}

auto IsViewportFocused() noexcept -> bool
{
  return has_focus;
}

}  // namespace Tactile