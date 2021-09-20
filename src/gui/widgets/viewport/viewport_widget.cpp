#include "viewport_widget.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "core/model.hpp"
#include "document_tab_bar.hpp"
#include "events/map_events.hpp"
#include "gui/icons.hpp"
#include "home_page_content.hpp"

namespace Tactile {
namespace {

constinit bool has_focus = false;

void RemoveTabBarFromNextWindow()
{
  ImGuiWindowClass wc{};
  wc.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
  ImGui::SetNextWindowClass(&wc);
}

void UpdateViewportContextMenu(entt::dispatcher& dispatcher)
{
  // FIXME this context menu won't show for all viewports when there are multiple maps
  if (ImGui::BeginPopupContextItem()) {
    if (ImGui::MenuItem(TAC_ICON_PROPERTIES " Show properties")) {
      dispatcher.enqueue<ShowMapPropertiesEvent>();
    }

    ImGui::EndPopup();
  }
}

}  // namespace

void UpdateViewportWidget(const Model& model, entt::dispatcher& dispatcher)
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{4, 4});

  RemoveTabBarFromNextWindow();
  if (ImGui::Begin("Viewport",
                   nullptr,
                   ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse))
  {
    has_focus = ImGui::IsWindowFocused();

    if (model.HasActiveDocument()) {
      UpdateDocumentTabBar(model, dispatcher);
    }
    else {
      UpdateHomePageContent(dispatcher);
    }
  }
  else {
    has_focus = false;
  }

  ImGui::PopStyleVar();
  UpdateViewportContextMenu(dispatcher);

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