#include "add_layer_popup.hpp"

#include <imgui.h>

#include "editor/events/layer_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"

namespace Tactile {

void UpdateAddLayerPopup(entt::dispatcher& dispatcher)
{
  if (Scoped::Popup popup{"AddLayerPopup"}; popup.IsOpen()) {
    if (ImGui::Selectable(TAC_ICON_TILE_LAYER " Tile layer")) {
      dispatcher.enqueue<AddLayerEvent>(LayerType::TileLayer);
    }

    if (ImGui::Selectable(TAC_ICON_OBJECT_LAYER " Object layer")) {
      dispatcher.enqueue<AddLayerEvent>(LayerType::ObjectLayer);
    }

    if (ImGui::Selectable(TAC_ICON_GROUP_LAYER " Group layer")) {
      dispatcher.enqueue<AddLayerEvent>(LayerType::GroupLayer);
    }
  }
}

void OpenAddLayerPopup()
{
  ImGui::OpenPopup("AddLayerPopup");
}

}  // namespace Tactile