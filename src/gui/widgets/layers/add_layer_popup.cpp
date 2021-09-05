#include "add_layer_popup.hpp"

#include <imgui.h>

#include "events/layer_events.hpp"
#include "gui/icons.hpp"

namespace Tactile {

void UpdateAddLayerPopup(entt::dispatcher& dispatcher)
{
  if (ImGui::BeginPopup("AddLayerPopup"))
  {
    if (ImGui::Selectable(TAC_ICON_TILE_LAYER " Tile layer"))
    {
      dispatcher.enqueue<AddLayerEvent>(LayerType::TileLayer);
    }

    if (ImGui::Selectable(TAC_ICON_OBJECT_LAYER " Object layer"))
    {
      dispatcher.enqueue<AddLayerEvent>(LayerType::ObjectLayer);
    }

    if (ImGui::Selectable(TAC_ICON_GROUP_LAYER " Group layer"))
    {
      dispatcher.enqueue<AddLayerEvent>(LayerType::GroupLayer);
    }

    ImGui::EndPopup();
  }
}

void OpenAddLayerPopup()
{
  ImGui::OpenPopup("AddLayerPopup");
}

}  // namespace Tactile