#include "layer_item_popup.hpp"

#include <imgui.h>

#include "assert.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "editor/events/layer_events.hpp"
#include "editor/events/property_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"

namespace Tactile {

void UpdateLayerItemPopup(const entt::registry& registry,
                          entt::dispatcher& dispatcher,
                          const LayerID id)
{
  if (auto popup = Scoped::Popup::ForItem("##LayerItemPopup"); popup.IsOpen()) {
    if (ImGui::MenuItem(TAC_ICON_INSPECT " Inspect Layer")) {
      const auto entity = Sys::FindLayer(registry, id);
      TACTILE_ASSERT(entity != entt::null);
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
    if (const auto isLayerVisible = Sys::IsLayerVisible(registry, id);
        ImGui::MenuItem(TAC_ICON_VISIBILITY " Toggle Layer Visibility",
                        nullptr,
                        isLayerVisible)) {
      dispatcher.enqueue<SetLayerVisibleEvent>(id, !isLayerVisible);
    }

    if (auto opacity = Sys::GetLayerOpacity(registry, id);
        ImGui::SliderFloat("Opacity", &opacity, 0, 1.0f)) {
      dispatcher.enqueue<SetLayerOpacityEvent>(id, opacity);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_MOVE_UP " Move Layer Up",
                        nullptr,
                        false,
                        Sys::CanMoveLayerUp(registry, id))) {
      dispatcher.enqueue<MoveLayerUpEvent>(id);
    }

    if (ImGui::MenuItem(TAC_ICON_MOVE_DOWN " Move Layer Down",
                        nullptr,
                        false,
                        Sys::CanMoveLayerDown(registry, id))) {
      dispatcher.enqueue<MoveLayerDownEvent>(id);
    }
  }
}

}  // namespace Tactile
