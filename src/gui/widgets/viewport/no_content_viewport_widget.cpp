#include "no_content_viewport_widget.hpp"

#include "core/events/add_map_event.hpp"
#include "core/events/open_map_event.hpp"
#include "imgui.h"

namespace Tactile {

void NoContentViewportWidget(entt::dispatcher& dispatcher)
{
  ImGui::Indent(50);

  ImGui::Spacing();
  ImGui::Spacing();

  ImGui::Text("No active map");

  ImGui::Spacing();
  if (ImGui::Button("Create new map"))
  {
    dispatcher.enqueue<AddMapEvent>();
  }

  ImGui::Spacing();
  if (ImGui::Button("Open existing map"))
  {
    dispatcher.enqueue<OpenMapEvent>();
  }
}

}  // namespace Tactile
