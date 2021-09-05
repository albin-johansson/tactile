#include "layer_item_popup.hpp"

#include <imgui.h>

#include "core/systems/layer_system.hpp"
#include "events/layer_events.hpp"
#include "gui/icons.hpp"

namespace Tactile {

void UpdateLayerItemPopup(const entt::registry& registry,
                          entt::dispatcher& dispatcher,
                          const layer_id id)
{
  if (ImGui::BeginPopupContextItem())
  {
    if (ImGui::MenuItem(TAC_ICON_PROPERTIES " Show properties"))
    {
      dispatcher.enqueue<ShowLayerPropertiesEvent>(id);
    }

    const auto isLayerVisible = Sys::IsLayerVisible(registry, id);
    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_VISIBILITY " Toggle visibility",
                        nullptr,
                        isLayerVisible))
    {
      dispatcher.enqueue<SetLayerVisibleEvent>(id, !isLayerVisible);
    }

    auto opacity = Sys::GetLayerOpacity(registry, id);
    ImGui::AlignTextToFramePadding();
    ImGui::Text(TAC_ICON_OPACITY " Opacity");
    ImGui::SameLine();
    if (ImGui::SliderFloat("##OpacitySlider", &opacity, 0, 1.0f))
    {
      dispatcher.enqueue<SetLayerOpacityEvent>(id, opacity);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_MOVE_UP " Move layer up",
                        nullptr,
                        false,
                        Sys::CanMoveLayerUp(registry, id)))
    {
      dispatcher.enqueue<MoveLayerUpEvent>(id);
    }

    if (ImGui::MenuItem(TAC_ICON_MOVE_DOWN " Move layer down",
                        nullptr,
                        false,
                        Sys::CanMoveLayerDown(registry, id)))
    {
      dispatcher.enqueue<MoveLayerDownEvent>(id);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_DUPLICATE " Duplicate layer"))
    {
      dispatcher.enqueue<DuplicateLayerEvent>(id);
    }

    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove layer"))
    {
      dispatcher.enqueue<RemoveLayerEvent>(id);
    }

    ImGui::EndPopup();
  }
}

}  // namespace Tactile
