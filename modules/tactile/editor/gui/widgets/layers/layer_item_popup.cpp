#include "layer_item_popup.hpp"

#include <imgui.h>

#include "tactile/core/systems/layers/layer_system.hpp"
#include "tactile/editor/events/layer_events.hpp"
#include "tactile/editor/gui/icons.hpp"

namespace Tactile {

void UpdateLayerItemPopup(const entt::registry& registry,
                          entt::dispatcher& dispatcher,
                          const LayerID id)
{
  if (ImGui::BeginPopupContextItem("##LayerItemPopup")) {
    if (ImGui::MenuItem(TAC_ICON_PROPERTIES " Show Layer Properties")) {
      dispatcher.enqueue<ShowLayerPropertiesEvent>(id);
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
                        isLayerVisible))
    {
      dispatcher.enqueue<SetLayerVisibleEvent>(id, !isLayerVisible);
    }

    auto opacity = Sys::GetLayerOpacity(registry, id);
    ImGui::AlignTextToFramePadding();
    ImGui::Text(TAC_ICON_OPACITY " Opacity");
    ImGui::SameLine();
    if (ImGui::SliderFloat("##OpacitySlider", &opacity, 0, 1.0f)) {
      dispatcher.enqueue<SetLayerOpacityEvent>(id, opacity);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_MOVE_UP " Move Layer Up",
                        nullptr,
                        false,
                        Sys::CanMoveLayerUp(registry, id)))
    {
      dispatcher.enqueue<MoveLayerUpEvent>(id);
    }

    if (ImGui::MenuItem(TAC_ICON_MOVE_DOWN " Move Layer Down",
                        nullptr,
                        false,
                        Sys::CanMoveLayerDown(registry, id)))
    {
      dispatcher.enqueue<MoveLayerDownEvent>(id);
    }

    ImGui::EndPopup();
  }
}

}  // namespace Tactile
