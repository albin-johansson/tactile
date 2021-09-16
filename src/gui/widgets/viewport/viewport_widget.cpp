#include "viewport_widget.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "core/model.hpp"
#include "events/map_events.hpp"
#include "gui/icons.hpp"
#include "map_content_viewport.hpp"
#include "no_content_viewport_widget.hpp"

namespace Tactile {
namespace {

constinit bool has_focus = false;

}  // namespace

void UpdateViewportWidget(const Model& model, entt::dispatcher& dispatcher)
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{4, 4});

  {
    // Makes sure that the viewport never features a tab bar
    ImGuiWindowClass wc;
    wc.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
    ImGui::SetNextWindowClass(&wc);
  }

  if (ImGui::Begin("Viewport",
                   nullptr,
                   ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse))
  {
    has_focus = ImGui::IsWindowFocused();

    if (model.HasActiveDocument()) {
      MapContentViewport(model, dispatcher);
    }
    else {
      NoContentViewportWidget(dispatcher);
    }
  }
  else {
    has_focus = false;
  }

  ImGui::PopStyleVar();

  if (ImGui::BeginPopupContextItem()) {
    if (ImGui::MenuItem(TAC_ICON_PROPERTIES " Show properties")) {
      dispatcher.enqueue<ShowMapPropertiesEvent>();
    }

    ImGui::EndPopup();
  }

  ImGui::End();
}

void CenterViewport()
{
  CenterMapContentViewport();
}

auto IsViewportFocused() noexcept -> bool
{
  return has_focus;
}

}  // namespace Tactile