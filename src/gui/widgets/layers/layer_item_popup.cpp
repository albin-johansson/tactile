#include "layer_item_popup.hpp"

#include <imgui.h>

#include "core/events/layers/duplicate_layer_event.hpp"
#include "core/events/layers/move_layer_down_event.hpp"
#include "core/events/layers/move_layer_up_event.hpp"
#include "core/events/layers/remove_layer_event.hpp"
#include "core/events/layers/set_layer_opacity_event.hpp"
#include "core/events/layers/set_layer_visible_event.hpp"
#include "core/events/layers/show_layer_properties_event.hpp"
#include "core/map_document.hpp"
#include "gui/icons.hpp"

namespace Tactile {

void UpdateLayerItemPopup(const MapDocument& document,
                          entt::dispatcher& dispatcher,
                          const layer_id id)
{
  if (ImGui::BeginPopupContextItem())
  {
    if (ImGui::MenuItem(TAC_ICON_PROPERTIES " Show properties"))
    {
      dispatcher.enqueue<ShowLayerPropertiesEvent>(id);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_VISIBILITY " Toggle visibility",
                        nullptr,
                        document.IsLayerVisible(id)))
    {
      dispatcher.enqueue<SetLayerVisibleEvent>(id, !document.IsLayerVisible(id));
    }

    auto opacity = document.GetMap().GetOpacity(id);
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
                        document.CanMoveLayerUp(id)))
    {
      dispatcher.enqueue<MoveLayerUpEvent>(id);
    }

    if (ImGui::MenuItem(TAC_ICON_MOVE_DOWN " Move layer down",
                        nullptr,
                        false,
                        document.CanMoveLayerDown(id)))
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
