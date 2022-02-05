#include "layer_item_popup.hpp"

#include <imgui.h>

#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/layer_tree_system.hpp"
#include "editor/events/layer_events.hpp"
#include "editor/events/property_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "misc/assert.hpp"

namespace tactile {

void UpdateLayerItemPopup(const entt::registry& registry,
                          entt::dispatcher& dispatcher,
                          const layer_id id)
{
  if (auto popup = scoped::popup::for_item("##LayerItemPopup"); popup.is_open()) {
    const auto [entity, layer] = sys::get_layer(registry, id);

    if (ImGui::MenuItem(TAC_ICON_INSPECT " Inspect Layer")) {
      dispatcher.enqueue<inspect_context_event>(entity);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_EDIT " Rename Layer")) {
      dispatcher.enqueue<open_rename_layer_dialog_event>(id);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_DUPLICATE " Duplicate Layer")) {
      dispatcher.enqueue<duplicate_layer_event>(id);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Layer")) {
      dispatcher.enqueue<remove_layer_event>(id);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_VISIBILITY " Toggle Layer Visibility",
                        nullptr,
                        layer.visible)) {
      dispatcher.enqueue<set_layer_visible_event>(id, !layer.visible);
    }

    if (auto opacity = layer.opacity; ImGui::SliderFloat("Opacity", &opacity, 0, 1.0f)) {
      dispatcher.enqueue<set_layer_opacity_event>(id, opacity);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_MOVE_UP " Move Layer Up",
                        nullptr,
                        false,
                        sys::can_move_layer_up(registry, entity))) {
      dispatcher.enqueue<move_layer_up_event>(id);
    }

    if (ImGui::MenuItem(TAC_ICON_MOVE_DOWN " Move Layer Down",
                        nullptr,
                        false,
                        sys::can_move_layer_down(registry, entity))) {
      dispatcher.enqueue<move_layer_down_event>(id);
    }
  }
}

}  // namespace tactile
