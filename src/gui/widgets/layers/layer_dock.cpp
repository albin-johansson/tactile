#include "layer_dock.hpp"

#include <imgui.h>

#include <limits>  // numeric_limits

#include "add_layer_popup.hpp"
#include "core/components/layer.hpp"
#include "core/components/parent.hpp"
#include "core/systems/layer_system.hpp"
#include "events/layer_events.hpp"
#include "gui/icons.hpp"
#include "gui/widgets/common/button.hpp"
#include "io/preferences.hpp"
#include "layer_item.hpp"

namespace Tactile {
namespace {

void UpdateLayerDockButtons(const entt::registry& registry,
                            entt::dispatcher& dispatcher)
{
  const auto activeLayerEntity = registry.ctx<ActiveLayer>().entity;
  const auto hasActiveLayer = activeLayerEntity != entt::null;

  Maybe<LayerID> activeLayerId;
  if (hasActiveLayer)
  {
    const auto& layer = registry.get<Layer>(activeLayerEntity);
    activeLayerId = layer.id;
  }

  if (Button(TAC_ICON_ADD, "Add new layer."))
  {
    OpenAddLayerPopup();
  }

  UpdateAddLayerPopup(dispatcher);

  ImGui::SameLine();
  if (Button(TAC_ICON_REMOVE, "Remove layer.", hasActiveLayer))
  {
    dispatcher.enqueue<RemoveLayerEvent>(*activeLayerId);
  }

  ImGui::SameLine();
  if (Button(TAC_ICON_DUPLICATE, "Duplicate layer.", hasActiveLayer))
  {
    dispatcher.enqueue<DuplicateLayerEvent>(*activeLayerId);
  }

  ImGui::SameLine();

  if (Button(TAC_ICON_MOVE_UP,
             "Move layer up.",
             activeLayerId && Sys::CanMoveLayerUp(registry, *activeLayerId)))
  {
    dispatcher.enqueue<MoveLayerUpEvent>(*activeLayerId);
  }

  ImGui::SameLine();

  if (Button(TAC_ICON_MOVE_DOWN,
             "Move layer down.",
             activeLayerId && Sys::CanMoveLayerDown(registry, *activeLayerId)))
  {
    dispatcher.enqueue<MoveLayerDownEvent>(*activeLayerId);
  }
}

}  // namespace

void UpdateLayerDock(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  if (!Prefs::GetShowLayerDock())
  {
    return;
  }

  bool isVisible = Prefs::GetShowLayerDock();
  if (ImGui::Begin("Layers", &isVisible, ImGuiWindowFlags_NoCollapse))
  {
    UpdateLayerDockButtons(registry, dispatcher);
    const auto windowHeight = ImGui::GetWindowHeight();
    const auto textLineHeight = ImGui::GetTextLineHeightWithSpacing();
    const auto size = ImVec2{std::numeric_limits<float>::min(),
                             windowHeight - (4 * textLineHeight)};

    if (ImGui::BeginListBox("##LayerTreeNode", size))
    {
      for (auto&& [entity, layer] : registry.view<Layer>().each())
      {
        /* Note, we rely on the Layer pool being sorted by layer index, so we can't
           include Parent in the view query directly. */
        const auto& parent = registry.get<Parent>(entity);
        if (parent.entity == entt::null)
        {
          LayerItem(registry, dispatcher, entity, layer);
        }
      }

      ImGui::EndListBox();
    }
  }

  Prefs::SetShowLayerDock(isVisible);
  ImGui::End();
}

}  // namespace Tactile
