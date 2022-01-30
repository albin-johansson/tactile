#include "layer_item_popup.hpp"

#include <imgui.h>

#include "assert.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/layer_tree_system.hpp"
#include "editor/events/layer_events.hpp"
#include "editor/events/property_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"

namespace tactile {

void UpdateLayerItemPopup(const entt::registry& registry,
                          entt::dispatcher& dispatcher,
                          const layer_id id)
{
  if (auto popup = scoped::Popup::ForItem("##LayerItemPopup"); popup.IsOpen()) {
    const auto [entity, layer] = sys::get_layer(registry, id);

    if (ImGui::MenuItem(TAC_ICON_INSPECT " Inspect Layer")) {
      dispatcher.enqueue<InspectContextEvent>(entity);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_EDIT " Rename Layer")) {
      dispatcher.enqueue<OpenRenameLayerDialogEvent>(id);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_DUPLICATE " Duplicate Layer")) {
      dispatcher.enqueue<DuplicateLayerEvent>(id);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Layer")) {
      dispatcher.enqueue<RemoveLayerEvent>(id);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_VISIBILITY " Toggle Layer Visibility",
                        nullptr,
                        layer.visible)) {
      dispatcher.enqueue<SetLayerVisibleEvent>(id, !layer.visible);
    }

    if (auto opacity = layer.opacity; ImGui::SliderFloat("Opacity", &opacity, 0, 1.0f)) {
      dispatcher.enqueue<SetLayerOpacityEvent>(id, opacity);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_MOVE_UP " Move Layer Up",
                        nullptr,
                        false,
                        sys::can_move_layer_up(registry, entity))) {
      dispatcher.enqueue<MoveLayerUpEvent>(id);
    }

    if (ImGui::MenuItem(TAC_ICON_MOVE_DOWN " Move Layer Down",
                        nullptr,
                        false,
                        sys::can_move_layer_down(registry, entity))) {
      dispatcher.enqueue<MoveLayerDownEvent>(id);
    }
  }
}

}  // namespace tactile
