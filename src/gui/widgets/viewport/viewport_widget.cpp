#include "viewport_widget.hpp"

#include <imgui.h>

#include "core/events/show_map_properties_event.hpp"
#include "core/model.hpp"
#include "gui/icons.hpp"
#include "map_content_viewport.hpp"
#include "no_content_viewport_widget.hpp"

namespace Tactile {

void UpdateViewportWidget(const Model& model, entt::dispatcher& dispatcher)
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{4, 4});

  if (ImGui::Begin("Viewport",
                   nullptr,
                   ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse))
  {
    if (model.GetActiveMapId())
    {
      MapContentViewport(model, dispatcher);
    }
    else
    {
      NoContentViewportWidget(dispatcher);
    }
  }

  ImGui::PopStyleVar();

  if (ImGui::BeginPopupContextItem())
  {
    if (ImGui::MenuItem(TAC_ICON_PROPERTIES " Show properties"))
    {
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

}  // namespace Tactile