#include "add_layer_context_menu.hpp"

#include <imgui.h>

#include "editor/events/layer_events.hpp"
#include "editor/gui/icons.hpp"

namespace tactile {

add_layer_context_menu::add_layer_context_menu() : context_menu{"AddLayerContextMenu"} {}

void add_layer_context_menu::on_update(const document_model&,
                                       entt::dispatcher& dispatcher)
{
  if (ImGui::Selectable(TAC_ICON_TILE_LAYER " Tile layer")) {
    dispatcher.enqueue<add_layer_event>(layer_type::tile_layer);
  }

  if (ImGui::Selectable(TAC_ICON_OBJECT_LAYER " Object layer")) {
    dispatcher.enqueue<add_layer_event>(layer_type::object_layer);
  }

  if (ImGui::Selectable(TAC_ICON_GROUP_LAYER " Group layer")) {
    dispatcher.enqueue<add_layer_event>(layer_type::group_layer);
  }
}

}  // namespace tactile