#include "add_layer_popup.hpp"

#include <imgui.h>

#include "editor/events/layer_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"

namespace tactile {

void UpdateAddLayerPopup(entt::dispatcher& dispatcher)
{
  if (scoped::Popup popup{"AddLayerPopup"}; popup.IsOpen()) {
    if (ImGui::Selectable(TAC_ICON_TILE_LAYER " Tile layer")) {
      dispatcher.enqueue<AddLayerEvent>(layer_type::tile_layer);
    }

    if (ImGui::Selectable(TAC_ICON_OBJECT_LAYER " Object layer")) {
      dispatcher.enqueue<AddLayerEvent>(layer_type::object_layer);
    }

    if (ImGui::Selectable(TAC_ICON_GROUP_LAYER " Group layer")) {
      dispatcher.enqueue<AddLayerEvent>(layer_type::group_layer);
    }
  }
}

void OpenAddLayerPopup()
{
  ImGui::OpenPopup("AddLayerPopup");
}

}  // namespace tactile