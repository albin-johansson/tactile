#include "viewport_widget.hpp"

#include "core/model.hpp"
#include "imgui.h"
#include "map_content_viewport.hpp"
#include "no_content_viewport_widget.hpp"

namespace tactile {

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
  ImGui::End();
}

void CenterViewport()
{
  CenterMapContentViewport();
}

}  // namespace tactile